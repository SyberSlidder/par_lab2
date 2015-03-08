#include <stdio.h>
#include <omp.h>

void work()
{
	int i;

	i = 10;
#pragma omp task shared(i)
	{
		int tid;
		tid = omp_get_thread_num();
#pragma omp critical
		printf("In Task %d, i = %d\n",tid, i);
	}

	/* Use TASKWAIT for synchronization. */
#pragma omp taskwait
}

int main(int argc, char** argv)
{
	omp_set_num_threads(8);
	omp_set_dynamic(0);

#pragma omp parallel 
	{
		work();
	}
}
