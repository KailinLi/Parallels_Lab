#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int size, rank;
    int *globaldata;
    int *localdata;
    int *result;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        globaldata = (int *)malloc(sizeof(int) * size * 2);
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < size; ++j)
                globaldata[i * size + j] = i * size + j;
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < size; ++j)
                printf("%d ", globaldata[i * size + j]);
        printf("\n");

        result = (int *)malloc(sizeof(int) * size);
    }
    
    localdata = (int *)malloc(sizeof(int) * 2);

    MPI_Scatter(globaldata, 2, MPI_INT, localdata, 2, MPI_INT, 0, MPI_COMM_WORLD);

    printf("**Processor %d has data:\n", rank);
    for (int i = 0; i < 2; ++i)
        printf("%d ", localdata[i]);
    printf("\n");
    fflush(stdin);
    localdata[0] += localdata[1];

    MPI_Gather(localdata, 1, MPI_INT, result, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Sum %d has data: ", rank);
        for (int i = 0; i < size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        fflush(stdin);
    }

    MPI_Finalize();
    return 0;
}