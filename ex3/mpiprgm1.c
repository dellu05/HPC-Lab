#include <stdio.h>
#include <string.h>
#include <mpi.h>
#define MAX 100

int main(void){
        int my_rank, comm_sz;
        char msg[MAX];
        MPI_Init(NULL,NULL);
        MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        if(my_rank!=0){
                sprintf(msg,"Hello from process %d\n", my_rank);
                MPI_Send(msg,strlen(msg)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
        }
        else{
                printf("Hello from process %d\n", my_rank);
                for (int i=1;i<comm_sz;i++){
                        MPI_Recv(msg,MAX,MPI_CHAR,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                        printf("%s",msg);
                }
        }
        MPI_Finalize();
        return 0;
}
