#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    
    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    srand(rank);
    int period = (rand() % 5);
    printf("Process %d out of %d is doing for %d seconds.\n", rank, world_size, period);
    sleep(period);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank = 0) {
        printf("All process have synced.\n");
    }
    
    MPI_Finalize();

    return 0;
}