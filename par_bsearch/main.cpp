#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include <mpi.h>
#include <limits.h>

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

void* par_bsearch (
	    const void* key, 
	    const void* base,
            size_t num, 
	    size_t size,
	    int (*compar)(const void*,const void*),
	    int num_keys,
	    int mpi_comm_rank,
	    int num_threads
	    )
{
  
  return (void *)0;
  
}


void search(int num_elements) {

  int * int_array = new int[num_elements];
  
  srand(time(NULL));
  // initialize to random integers
  for (int i = 0; i < num_elements; i++) {
    int_array[i] = (rand() % INT_MAX) + 1;
  }
  // Sort the array
  //print_array(int_array,num_elements);
  //printf("\n");
  qsort((void *) int_array,num_elements,sizeof(int),int_comparator);

  //print_array(int_array,num_elements);
  
}
  
int main(int argc, char * argv[]) {
  
  search(5);
  
    return 0;
  
}