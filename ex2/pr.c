#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

double determinant(int n, double A[n][n]) {
    int i, j, k;
    double det = 1.0;
    int swapCount = 0;

    for (i = 0; i < n; i++) {
        if (A[i][i] == 0) {
            int swapped = 0;
            for (j = i + 1; j < n; j++) {
                if (A[j][i] != 0) {
                    for (k = 0; k < n; k++) {
                        double temp = A[i][k];
                        A[i][k] = A[j][k];
                        A[j][k] = temp;
                    }
                    swapCount++;
                    swapped = 1;
                    break;
                }
            }
            if (!swapped) { det = 0; break; }
        }
        for (j = i + 1; j < n; j++) {
            double factor = A[j][i] / A[i][i];
            for (k = i; k < n; k++)
                A[j][k] -= factor * A[i][k];
        }
    }

    if (det != 0) {
        for (i = 0; i < n; i++) det *= A[i][i];
        if (swapCount % 2 != 0) det = -det;
    }
    return det;
}

int main() {
    int i,j,k;
    int m1, n1, m2, n2;
    int **A, **B;
    printf("Enter rows and columns of Matrix A: ");
    scanf("%d %d", &m1, &n1);
    printf("Enter rows and columns of Matrix B: ");
    scanf("%d %d", &m2, &n2);


    A = (int **)malloc(m1 * sizeof(int *));
    for (i = 0; i < m1; i++)
        A[i] = (int *)malloc(n1 * sizeof(int));


    B = (int **)malloc(m2 * sizeof(int *));
    for (i = 0; i < m2; i++)
        B[i] = (int *)malloc(n2 * sizeof(int));


    srand(getpid());


    for (i = 0; i < m1; i++)
        for (j = 0; j < n1; j++)
            A[i][j] = rand() % 10;


    for (i = 0; i < m2; i++)
        for (j = 0; j < n2; j++)
            B[i][j] = rand() % 10;


    int shmid = shmget(IPC_PRIVATE, 4 * sizeof(double), IPC_CREAT | 0666);
    if (shmid < 0) { perror("shmget failed"); exit(1); }
    double *child_times = (double *)shmat(shmid, NULL, 0);

    pid_t p1, p2, p3, p4;


    p1 = fork();
    if (p1 == 0) {
        struct timeval t1, t2;
        gettimeofday(&t1, NULL);

        if (m1 == m2 && n1 == n2) {
            double add[m1][n1];
            for ( i = 0; i < m1; i++)
                for ( j = 0; j < n1; j++)
                    add[i][j] = A[i][j] + B[i][j];
            printf("\n [Process 1] Matrix Addition done!\n");
     //       printf("\n[Process 1] Addition:\n");
     //       for ( i = 0; i < m1; i++) {
       //         for ( j = 0; j < n1; j++)
         //           printf("%.2lf ", add[i][j]);
           //     printf("\n");
           // }
        } else {
            printf("\n[Process 1] Addition not possible\n");
        }

        gettimeofday(&t2, NULL);
        child_times[0] =  (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
        exit(0);
    }


    p2 = fork();
    if (p2 == 0) {
        struct timeval t1, t2;
        gettimeofday(&t1, NULL);

        if (m1 == m2 && n1 == n2) {
            double sub[m1][n1];
            for ( i = 0; i < m1; i++)
                for ( j = 0; j < n1; j++)
                    sub[i][j] = A[i][j] - B[i][j];
            printf("\n [Process 2] Matrix Subtraction done!\n");
     //       printf("\n[Process 2] Subtraction:\n");
       //     for ( i = 0; i < m1; i++) {
         //       for ( j = 0; j < n1; j++)
           //         printf("%.2lf ", sub[i][j]);
             //   printf("\n");
         //   }
        } else {
            printf("\n[Process 2] Subtraction not possible\n");
        }

        gettimeofday(&t2, NULL);
        child_times[1] = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
        exit(0);
    }

    // ---------------- Process 3: Multiplication ----------------
    p3 = fork();
    if (p3 == 0) {
        struct timeval t1, t2;
        gettimeofday(&t1, NULL);

        if (n1 != m2) {
            printf("\n[Process 3] Multiplication not possible\n");
        } else {
            double mul[m1][n2];
            for ( i = 0; i < m1; i++)
                for ( j = 0; j < n2; j++) {
                    mul[i][j] = 0;
                    for ( k = 0; k < n1; k++)
                        mul[i][j] += A[i][k] * B[k][j];
                }
            printf("\n [Process 3] Matrix Multiplication done!\n");
          //  printf("\n[Process 3] Multiplication:\n");
         //   for ( i = 0; i < m1; i++) {
           //     for ( j = 0; j < n2; j++)
             //       printf("%.2lf ", mul[i][j]);
               // printf("\n");
           // }
        }

        gettimeofday(&t2, NULL);
        child_times[2] = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
        exit(0);
    }


    p4 = fork();
    if (p4 == 0) {
        struct timeval t1, t2;
        gettimeofday(&t1, NULL);

        if (m1 == n1) {
            double copyA[m1][n1];
            for ( i = 0; i < m1; i++)
                for ( j = 0; j < n1; j++)
                    copyA[i][j] = A[i][j];
            double detA = determinant(m1, copyA);
       //     printf("\n[Process 4] Determinant of A = %.2lf", detA);
        } else {
            printf("\n[Process 4] Determinant of A not possible\n");
        }

        if (m2 == n2) {
            double copyB[m2][n2];
            for ( i = 0; i < m2; i++)
                for ( j = 0; j < n2; j++)
                    copyB[i][j] = B[i][j];
            double detB = determinant(m2, copyB);
     //       printf("\n[Process 4] Determinant of B = %.2lf\n", detB);
        } else {
            printf("\n[Process 4] Determinant of B not possible\n");
        }
        printf("\n[Process 4] Determinant calculation for A and B is done!\n");
        gettimeofday(&t2, NULL);
        child_times[3] = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec) / 1000000.0;
        exit(0);
    }


    for ( i = 0; i < 4; i++)
        wait(NULL);

    double max_time = child_times[0];
    for ( i = 1; i < 4; i++)
        if (child_times[i] > max_time) max_time = child_times[i];

    printf("\nChild execution times (seconds):\n");
    for ( i = 0; i < 4; i++)
        printf("Process %d: %.6lf s\n", i+1, child_times[i]);

    printf("\nParallel execution time (maximum among processes): %.6lf s\n", max_time);


    shmdt(child_times);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
