#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#ifdef _OPENMP
#include <omp.h>
#endif

typedef float real;

int main (int argc, char *argv[]) {

  double te,ts;
  int   i, n;
  real *x, *y;
	real a;
	
	a = 1./3.;
  
	if(argc>1){
		n = atoi(argv[1]);
		assert(n>2);
  }else{
		n=1000000; /* 1M */
  }

	x = (real*) malloc(n*sizeof(real)); assert(x!=0);
	y = (real*) malloc(n*sizeof(real)); assert(y!=0);

#pragma omp parallel for schedule(static)
	for (i=0;i<n;i++){
		x[i] = i %10;
		y[i] =(i/2 +x[i]) ;
	}

#ifdef _OPENMP
	ts = omp_get_wtime();
#endif

#pragma omp parallel for schedule(static)
	for (i=0;i<n;i++){
		x[i] = a*x[i] + y[i];
	}

#ifdef _OPENMP
	te = omp_get_wtime();
  printf("  Time=%g\n", te-ts);
#endif
	
	free(x);
	free(y);
	return 0;
}

	



