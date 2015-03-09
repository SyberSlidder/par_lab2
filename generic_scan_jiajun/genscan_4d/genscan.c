#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <stdio.h>
void userBinaryOperator(void *input1andOutput, void *input2);
void userBinaryOperator2(void *x1, void *x2)
{
//        double *a = (double *) x1;
//        double *b = (double *) x2;
        double tmp[4];
//	#pragma omp parallel for 
        for(int i=0;i<4;i++){
                tmp[i]= *(double*)(x1+i*sizeof(double)) ;	
	}
//	#pragma omp parallel for 
        for(int i=0; i<4; i++){
                *((double*)(x1+i*sizeof(double))) += *((double*)(x2+i*sizeof(double)));
	}
//	#pragma omp parallel for 
        for(int i=0; i<4; i++)
                *((double*)(x2+i*sizeof(double))) = *(tmp+i);
}

#define ubop(x1,x2) userBinaryOperator( (void *) (x1), (void *) (x2) );
#define ubop2(x1,x2) userBinaryOperator2( (void *) (x1), (void *) (x2) );

/* Helper function to round up to a power of 2. 
 *  */
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

void genericScan(void **X, int n, size_t l)
{
	int i;
	char **c = (char **)X;
//	for (i=1;i<n;i++)	
//		ubop( (void *)(c+i*l),  (void *)(c+l*(i-1)) ); 

	int level;
	int maxlevel = log(n)/log(2); 
	int stride=2;
	//upsweep
	for(level=1;level<=maxlevel;level++,stride*=2){
		#pragma omp parallel for 
 		for(i=stride-1;i<n;i+=stride){
			ubop( (void *)*(c+i), (void *)*(c+(i-stride/2))) ;
		}
	}
/*	printf("upsweep\n");
        for(int i=0;i<n;i++){
            for(int j=0; j<4;j++)
                printf("%f ",*(*(c+i)+j));
            printf("\n");
        }
*/
	double *last = (double*)*(c+(n-1));
	for(int i=0; i<4; i++)
		last[i]=0;
	//down-sweep
	for(level=1,stride=n;level<=maxlevel;level++,stride/=2){
		#pragma omp parallel for 
		for(i=n-1;i>0;i-=stride){
			ubop2( (void *)*(c+i), (void *)*(c+(i-stride/2)));
		}
	}

}


