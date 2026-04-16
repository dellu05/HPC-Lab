#include <stdio.h>
#include <omp.h>

int main() {
    int s, i, nt, t, c=0;
    printf("Enter the size of the array: ");
    scanf("%d", &s);
    int arr[s];
    printf("Enter array elements: ");
    for (i=0;i<s;i++){
       scanf("%d",&arr[i]);
    }
    printf("Enter the target: ");
    scanf("%d", &t);
    printf("Enter number of threads: ");
    scanf("%d",&nt);
    #pragma omp parallel for reduction(+:c) num_threads(nt)
    for (i = 0; i < s; i++) {
        if(arr[i]<t){
           c+=1;
        }
    }
    printf("Total number of values less than %d is %d\n\n", t, c);
    return 0;
}
