#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    
    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0) {
        printf("Greeting from the root process.\n");
    } else {
        printf("Hello from the %d process out of the %d\n", rank, world_size);
    }

    MPI_Finalize();

    return 0;
}