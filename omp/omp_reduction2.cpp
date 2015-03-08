/******************************************************************************
* FILE: omp_reduction.c
* DESCRIPTION:
*   OpenMP Example - Combined Parallel Loop Reduction - C/C++ Version
*   This example demonstrates a sum reduction within a combined parallel
 loop
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

#include "seq_reduction.hxx"


#define MAX_THREAD_NUM 12
typedef float real;

int main (int argc, char *argv[]) {

  double te,ts;
  int   i, n, no;
  real *a, *b, sum;

  if(argc>1){
	 no = atoi(argv[1]);
	 n= no * MAX_THREAD_NUM;
	 assert(n>2);
  }else{
	 no=1000000; /* 1M */
	 n = no*MAX_THREAD_NUM;
  }

  a=(real*)malloc((size_t)n*sizeof(real));
  b=(real*)malloc((size_t)n*sizeof(real));


/* Some initializations */
#ifdef _OPENMP
  printf("has openmp\n");
#pragma omp parallel for schedule (static) 
#endif
  for (i=0; i < n; i++)
	 a[i] = b[i] = 0.1;
  sum = 0.0;

 
   ts = omp_get_wtime();
  /* ********************** begin parallel region ************************* */
#ifdef _OPENMP
#pragma omp parallel for reduction(+:sum) schedule(static) 
#endif
  for (i=0;i<n;i++) sum+=a[i]*b[i];
  //  for (i=0; i < MAX_THREAD_NUM; i++)    sum += reduction<real>(&a[i*no],&b[i*no],no);
  /* ********************* end parallel region **************************** */

  te = omp_get_wtime();
  printf("   Sum = %f \t  Time=%g\n",sum, te-ts);

  free(a);
  free(b);
  return 0;
}
