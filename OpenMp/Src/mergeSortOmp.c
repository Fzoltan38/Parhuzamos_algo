#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
//gcc -fopenmp mergeSortOmp.c -o mergeSortOmp
//./mergeSortOmp

int arr[]={21,17,13,30,24,29,36,15};

void merge(int * array, int n, int * tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;
	
   while (i<n/2 && j<n) {
      if (array[i] < array[j]) {
         tmp[ti] = array[i];
         ti++; i++;
      } else {
         tmp[ti] = array[j];
         ti++; 
	 j++;
      }
   }
   while (i<n/2) { 
      tmp[ti] = array[i];
	ti++;
	i++;
   }
      while (j<n) { 
         tmp[ti] = array[j];
         ti++; 
	 j++;
   }
	
   memcpy(array, tmp, n*sizeof(int));

} 

void mergeSort(int * array, int n, int * tmp)
{
   if (n < 2) return;

   #pragma omp task firstprivate (array, n, tmp)
   mergeSort(array, n/2, tmp);
	printf("%p-->%d-->%p\n",array,n/2,tmp);
   #pragma omp task firstprivate (array, n, tmp)
   mergeSort(array+(n/2), n-(n/2), tmp);
	printf("%d->%d->%d\n",*array+(n/2),n-(n/2),*tmp);
   #pragma omp taskwait

   merge(array, n, tmp);
}


int main()
{
   int n=sizeof(arr)/sizeof(arr[0]);
   double time_spent,start, stop;
   
   int tmp[n];

   printf("A elemek száma:\n%d\n",n);
  
   start = omp_get_wtime();
   #pragma omp parallel
   {
      #pragma omp single
      mergeSort(arr, n, tmp);
   }
   stop = omp_get_wtime();
   
   printf("Rendezett tömb:\n");
   
    for (int i = 0; i < n; i++) {
      printf("%d ",arr[i]);
   }
   
   time_spent=stop-start;
   printf("\n\nFutás ideje: %f sec\n", time_spent);
   
   return 0;
}

