#include <stdio.h>
#include <pthread.h>
#define N 10
int vector_a[N];
int vector_b[N];
int vector_result[N];
void * add(void * index) {
    vector_result[(int)index] = vector_a[(int)index] + vector_b[(int)index];
    return (void *)0;
}
void init_vector(int *v) {
    for (int i = 0; i < N; ++i)
        v[i] = i;
}
int main() {
    int err;
    init_vector(vector_a);
    init_vector(vector_b);
    pthread_t tid[N];
    for (int i = 0; i < N; ++i) {
        err = pthread_create(tid + i, NULL, add, (void *)i);
    }
    for (int i = 0; i < N; ++i) {
        void *tret;
        err = pthread_join(*(tid + i), &tret);
    }
    for (int i = 0; i < N; ++i)
        printf("%d ", vector_result[i]);
    printf("\n");
}