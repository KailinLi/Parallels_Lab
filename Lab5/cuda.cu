#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>
#include <stdio.h>
#include <ctime>
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
#define THREAD_CNT 1024

__global__ void erosion_kernel(uchar *image, uchar *res, int *matrix, int rows, int cols) {
    int i = threadIdx.x;
    int begin = i * rows / THREAD_CNT;
    int end = min((i + 1) * rows / THREAD_CNT, rows);
    for (int pos = begin; pos < end; ++pos) {
        uchar* aim = res + (pos * cols);
        if (pos < EROSION_SIZE / 2 || pos >= rows - EROSION_SIZE / 2) {
            for (int l = 0; l < cols; ++l)
                aim[l] = FULL;
            continue;
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
}

void erosion_image(uchar *image_data, uchar *res_data, int rows, int cols) {
    int size = rows * cols;

    int *dev_matrix = 0;
    uchar *dev_image_data = 0;
    uchar *dev_res_data = 0;
    clock_t begin;
    clock_t end;
    double timeCost;
    cudaError_t cudaStatus;

    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!");
        goto Error;
    }
    cudaStatus = cudaMalloc((void**)&dev_image_data, size * sizeof(uchar));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }
    cudaStatus = cudaMalloc((void**)&dev_res_data, size * sizeof(uchar));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }
    cudaStatus = cudaMalloc((void**)&dev_matrix, EROSION_SIZE * EROSION_SIZE * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_image_data, image_data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }
    // cudaStatus = cudaMemcpy(dev_res_data, res_data, size * sizeof(uchar), cudaMemcpyHostToDevice);
    // if (cudaStatus != cudaSuccess) {
    //     fprintf(stderr, "cudaMalloc failed!");
    //     goto Error;
    // }

    cudaStatus = cudaMemcpy(dev_matrix, EROSION_MATRIX, EROSION_SIZE * EROSION_SIZE * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }
    begin = clock();
    erosion_kernel<<<1, THREAD_CNT>>>(dev_image_data, dev_res_data, dev_matrix, rows, cols);

    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }
    end = clock();
    timeCost = (double)(end - begin)/CLOCKS_PER_SEC;
    fprintf(stderr, "time: %lf\n", timeCost);

    cudaStatus = cudaMemcpy(res_data, dev_res_data, size * sizeof(uchar), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

Error:
    cudaFree(dev_image_data);
    cudaFree(dev_res_data);
    cudaFree(dev_matrix);

    cudaDeviceReset();
}