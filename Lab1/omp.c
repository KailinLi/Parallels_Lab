#include <stdio.h>
#include <pthread.h>
#define N 10
int vector_a[N];
int vector_b[N];
int vector_result[N];
void init_vector(int *v) {
    for (int i = 0; i < N; ++i)
        v[i] = i;
}
int main() {
    int err;
    init_vector(vector_a);
    init_vector(vector_b);
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        printf("a[%d] + b[%d] = ", i, i);
        vector_result[i] = vector_a[i] + vector_b[i];
        printf("%d\n", vector_result[i]);
    }
    for (int i = 0; i < N; ++i)
        printf("%d ", vector_result[i]);
    printf("\n");
}