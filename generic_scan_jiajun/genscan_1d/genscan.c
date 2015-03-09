#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <stdio.h>
void userBinaryOperator(void *input1andOutput, void *input2);
void userBinaryOperator2(void *input1andOutput, void *input2)
{
        int *a = (int *) input1andOutput;
        int *b = (int *) input2;
	int tmp = *a;
	*a += *b;
	*b = tmp;
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

void genericScan(void *X, int n, size_t l)
{
	int i;
	int *c = (int *)X;
//	char *limit = c+(n-1)*l; //address of last element
//	for (i=1;i<n;i++)	
//		ubop( (void *)(c+i*l),  (void *)(c+l*(i-1)) ); 

	int level;
	int maxlevel = log(n)/log(2); 
	int stride=2;
	//upsweep
	for(level=1;level<=maxlevel;level++){
		//printf("level,stride %d  %d\n",level,stride);
		#pragma omp parallel for 
 		for(i=stride-1;i<n;i+=stride){
			//ubop( (void *)(c+i*l), (void *)(c+l*(i-stride/2))) ;
			ubop( (void *)(c+i), (void *)(c+(i-stride/2))) ;
			//int  tid = omp_get_thread_num();
			//printf("thread = %d\n", tid);
		}
		stride *=2;
	}

	//int *last = (int*)(c+l*(n-1));
	int *last = (int*)(c+(n-1));
	*last = 0;
	//down-sweep
	for(level=1,stride=n;level<=maxlevel;level++,stride/=2){
//		printf("level,stride %d  %d\n",level,stride);
		#pragma omp parallel for 
		for(i=n-1;i>0;i-=stride){
			//ubop2( (void *)(c+l*i), (void *)(c+l*(i-stride/2)));
			ubop2( (void *)(c+i), (void *)(c+(i-stride/2)));
//                      printf("pair %d and %d\n",i,i-stride/2);
		}
	}

}


