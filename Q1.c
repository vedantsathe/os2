#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 1000

int main(int argc, char **argv) {
    int rank, size;
    int array[ARRAY_SIZE];
    int local_min, global_min;
    int i;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Generate random numbers on rank 0
    if (rank == 0) {
        for (i = 0; i < ARRAY_SIZE; i++) {
            array[i] = rand();
        }
    }

    // Scatter array to all processes
    MPI_Scatter(array, ARRAY_SIZE/size, MPI_INT,
                array + (ARRAY_SIZE/size)*rank, ARRAY_SIZE/size, MPI_INT,
                0, MPI_COMM_WORLD);

    // Find local minimum
    local_min = array[0];
    for (i = 1; i < ARRAY_SIZE/size; i++) {
        if (array[i] < local_min) {
            local_min = array[i];
        }
    }

    // Find global minimum
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    // Print global minimum on rank 0
    if (rank == 0) {
        printf("The minimum value is %d\n", global_min);
    }

    // Finalize MPI environment
    MPI_Finalize();
    return 0;
}