// g++ omp.cc -fopenmp `pkg-config --cflags --libs opencv`
// export OMP_NUM_THREADS=20
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
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

void erosion_line(Mat &image, Mat &res, int pos, int rows, int cols) {
    uchar* aim = res.ptr<uchar>(pos);
    if (pos < EROSION_SIZE / 2 && pos >= rows - EROSION_SIZE / 2) {
        for (int l = 0; l < cols; ++l)
            aim[l] = FULL;
        return;
    }
    uchar* cache[EROSION_SIZE];
    for (int i = 0; i < EROSION_SIZE; ++i) {
        cache[i] = image.ptr<uchar>(i + pos - EROSION_SIZE / 2);
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
void erosion_part(Mat *image, Mat *res, int begin, int end, int rows, int cols) {
    for (int i = begin; i < end; ++i) {
        erosion_line(*image, *res, i, rows, cols);
    }
}
void erosion_image(Mat &image, Mat &res) {
    int rows = image.rows;
    int cols = image.cols;
    #pragma omp parallel for
    for (int i = 0; i < OMP_CNT; ++i) {
        erosion_part(&image, &res, i * rows / OMP_CNT, 
                        min((i + 1) * rows / OMP_CNT, rows), rows, cols);
    }
}
int main (int argc, char *argv[]) {
    if (argc != 2) return -1;

    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Mat image, res;
    int *send_msg;
    int *rcv_msg;
    uchar *image_data;
    uchar *res_data;
    if (rank == 0) {
        image = imread(argv[1], IMREAD_GRAYSCALE);      // read image
        threshold(image, image, 105, 255, CV_THRESH_BINARY_INV);  // turn image to binary
        res = image.clone();
        image_data = image.ptr<uchar>(0);
        int data_length = image.cols * image.rows;
        res_data = (uchar *)malloc(sizeof(uchar) * data_length);

        send_msg = (int *)malloc(sizeof(int) * size * 2);
        for (int i = 0; i < size / 2; ++i) {
            send_msg[i * 2] = image.cols;
            send_msg[i * 2 + 1] = 
        }
    }


    int64 t0 = cv::getTickCount();
    erosion_image(image, res);
    int64 t1 = cv::getTickCount();
    double secs = (t1-t0)/cv::getTickFrequency();
    cout << "time cost:" << secs << endl;

    // namedWindow("Origin", WINDOW_AUTOSIZE);
    // imshow("Origin", image);
    // namedWindow("Result", WINDOW_AUTOSIZE);
    // imshow("Result", res);
    // waitKey(0);
    return 0;
}