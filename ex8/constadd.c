#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, i, constant, nt;

    printf("Enter size of array: ");
    scanf("%d", &n);

    printf("Enter constant value to add: ");
    scanf("%d", &constant);
    printf("Enter number of threads: ");
    scanf("%d",&nt);
    int A[n];

    for(i = 0; i < n; i++) {
        A[i] = rand() % 100;
    }

    printf("\nArray BEFORE addition:\n");
    for(i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
    #pragma omp parallel for num_threads(nt)
    for(i = 0; i < n; i++) {
        A[i] = A[i] + constant;
    }
    printf("\nArray AFTER addition:\n");
    for(i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
    return 0;
}
