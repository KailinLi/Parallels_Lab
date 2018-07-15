// mpic++ mpi.cc -o mpi `pkg-config --cflags --libs opencv`
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <mpi.h>
using namespace cv;
using namespace std;

const int EROSION_SIZE = 5;
const int EROSION_MATRIX[EROSION_SIZE][EROSION_SIZE] = {
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0}
};
const int EMPTY = 0;
const int FULL = 255;
const int OMP_CNT = 10;

void erosion_line(uchar *image, uchar *res, int pos, int rows, int cols) {
    uchar* aim = res + pos * cols;
    if (pos < EROSION_SIZE / 2 || pos >= rows - EROSION_SIZE / 2) {
        for (int l = 0; l < cols; ++l)
            aim[l] = FULL;
        return;
    }
    uchar* cache[EROSION_SIZE];
    for (int i = 0; i < EROSION_SIZE; ++i) {
        cache[i] = image + (i + pos - EROSION_SIZE / 2) * cols;
    }
    int l;
    for (l = 0; l < EROSION_SIZE / 2 && l < cols; ++l) {
        aim[l] = FULL;
    }
    for (; l < cols - EROSION_SIZE / 2; ++l) {
        bool result = true;
        for (int i = 0; i < EROSION_SIZE && result; ++i) {
            for (int j = 0; j < EROSION_SIZE; ++j) {
                if (EROSION_MATRIX[i][j] && !cache[i][j + l - EROSION_SIZE / 2]) {
                    result = false;
                    break;
                }
            }
        }
        if (result) aim[l] = FULL;
        else aim[l] = EMPTY;
    }
    for (; l < cols; ++l)
        aim[l] = FULL;
}

int main (int argc, char *argv[]) {
    if (argc != 2) return -1;

    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Mat image, res;
    int *send_msg = nullptr;
    int *rcv_msg = nullptr;
    uchar *image_data = nullptr;
    uchar *res_data = nullptr;
    uchar *part_data = nullptr;
    uchar *part_res = nullptr;
    int *sendcounts = nullptr;
    int *displs = nullptr;
    if (rank == 0) {
        image = imread(argv[1], IMREAD_GRAYSCALE);      // read image
        threshold(image, image, 105, 255, CV_THRESH_BINARY_INV);  // turn image to binary
        res = image.clone();
        res_data = res.ptr<uchar>(0);
        image_data = image.ptr<uchar>(0);
        int data_length = image.cols * image.rows;
        int part_length = data_length / size;
        // res_data = (uchar *)malloc(sizeof(uchar) * data_length);
        send_msg = (int *)malloc(sizeof(int) * size * 3);

        sendcounts = (int *)malloc(sizeof(int) * size);
        displs = (int *)malloc(sizeof(int) * size);

        for (int i = 0; i < size; ++i) {
            int begin = i * image.rows / size;
            int end = min((i + 1) * image.rows / size, image.rows);
            sendcounts[i] = (end - begin) * image.cols + 
                            (EROSION_SIZE / 2) * ((begin == 0 || end == image.rows) ? 1 : 2) * image.cols;
            displs[i] = max((begin - (EROSION_SIZE / 2)) * image.cols, 0);
            send_msg[i * 3] = image.cols;
            send_msg[i * 3 + 1] = sendcounts[i];
            send_msg[i * 3 + 2] = (begin == 0) ? 0 : ((end != image.rows) ? 1 : 2);
            // cout << send_msg[i * 2] << "**" << send_msg[i * 2 + 1] << endl;
            // cout << begin << "&& "<< end << "^^" << displs[i] << endl;
        }
    }

    rcv_msg = (int *)malloc(sizeof(int) * 3);
    MPI_Scatter(send_msg, 3, MPI_INT, rcv_msg, 3, MPI_INT, 0, MPI_COMM_WORLD);

    
    // printf("Processor %d: ", rank);
    // printf("%d %d %d\n", rcv_msg[0], rcv_msg[1], rcv_msg[2]);
    
    part_data = (uchar *)malloc(sizeof(uchar) * rcv_msg[1]);
    part_res = (uchar *)malloc(sizeof(uchar) * rcv_msg[1]);

    MPI_Gather(rcv_msg, 1, MPI_INT, send_msg, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int64 t0 = cv::getTickCount();
    MPI_Scatterv(image_data, sendcounts, displs, MPI_UNSIGNED_CHAR, part_data, 
                rcv_msg[1], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);


    for (int i = 0; i < rcv_msg[1] / rcv_msg[0]; ++i) {
        erosion_line(part_data, part_res, i, rcv_msg[1] / rcv_msg[0], rcv_msg[0]);
    }


    if (rank == 0) {
        for (int i = 0; i < size; ++i) {
            sendcounts[i] = sendcounts[i] - ((i == 0 || i == size - 1) ? 1 : 2) * rcv_msg[0] * (EROSION_SIZE / 2);
            displs[i] = (displs[i] == 0) ? displs[i] : (displs[i] + (EROSION_SIZE / 2) * rcv_msg[0]);
        }
    }
    MPI_Gatherv(part_res + ((rcv_msg[2] == 0) ? 0 : (EROSION_SIZE / 2) * rcv_msg[0]), 
                rcv_msg[1] - rcv_msg[0] * ((rcv_msg[2] == 1) ? 2 : 1) * (EROSION_SIZE / 2),
                MPI_UNSIGNED_CHAR, res_data,
                sendcounts, displs,
                MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD
                );
    int64 t1 = cv::getTickCount();
    

    // MPI_Gather(rcv_msg, 1, MPI_INT, send_msg, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // int64 t0 = cv::getTickCount();
    // erosion_image(image, res);
    // int64 t1 = cv::getTickCount();
    // double secs = (t1-t0)/cv::getTickFrequency();
    // cout << "time cost:" << secs << endl;

    if (rank == 0) {
        double secs = (t1-t0)/cv::getTickFrequency();
        cout << "time cost:" << secs << endl;
        // namedWindow("Origin", WINDOW_AUTOSIZE);
        // imshow("Origin", image);
        // namedWindow("Result", WINDOW_AUTOSIZE);
        // imshow("Result", res);
        // waitKey(0);
    }
    MPI_Finalize();
    return 0;
}