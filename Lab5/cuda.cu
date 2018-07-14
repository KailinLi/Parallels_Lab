#include "cuda_runtime.h"
#include "device_launch_parameters.h"

typedef unsigned char uchar;

#define EROSION_SIZE 5
const int EROSION_MATRIX[EROSION_SIZE * EROSION_SIZE] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    1, 1, 1, 1, 1,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0
};
#define EMPTY 0
#define FULL 255
#define THREAD_CNT 20


__global__ void erosion_line(uchar* image, uchar* res, int *matrix, int pos, int rows, int cols) {
    uchar* aim = res + (pos * cols);
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
                if (matrix[i * EROSION_SIZE + j] && !cache[i][j + l - EROSION_SIZE / 2]) {
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

__global__ void erosion_kernel(uchar *image, uchar *res, int *matrix, int rows, int cols) {
    int i = threadIdx.x;
    int begin = i * rows / THREAD_CNT;
    int end = min((i + 1) * rows / THREAD_CNT, rows);
    for (int pos = begin; pos < end; ++pos) {
        erosion_line(image, res, matrix, k, rows, cols);
    }
}

void erosion_image(uchar *image_data, uchar *res_data, int rows, int cols) {
    int size = rows * cols;

    int *dev_matrix = 0;
    uchar *dev_image_data = 0;
    uchar *dev_res_data = 0;

    cudaSetDevice(0);
    cudaMalloc((void**)&dev_image_data, size * sizeof(uchar));
    cudaMalloc((void**)&dev_res_data, size * sizeof(uchar));
    cudaMalloc((void**)&dev_matrix, EROSION_SIZE * EROSION_SIZE * sizeof(int));

    cudaMemcpy(dev_image_data, image_data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_res_data, res_data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_matrix, EROSION_MATRIX, EROSION_SIZE * EROSION_SIZE * sizeof(uchar), cudaMemcpyHostToDevice);

    erosion_kernel<<<1, THREAD_CNT>>>(dev_image_data, dev_res_data, dev_matrix, rows, cols);

    cudaGetLastError();
    cudaDeviceSynchronize();

    cudaMemcpy(res_data, dev_res_data, size * sizeof(uchar), cudaMemcpyDeviceToHost);

    cudaFree(dev_image_data);
    cudaFree(dev_res_data);
    cudaFree(dev_matrix);
}