/******************************************************************************
* FILE: omp_reduction.c
* DESCRIPTION:
*   OpenMP Example - Combined Parallel Loop Reduction - C/C++ Version
*   This example demonstrates a sum reduction within a combined parallel loop
*   construct.  Notice that default data element scoping is assumed - there
*   are no clauses specifying shared or private variables.  OpenMP will
*   automatically make loop index variables private within team threads, and
*   global variables shared.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
******************************************************************************/
/* ********************************************************************** */
/*  Revized by George Biros */

#ifdef _OPENMP
#include <omp.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main (int argc, char *argv[]) {

  double te,ts;
  int   i, n;
  float *a, *b, sum;

  if(argc>1){
	 n = atoi(argv[1]);
	 assert(n>2);
  }else{
	 n=1000000; /* 1M */
  }

  a=(float*)malloc((size_t)n*sizeof(float));
  b=(float*)malloc((size_t)n*sizeof(float));

#ifdef _OPENMP
	omp_set_dynamic(0);
#endif 

/* Some initializations */
#ifdef _OPENMP
#pragma omp parallel for  
#endif
  for (i=0; i < n; i++)
	 a[i] = b[i] = 0.1;
  sum = 0.0;

#ifdef _OPENMP
  ts = omp_get_wtime();
#endif 
  /* ********************** begin parallel region ************************* */
#ifdef _OPENMP
#pragma omp parallel for reduction(+:sum) schedule(static)
#endif
  for (i=0; i < n; i++)
    sum +=  (a[i] * b[i]);
  /* ********************* end parallel region **************************** */

#ifdef _OPENMP 
  te = omp_get_wtime();
  printf("   Sum = %f \t  Time=%g\n",sum, te-ts);
#endif 

  free(a);
  free(b);
  return 0;
}
