#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    
    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if(rank == 0) {
        printf(
            "Greeting from the root process.\n" \
            "Waiting for the messages from the worker procces.\n"
        );
        
        #define MAX_MSG_SIZE 1024
        static char message[MAX_MSG_SIZE];
        for (int i = 1; i < world_size; i++){
            MPI_Recv(message, MAX_MSG_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            printf("Got msg %s from the %d process.\n", message, i);
        }
    } else {
        printf(
            "Hello from the %d process out of the %d\n" \
            "Sending a greeting to the root process.\n",
            rank, 
            world_size
        );

        static const char message[] = "hello";
        MPI_Send(message, sizeof(message), MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();

    return 0;
}