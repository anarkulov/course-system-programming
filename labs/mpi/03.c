#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    
    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int token = rank == 0 ? 1 : 0;
    
    if (rank != 0) {
        int prev_neighbour = rank - 1;
        MPI_Recv(&token, 1, MPI_INT, prev_neighbour, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("Process %d out of %d got the token %d from %d\n", rank, world_size, token, prev_neighbour);
    }

    int next_neighbour = (rank + 1) % world_size;
    printf("Process %d out of %d is sending the token %d to %d\n", rank, world_size, token, next_neighbour);
    MPI_Send(&token, 1, MPI_INT, next_neighbour, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int last_process = world_size - 1;
        MPI_Recv(&token, 1, MPI_INT, last_process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process %d out of %d got the token %d from %d\n" \
                "All process have synced.\n"
                rank, world_size, token, last_process);
    }

    MPI_Finalize();

    return 0;
}