#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
void genericScan(void *X, int n, size_t l);

// the use must define this function to use genericScan
void userBinaryOperator( void *x1, void *x2){
	int *a = (int *) x1;
	int *b = (int *) x2;
  *a += *b;
}

static inline int nextPow2(int n)
{
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    return n;
}


int main( int argc, char **argv){	
	//time_t te,ts;	
	double te,ts;
	// problem setup
	int n=10;
	if(argc>1) n=atoi(argv[1]);
	int N=nextPow2(n);
	int *x = (int *) malloc( sizeof(int)*N) ;
	srand((unsigned)time(NULL));
	for(int i=0;i<n;i++) x[i] =rand()%3;
	for(int i=n;i<N;i++) x[i] = 0;
	int end = x[n-1];
//	printf("end = %d\n",end);
	// print input
//	printf("input:\n");
//	for(int i=0;i<n;i++) printf("%d ",x[i]); 	printf("\n");
//	ts = time(NULL);
	ts = omp_get_wtime();
	// scan
	genericScan((void*) x,N,sizeof(int));
//	te = time(NULL);
	te = omp_get_wtime();
	printf("Time= %.16g\n",te-ts);
	//print output
	int *t = x + 1;
	t[n-1] = end + x[n-1];
//	printf("result:\n");
//	for(int i=0;i<n;i++) printf("%d ",t[i]); 	printf("\n");

	// clean up
	free(x);
	return 0;
}
	




	


	



