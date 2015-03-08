#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);  
double dboard (int darts);

#define DARTS 50000   	/* number of throws at dartboard */
#define ROUNDS 100    	/* number of times "darts" is iterated */

int main(int argc, char *argv[])
{
	double pi;          	/* average of pi after "darts" is thrown */
	double avepi;       	/* average pi value for all iterations */
	float realpi = 3.1415926535897;
	int i, n;

	printf("Starting serial version of pi calculation using dartboard algorithm...\n");
	srandom (5);
	avepi = 0;
	for (i = 0; i < ROUNDS; i++) {
		/* Perform pi calculation on serial processor */
		pi = dboard(DARTS);
		avepi = ((avepi * i) + pi)/(i + 1); 
	}    
	printf("   After %3d throws, average value of pi = %10.8f\n",
					 (DARTS * (i + 1)),avepi);
	printf("\nReal value of PI: %f \n",realpi);
}

