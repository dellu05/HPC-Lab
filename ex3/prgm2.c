#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mpi.h>

#define MAX 100
#define TYPE1 1
#define TYPE2 2

int isPali(char str[]) {
    int i = 0, j = strlen(str) - 1;
    while (i < j) {
        if (str[i] != str[j])
            return 0;
        i++;
        j--;
    }
    return 1;
}

int main(void) {
    int my_rank, comm_sz;

    char *s1 = "hello";
    char *s2 = "level";
    char *s3 = "jesus";
    char *s4 = "world";
    char msg[MAX];

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 1) {
        MPI_Send(s1, strlen(s1) + 1, MPI_CHAR, 0, TYPE1, MPI_COMM_WORLD);
    }
    else if (my_rank == 2) {
        MPI_Send(s2, strlen(s2) + 1, MPI_CHAR, 0, TYPE2, MPI_COMM_WORLD);
    }
    else if (my_rank == 3) {
        MPI_Send(s3, strlen(s3) + 1, MPI_CHAR, 0, TYPE1, MPI_COMM_WORLD);
    }
    else if (my_rank == 4) {
        MPI_Send(s4, strlen(s4) + 1, MPI_CHAR, 0, TYPE2, MPI_COMM_WORLD);
    }
    else if (my_rank == 0) {
        MPI_Status s;
        for (int i = 1; i < comm_sz; i++) {

            MPI_Recv(msg, MAX, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);

            if (s.MPI_TAG == TYPE1) {
                for (int k = 0; msg[k]; k++) {
                    msg[k] = toupper(msg[k]);
                }
                printf("From rank %d (Type1): Uppercase = %s\n", s.MPI_SOURCE, msg);
            }
            else if (s.MPI_TAG == TYPE2) {
                if (isPali(msg)) {
                    printf("From rank %d (Type2): %s is a Palindrome\n", s.MPI_SOURCE, msg);
                } else {
                    printf("From rank %d (Type2): %s is not a Palindrome\n", s.MPI_SOURCE, msg);
                }
            }
        }
    }

    MPI_Finalize();
    return 0;
}
