#include <stdio.h>
#include <omp.h>
int main()
{
	int i=5; // a shared variable

#pragma omp parallel
	{
		int c; //local variables
		int tid; 
		tid = omp_get_thread_num();
		printf("tid = %d, i = %d\n",tid,i);
	}	
	return 0;
}






