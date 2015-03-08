#include <stdio.h>
#include <stdlib.h>
void srandom (unsigned seed);
double dboard (int darts);
#define ROUNDS 10       /* number of times "darts" is iterated */

#include "mpi.h"
#define ROOT 0        /* task ID of master task */
#define DARTS 2000000     /* number of throws at dartboard */
int main (int argc, char *argv[])
{
	double	homepi,        /* value of pi calculated by current task */
		pisum,      	       /* sum of tasks' pi values */
		pi;  	               /* average of pi after "darts" is thrown */
	int	rank,	             /* task ID - also used as seed number */
		size,                /* number of tasks */
		rc,                  /* return code */
		i;
	MPI_Status status;

	/* Obtain number of tasks and task ID */
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	printf ("MPI task %d has started...\n", rank);

	if (rank == ROOT) 
		printf ("Using %d tasks to compute pi (3.1415926535)\n",size);

	/* Set seed for random number generator equal to task ID */
	srandom (rank);

	pi = 0;
	/* All tasks calculate pi using dartboard algorithm */
	homepi = dboard(DARTS);
	rc = MPI_Reduce(&homepi, &pisum, 1, MPI_DOUBLE, MPI_SUM,ROOT, MPI_COMM_WORLD);
	if (rc != MPI_SUCCESS)  printf("%d: failure on mpc_reduce\n", rank);

	if (rank == ROOT) pi = pisum/size;

	if(rank==ROOT)
		printf("After %8d throws, average value of pi = %10.8f\n",	DARTS*size,pi);

	MPI_Finalize();
	return 0;
}

