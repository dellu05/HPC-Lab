#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX 10

int A[MAX][MAX], B[MAX][MAX];
int add[MAX][MAX], mul[MAX][MAX];
int r1, c1, r2, c2;

void *matrix_add(void *arg)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int i, j;

    for(i = 0; i < r1; i++)
    {
        for(j = 0; j < c1; j++)
        {
            add[i][j] = A[i][j] + B[i][j];
        }
    }

    gettimeofday(&end, NULL);

    double time_taken =
        (end.tv_sec - start.tv_sec) +
        (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("\nAddition completed by thread.\n");
    printf("Addition Thread Execution Time: %f seconds\n", time_taken);

    pthread_exit(NULL);
}

void *matrix_mul(void *arg)
{
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int i, j, k;

    for(i = 0; i < r1; i++)
    {
        for(j = 0; j < c2; j++)
        {
            mul[i][j] = 0;
            for(k = 0; k < c1; k++)
            {
                mul[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    gettimeofday(&end, NULL);

    double time_taken =
        (end.tv_sec - start.tv_sec) +
        (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("\nMultiplication completed by thread.\n");
    printf("Multiplication Thread Execution Time: %f seconds\n", time_taken);

    pthread_exit(NULL);
}

int main()
{
    pthread_t t1, t2;
    int i, j;

    printf("Enter rows and columns of Matrix A: ");
    scanf("%d %d", &r1, &c1);

    printf("Enter rows and columns of Matrix B: ");
    scanf("%d %d", &r2, &c2);

    if(c1 != r2)
    {
        printf("Matrix multiplication not possible!\n");
        return 0;
    }

    printf("\nEnter elements of Matrix A:\n");
    for(i = 0; i < r1; i++)
        for(j = 0; j < c1; j++)
            scanf("%d", &A[i][j]);

    printf("\nEnter elements of Matrix B:\n");
    for(i = 0; i < r2; i++)
        for(j = 0; j < c2; j++)
            scanf("%d", &B[i][j]);

    pthread_create(&t1, NULL, matrix_add, NULL);
    pthread_create(&t2, NULL, matrix_mul, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nMatrix Addition Result:\n");
    for(i = 0; i < r1; i++)
    {
        for(j = 0; j < c1; j++)
            printf("%d ", add[i][j]);
        printf("\n");
    }

    printf("\nMatrix Multiplication Result:\n");
    for(i = 0; i < r1; i++)
    {
        for(j = 0; j < c2; j++)
            printf("%d ", mul[i][j]);
        printf("\n");
    }

    return 0;
}
