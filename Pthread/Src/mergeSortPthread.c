#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <sys/time.h>
//gcc mergeSortPthread.c -omergeSortPthread -lpthread
//./mergeSortPthread

typedef struct Data{
    int low;
    int high;
}Data;

int arr[]={21,17,13,30,24,29,36,15};

void merge(int low, int mid, int high)
{
    int i, j, k;
   
    int n1 = mid - low + 1;
    int n2 =  high - mid;
    
    int left[n1], right[n2];
   
    for (i = 0; i < n1; i++)
        left[i] = arr[low + i];
 
    
    for (j = 0; j < n2; j++)
        right[j] = arr[mid+ 1+ j];
      
    
    i = 0;
    j = 0;
    k = low;
    
    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
        {
            arr[k] = left[i];
            i++;
        }
        else
        {
            arr[k] = right[j];
            j++;
        }
        k++;
    }
    
    while (i < n1)
    {
        arr[k] = left[i];
        i++;
        k++;
    }
    
    while (j < n2)
    {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void* mergeSort(void* arg)
{
    Data data = *((Data*)arg);
    int low = data.low;
    int high = data.high;
    
    if (low < high)
    {
     	
        int mid = low+(high-low)/2;        
        
        pthread_t thread1,thread2;
        
        Data d1 = {low, mid};
        Data d2 = {mid+1, high};
        
        pthread_create(&thread1, NULL, mergeSort, &d1);
        pthread_create(&thread2, NULL, mergeSort, &d2);
     	
        pthread_join(thread1, NULL);   
        pthread_join(thread2, NULL);
    
        merge(low, mid, high);
    }
}

int main()
{  
    int n=sizeof(arr)/sizeof(arr[0]);
    
    struct timeval  start, stop;
    double time_spent;
    
    Data d = {0, n-1};
    printf("A elemek száma:\n%d\n",n);
    
    //pthread_t threads[NUM_THREADS];
    pthread_t t;
    
    gettimeofday(&start, NULL);
    
    pthread_create(&t, NULL, mergeSort, &d);
    pthread_join(t, NULL);
    printf("\nRendezett tömb:\n");
    
    gettimeofday(&stop, NULL);
    
    for (int i=0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
    
    time_spent = ((double) ((double) (stop.tv_usec - start.tv_usec) / 1000000 + (double) (stop.tv_sec - start.tv_sec)));
    printf("\nFutás ideje: %f sec\n", time_spent);
    
    pthread_exit(NULL);
}
