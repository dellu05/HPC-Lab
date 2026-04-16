#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, i, nt;

    printf("Enter size of arrays: ");
    scanf("%d", &n);
    printf("Enter number of threads: ");
    scanf("%d",&nt);
    int A[n], B[n], C[n];
    for(i = 0; i < n; i++) {
        A[i] = rand() % 100;
        B[i] = rand() % 100;
    }

    #pragma omp parallel for num_threads(nt)
    for(i = 0; i < n; i++) {
        C[i] = A[i] + B[i];
    }

    printf("\nArray A:\n");
    for(i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\nArray B:\n");
    for(i = 0; i < n; i++) {
        printf("%d ", B[i]);
    }
    printf("\nResult Array C (A+B):\n");
    for(i = 0; i < n; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");
   return 0;
}
