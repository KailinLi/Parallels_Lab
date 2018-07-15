#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
const int SIZE = 5;

void addWithCuda(int *c, const int *a, const int *b, unsigned int size);
__global__ void addKernel(int *c, const int *a, const int *b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}
void init_vector(int *v) {
    for (int i = 0; i < N; ++i)
        v[i] = i;
}
int main() {
    const int a[SIZE];
    const int b[SIZE];
    int c[SIZE];
    init_vector(a);
    init_vector(b);
    addWithCuda(c, a, b, SIZE);
    printf("{");
    for (int i = 0; i < SIZE; ++i)
        printf("%d, ", a[i]);
    printf("} + ");
    printf("{");
    for (int i = 0; i < SIZE; ++i)
        printf("%d, ", b[i]);
    printf("} =\n");
    printf("{");
    for (int i = 0; i < SIZE; ++i)
        printf("%d, ", c[i]);
    printf("}\n");

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaDeviceReset();

    return 0;
}

// Helper function for using CUDA to add vectors in parallel.
void addWithCuda(int *c, const int *a, const int *b, unsigned int size)
{
    int *dev_a = 0;
    int *dev_b = 0;
    int *dev_c = 0;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaSetDevice(0);

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaMalloc((void**)&dev_c, size * sizeof(int));

    cudaMalloc((void**)&dev_a, size * sizeof(int));

    cudaMalloc((void**)&dev_b, size * sizeof(int));

    // Copy input vectors from host memory to GPU buffers.
    cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);

    // Launch a kernel on the GPU with one thread for each element.
    addKernel<<<1, size>>>(dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);
    
    return;
}
