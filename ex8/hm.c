#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){
   int n,i,j,nt;
   printf("\nEnter size of the square matrix:");
   scanf("%d",&n);
   int o[n][n], h[n][n];
   printf("Enter number of threads: ");
   scanf("%d",&nt);
   for(i=0;i<n;i++){
      for(j=0;j<n;j++){
         o[i][j]=rand()%100;
      }
   }

   printf("\nOriginal matrix\n");
   for(i=0;i<n;i++){
      for(j=0;j<n;j++){
         printf("%d ",o[i][j]);
      }
      printf("\n");
   }
   #pragma omp parallel for num_threads(nt)
   for(i=0;i<n;i++){
      int k=0;
      for(j=n-1;j>=0;j--){
         h[i][k++]=o[i][j];
      }
   }
   printf("\nHorizontal mirror matrix\n");
   for(i=0;i<n;i++){
      for(j=0;j<n;j++){
         printf("%d ",h[i][j]);
      }
      printf("\n");
   }
   return 0;
}
