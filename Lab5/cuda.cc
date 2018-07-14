#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
// #include "cuda.cu"
// #define EROSION_SIZE 5
// const int EROSION_MATRIX[EROSION_SIZE * EROSION_SIZE] = {
//     0, 0, 1, 0, 0,
//     0, 0, 1, 0, 0,
//     1, 1, 1, 1, 1,
//     0, 0, 1, 0, 0,
//     0, 0, 1, 0, 0
// };
// #define EMPTY 0
// #define FULL 255
// #define THREAD_CNT 20


// void erosion_line(uchar* image, uchar* res, int *matrix, int pos, int rows, int cols) {
//     uchar* aim = res + (pos * cols);
//     if (pos < EROSION_SIZE / 2 || pos >= rows - EROSION_SIZE / 2) {
//         for (int l = 0; l < cols; ++l)
//             aim[l] = FULL;
//         return;
//     }
//     uchar* cache[EROSION_SIZE];
//     for (int i = 0; i < EROSION_SIZE; ++i) {
//         cache[i] = image + (i + pos - EROSION_SIZE / 2) * cols;
//     }
//     int l;
//     for (l = 0; l < EROSION_SIZE / 2 && l < cols; ++l) {
//         aim[l] = FULL;
//     }
//     for (; l < cols - EROSION_SIZE / 2; ++l) {
//         bool result = true;
//         for (int i = 0; i < EROSION_SIZE && result; ++i) {
//             for (int j = 0; j < EROSION_SIZE; ++j) {
//                 if (matrix[i * EROSION_SIZE + j] && !cache[i][j + l - EROSION_SIZE / 2]) {
//                     result = false;
//                     break;
//                 }
//             }
//         }
//         if (result) aim[l] = FULL;
//         else aim[l] = EMPTY;
//     }
//     for (; l < cols; ++l)
//         aim[l] = FULL;
// }

// __global__ void erosion_kernel(uchar *image, uchar *res, int *matrix, int rows, int cols) {
//     int i = threadIdx.x;
//     int begin = i * rows / THREAD_CNT;
//     int end = min((i + 1) * rows / THREAD_CNT, rows);
//     for (int k = begin; k < end; ++k) {
//         erosion_line(image, res, matrix, k, rows, cols);
//     }
// }

// void erosion_image(uchar *image_data, uchar *res_data) {
//     int rows = image.rows;
//     int cols = image.cols;
//     int size = rows * cols;

//     int *dev_matrix = nullptr;
//     uchar *dev_image_data = nullptr;
//     uchar *dev_res_data = nullptr;

//     cudaSetDevice(0);
//     cudaMalloc((void**)&dev_image_data, size * sizeof(uchar));
//     cudaMalloc((void**)&dev_res_data, size * sizeof(uchar));
//     cudaMalloc((void**)&dev_matrix, EROSION_SIZE * EROSION_SIZE * sizeof(int));

//     cudaMemcpy(dev_image_data, image_data, size * sizeof(uchar), cudaMemcpyHostToDevice);
//     cudaMemcpy(dev_res_data, res_data, size * sizeof(uchar), cudaMemcpyHostToDevice);
//     cudaMemcpy(dev_matrix, EROSION_MATRIX, EROSION_SIZE * EROSION_SIZE * sizeof(uchar), cudaMemcpyHostToDevice);

//     erosion_kernel<<<1, THREAD_CNT>>>(dev_image_data, dev_res_data, dev_matrix, rows, cols);

//     cudaGetLastError();
//     cudaDeviceSynchronize();

//     cudaMemcpy(res_data, dev_res_data, size * sizeof(uchar), cudaMemcpyDeviceToHost);

//     cudaFree(dev_image_data);
//     cudaFree(dev_res_data);
//     cudaFree(dev_matrix);
// }
// void addWithCuda(int *c, const int *a, const int *b, unsigned int size);

// __global__ void addKernel(int *c, const int *a, const int *b)
// {
//     int i = threadIdx.x;
//     c[i] = a[i] + b[i];
// }

int main()
{
    // const int SIZE = 5;
    // const int a[SIZE] = { 1, 2, 3, 4, 5 };
    // const int b[SIZE] = { 10, 20, 30, 40, 50 };
    // int c[SIZE] = { 0 };

    // addWithCuda(c, a, b, SIZE);

    // printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
    //     c[0], c[1], c[2], c[3], c[4]);
    if (argc != 2) return -1;
    Mat image = imread(argv[1], IMREAD_GRAYSCALE);      // read image
    threshold(image, image, 105, 255, CV_THRESH_BINARY_INV);  // turn image to binary
    Mat res = image.clone();
    uchar *image_data = image.ptr<uchar>(0);
    uchar *res_data = res.ptr<uchar>(0);
    int64 t0 = cv::getTickCount();
    erosion_image(image_data, res_data);
    int64 t1 = cv::getTickCount();
    double secs = (t1-t0)/cv::getTickFrequency();
    cout << "time cost:" << secs << endl;

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaDeviceReset();

    return 0;
}
