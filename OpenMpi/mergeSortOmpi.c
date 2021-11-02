#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi/mpi.h>
//gcc mergeSortOmpi.c  -o mergeSortOmpi -l mpi
//./mergeSortOmpi

void merge(int *, int *, int, int, int);
void mergeSort(int *, int *, int, int);

int arr[]={21,17,13,30,24,29,36,15};

void merge(int *a, int *b, int l, int m, int r) {
	
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;
	
	while((h <= m) && (j <= r)) {
		
		if(a[h] <= a[j]) {
			
			b[i] = a[h];
			h++;
			
			}
			
		else {
			
			b[i] = a[j];
			j++;
			
			}
			
		i++;
		
		}
		
	if(m < h) {
		
		for(k = j; k <= r; k++) {
			
			b[i] = a[k];
			i++;
			
			}
			
		}
		
	else {
		
		for(k = h; k <= m; k++) {
			
			b[i] = a[k];
			i++;
			
			}
			
		}
		
	for(k = l; k <= r; k++) {
		
		a[k] = b[k];
		
		}
	
		
	}

void mergeSort(int *a, int *b, int l, int r) {
	
	int m;
	
	if(l < r) {
		
		m = (l + r)/2;
		
		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);
		
		}
		
	}

int main(int argc, char** argv) {
	
	int i;
	double time_spent,start, stop;
	int n=sizeof(arr)/sizeof(arr[0]);
		
	printf("A elemek száma:\n%d\n",n);
	
	int world_rank;
	int world_size;
	start = MPI_Wtime();
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
		
	int size = n/world_size;
	
	int *sub_array = malloc(size * sizeof(int));
	MPI_Scatter(arr, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	int *tmp_array = malloc(size * sizeof(int));
	mergeSort(sub_array, tmp_array, 0, (size - 1));
	
	int *sorted = NULL;
	if(world_rank == 0) {
		
		sorted = malloc(n * sizeof(int));
		
		}
	
	MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	int c;
	if(world_rank == 0) {
		
		int *other_array = malloc(n * sizeof(int));
		mergeSort(sorted, other_array, 0, (n - 1));
		
		printf("\nRendezett tömb:\n");
		for(c = 0; c < n; c++) {
			
			printf("%d ", sorted[c]);
			
			}
			
		printf("\n");
			
		free(sorted);
		free(other_array);
			
		}
		
	free(sub_array);
	free(tmp_array);
	
	MPI_Barrier(MPI_COMM_WORLD);
	stop = MPI_Wtime();
	MPI_Finalize();
	
	time_spent=stop-start;
   	printf("\nFutás ideje: %f sec\n", time_spent);
   	
   	return 0;	
}


