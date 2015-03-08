#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include <mpi.h>
#include <limits.h>
#include <omp.h>

#define DEBUG 1

typedef struct {
  int key;
  int index;
  bool found;
} resultStruct;

void print_array(int * ptr, int num_elements) {
  for(int i = 0; i < num_elements; i++) {
      printf("%d ",ptr[i]);
  }
}

int int_comparator(const void * p1,const void *p2) {

  int int1 = *((const int *)p1);
  int int2 = *((const int *)p2);
  
  
 if (int1 < int2) {
   return -1;
 } else if (int1 > int2) {
   return 1;
 } else {
  return 0; 
 }
 return 0;
}

void par_bsearch (
	    const void* keys, 
	    const void* base,
            size_t num, 
	    size_t size,
	    int (*compar)(const void*,const void*),
	    int num_keys,
	    MPI_Comm mpi_comm,
	    int num_threads,
	    resultStruct * result_buff
	    )
{
   
    int threadID,startIndex,numToProcess;
    int per_thread = num / num_threads;
    int remainder  = num_threads % per_thread;
    int resultSlot;
   
    #pragma omp parallel private(threadID,startIndex,numToProcess,resultSlot)
    {
      resultSlot = 0;
      threadID = omp_get_thread_num();
      if (threadID == 0) {
	numToProcess = per_thread + remainder;
	startIndex   = 0;
      } else {
	numToProcess = per_thread;
	startIndex = (per_thread * threadID) + remainder;
      }
      
      for (int curr_key_index = 0; curr_key_index < num_keys; curr_key_index++) {
	void * searchResult = bsearch(
				    &(((int *)keys)[curr_key_index]),
				    ((int *)base+startIndex),
				    numToProcess,
				    size,
				    compar
				  );
	
	if (searchResult != 0) {
	  int index = (int)((int *)searchResult - (int *)base);
	  int resultIndex = (threadID * num_threads) + resultSlot;
	  result_buff[resultIndex].found = true;
	  result_buff[resultIndex].key   = ((int *)keys)[curr_key_index];
	  result_buff[resultIndex].index = index;
	  resultSlot++;
	}
	
      }
      
    }
  
}


void search(int num_elements) {



  
}
  
int main(int argc, char * argv[]) {
  
   int array_size;
   int nodes;
   int sockets_per_node;
   int threads_per_socket;
   int num_keys;
   int max_val;
   
   if (argc < 7) {
     printf("Usage: ./bsearch array_size num_keys nodes sockets_per_node threads_per_socket\n");
     return -1;
   } else {
      
      sscanf(argv[1],"%d",&array_size);
      sscanf(argv[2],"%d",&num_keys);
      sscanf(argv[3],"%d",&max_val);
      sscanf(argv[4],"%d",&nodes);
      sscanf(argv[5],"%d",&sockets_per_node);
      sscanf(argv[6],"%d",&threads_per_socket);
      /*
      printf("Elements: %d\n",array_size);
      printf("Nodes: %d\n",nodes);
      printf("Sock/Node: %d\n",sockets_per_node);
      printf("Thread/Sock: %d\n",threads_per_socket);
      */
   }
  
    MPI_Init(&argc,&argv);
    
    int rank;
    int size;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int elems_per_task = array_size / size;
    int remainder = array_size % elems_per_task;

    // P0 is the master process
    int * int_array = 0;
    int * keys_array = 0;
    int startIndex = 0;
    int endIndex = 0;
    MPI_Status status_handle;
    
    if (rank == 0) {
      srand(time(NULL));
      
      int_array  = new int[array_size];
      keys_array = new int[num_keys]; 
      
      // Place keeper
      for (int i = 0; i < num_keys; i++) {
	keys_array[i] = (rand() % max_val) + 1;
      }
      
      // initialize to random integers
      for (int i = 0; i < array_size; i++) {
	int_array[i] = (rand() % max_val) + 1;
      }
      // Sort the array
      //printf("Raw array: \n");
      //print_array(int_array,array_size);
      //printf("\n");
      qsort((void *) int_array,array_size,sizeof(int),int_comparator);
      //printf("Sorted array: \n");
      //print_array(int_array,array_size);
      //printf("\n");
      
      startIndex = 0;
      endIndex   = startIndex + (elems_per_task) + remainder;
      int * send_buff = int_array + endIndex;
      
      // Send out parts of the sorted array
      for (int curr_rank = 1; curr_rank < size; curr_rank++) {
	MPI_Ssend(send_buff,elems_per_task,MPI_INT,curr_rank,0,MPI_COMM_WORLD);
	send_buff += elems_per_task;
      }
      
      // Send out the keys we are looking for
      for (int curr_rank = 1; curr_rank < size; curr_rank++) {
	MPI_Ssend(keys_array,num_keys,MPI_INT,curr_rank,1,MPI_COMM_WORLD);
      }     
      
    } else {
      
      int_array = new int[elems_per_task];
      keys_array = new int[num_keys];
      
      MPI_Recv((void* )int_array, elems_per_task, MPI_INT, 0, 0, MPI_COMM_WORLD,&status_handle);
      
      startIndex = remainder + elems_per_task*rank;
      endIndex = startIndex + elems_per_task;
      
      MPI_Recv((void *)keys_array, num_keys, MPI_INT, 0, 1, MPI_COMM_WORLD, &status_handle);
      
    }
    
    // At this point the array is distributed between MPI Tasks and all know which keys 
    // they are looking for.
    
    // Some openmp book keeping
    omp_set_num_threads(threads_per_socket);
    
    resultStruct * results = new resultStruct[threads_per_socket * num_keys]; 
    
    for (int i = 0; i < (threads_per_socket * num_keys); i++) {
      results[i].found = false;
    }
    
    // Do the binary search
    par_bsearch(
	    keys_array, 
	    int_array,
            (rank == 0 ? elems_per_task + remainder: elems_per_task), 
	    sizeof(int),
	    int_comparator,
	    num_keys,
	    MPI_COMM_WORLD,
	    threads_per_socket,
	    results
	    );

    // Results are done by here
    // print out the results
    for (int i = 0; i < (threads_per_socket * num_keys); i++) {
      resultStruct curr_res = results[i];
      if (curr_res.found) {
	// a result was found
	printf("Key %d was found on rank %d at index %d\n",curr_res.key,rank,startIndex+curr_res.index);
	
      }
      
    }
    
    
    /*
    printf("Array from Rank %d of size %d: ",rank,size);
    for (int i = 0; i < elems_per_task; i++) {
      printf("%d ",int_array[i]);
    }
    printf("\n");
    */

    MPI_Finalize();
    
    return 0;
  
}