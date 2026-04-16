#include <stdio.h>
#include <omp.h>

int main() {
    int s, i, nt;
    printf("Enter the size of the array: ");
    scanf("%d", &s);
    int arr[s];
    printf("Enter array elements: ");
    for (i=0;i<s;i++){
       scanf("%d",&arr[i]);
    }
    printf("Enter number of threads: ");
    scanf("%d",&nt);
    int maxi=arr[0];
    #pragma omp parallel for reduction(max:maxi) num_threads(nt)
    for (i = 1; i < s; i++) {
        if(maxi<arr[i]){
           maxi=arr[i];
        }
    }
    printf("Maximum element is %d\n\n", maxi);
    return 0;
}
