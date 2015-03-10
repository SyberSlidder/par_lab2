#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
void genericScan(void **X, int n, size_t l);

// the use must define this function to use genericScan
void userBinaryOperator( void *x1, void *x2){
//	#pragma omp parallel for
 	for(int i=0; i<4; i++){
		*((double*)(x1+i*sizeof(double))) += *((double*)(x2+i*sizeof(double)));
	}
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
	omp_set_num_threads(32);	
	//time_t te,ts;	
	double te,ts;
	// problem setup
	int n=10;
	if(argc>1) n=atoi(argv[1]);
	int N=nextPow2(n);
//	printf("N=%d\n",N);
	double **x;
	x = (double**)malloc(N*sizeof(double*));
        if(x==NULL){printf("malloc fail!,sizeofchar*=%d,sizeofdouble*=%d\n",sizeof(char*),sizeof(double*)); return 0;}
	for(int i=0;i<N;i++){
		x[i] =(double*) malloc(4*sizeof(double));
		if(x[i] == NULL){ printf("x[%d] malloc failed \n",i);
				return 0;}
	}
//	printf("malloc success !\n");
	srand((unsigned)time(NULL));

	// print input
	for(int i=0; i<n; i++){
	    for(int i0=0;i0<4;i0++){
		x[i][i0] = rand()*0.001;
///		printf("%f  ",x[i][i0]);
	    }
//	    printf("\n");
	}

	for(int i=n; i<N; i++)
            for(int i0=0;i0<4;i0++)
                x[i][i0] = 0;
	double end[4];
	for(int i=0;i<4;i++)
                end[i]= x[n-1][i] ;
	ts = omp_get_wtime();
	// scan
	genericScan((void**) x,N,sizeof(double*));
	te = omp_get_wtime();
	printf("Time= %.4g\n",te-ts);
	//print output
	double **t = x+1;
	for(int i=0;i<4;i++){
            t[n-1][i] = x[n-1][i]+ end[i] ;
	//    printf("t[n-1][%d]=%f\n",i,t[n-1][i]);
	}
/*
	for(int i=0;i<n;i++){ 
	    for(int j=0; j<4;j++)
		printf("%f ",t[i][j]); 	
	    printf("\n");
	}
*/
	// clean up
	free(x);
	return 0;
}
	




	


	



