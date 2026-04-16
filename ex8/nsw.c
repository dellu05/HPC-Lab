#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, i, ws,nt;
    printf("\nEnter size of array: ");
    scanf("%d", &n);
    printf("Enter window size: ");
    scanf("%d", &ws);
    printf("Enter number of threads: ");
    scanf("%d",&nt);
    int a[n];
    for (i = 0; i < n; i++) {
        a[i] = rand() % 100;
    }

    printf("\nOriginal Array:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n\n--- Parallel Window Sum ---\n");

    #pragma omp parallel for num_threads(nt)
    for (i = 0; i < n; i = i + ws) {
        int s = 0;
        int k = i;
        int j = 0;
        int window_num = (i / ws) + 1;

        while (j < ws && k < n) {
            s += a[k];
            k++;
            j++;
        }

        printf("Sum of window %d (Thread %d): %d\n", window_num, omp_get_thread_num(), s);
    }

    return 0;
}
