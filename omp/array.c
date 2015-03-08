/* wall clock */
#include <sys/time.h>

/*  filename arrayUpdate.c            */
/*  OpenMP tutorial example           */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>  /* Here's the header file for OpenMP. */

double walltime( double* ); /* the clock on the wall */


/* ********************************************************************** */
int array1( int argc, char *argv[] )
{

  /*  Arguments required for executing argv[0]:    */
  /*  0                         -> (executable)    */
  /*  1                         -> arraySize       */

  double *x, *y;     /* the arrays                 */
  int    arraySize;  /* get from the command line  */
  int    i;

  double startTime, elapsedTime; /* for timing */
  double clockZero = 0.0;

  arraySize = (int) atof( argv[1] );
  
  /* Check the command line arguments. */
  if (argc != 2)
    {
      fprintf( stdout, "usage:  %s  arraySize\n", argv[0] );
      exit(-1);
    }

  /* Allocate memory for the arrays. */
  x = (double *) malloc( (size_t) (  arraySize * sizeof(double) ) );
  y = (double *) malloc( (size_t) (  arraySize * sizeof(double) ) );

  /* Initialize x with some junk. */
  for ( i = 0; i < arraySize; i++ )
    {
      x[i] = ( (double) i ) / ( i + 1000 );
    }

  /* Now begins the real work which we want to parallelize. */
  /* Mark the starting time. */
  startTime = walltime( &clockZero );

  /* Here's the OpenMP pragma that parallelizes the for-loop. */
#pragma omp parallel for
  for ( i = 0; i < arraySize; i++ )
    {
      y[i] = sin( exp( cos( - exp( sin(x[i]) ) ) ) );
    }

  /* Work's done. Get the elapsed wall time. */
  elapsedTime = walltime( &startTime );

  /* Print the wall time and terminate. */
  fprintf( stdout, "\nwall time = %.2fs for array size = %.0e\n",
			  elapsedTime, (double) arraySize );

  fprintf( stdout, "\nProgram successfully terminated.\n\n\a" );

  return(0);
}


/* ********************************************************************** */
int array2( int argc, char *argv[] )
{

  /*  Arguments required for executing argv[0]:    */
  /*  0                         -> (executable)    */
  /*  1                         -> arraySize       */

  double *x, *y;     /* the arrays                 */
  int    arraySize;  /* get from the command line  */
  int    i;

  /* Additional variables for arrayUpdate_II: */
  int    privIndx;
  double privDbl;

  /* Additional shared variable for arrayUpdate_III: */
  int globalCount;

  double startTime, elapsedTime; /* for timing */
  double clockZero = 0.0;

  arraySize = (int) atof( argv[1] );
  
  /* Check the command line arguments. */
  if (argc != 2)
    {
      fprintf( stdout, "usage:  %s  arraySize\n", argv[0] );
      exit(-1);
    }

  /* Allocate memory for the arrays. */
  x = (double *) malloc( (size_t) (  arraySize * sizeof(double) ) );
  y = (double *) malloc( (size_t) (  arraySize * sizeof(double) ) );

  /* Initialize x with some junk. */
  for ( i = 0; i < arraySize; i++ )
    {
      x[i] = ( (double) i ) / ( i + 1000 );
    }

  /* Now begins the real work which we want to parallelize.        */
  /* Initialize the counter that resides in the global data space. */
  globalCount = 0;

  /* Mark the starting time. */
  startTime = walltime( &clockZero );

  /* Here's the OpenMP pragma that parallelizes the for-loop. */
  /* This parallel construct has 3 private variables:         */
  /*   1) outer loop index "i" by default                     */
  /*   2) inner loop index "privIndx" by explicit declaration */
  /*   3) variable "privDbl" by explicit declaration          */
#pragma omp parallel for private( privIndx, privDbl )
  for ( i = 0; i < arraySize; i++ )
    {
      for ( privIndx = 0; privIndx < 16; privIndx++ )
		  {
			 privDbl = ( (double) privIndx ) / 16;
			   y[i] =
				  sin( exp( cos( - exp( sin(x[i]) ) ) ) ) + cos( privDbl );

				/* globalCount totals the number of times sin() is called. */
				/* Here, each thread must read globalCount, add 1 to the   */
				/* value, and write the new value back to globalCount.     */
				globalCount = globalCount + 1;  /* same as globalCount++   */
		  }
    }

  /* Work's done. Get the elapsed wall time. */
  elapsedTime = walltime( &startTime );

  /* Print the global counter. */
  fprintf( stdout, "\nthe global counter should equal %d\n", arraySize*16 );
  fprintf( stdout, "globalCount = %d\n", globalCount );

  /* Print the wall time and terminate. */
  fprintf( stdout, "\nwall time = %.2fs for array size = %.0e\n",
			  elapsedTime, (double) arraySize );

  fprintf( stdout, "\nProgram successfully terminated.\n\n\a" );

  return(0);
}


/* ********************************************************************** */
double walltime( double *t0 )
{
  double mic, time;
  double mega = 0.000001;
  struct timeval tp;
  struct timezone tzp;
  static long base_sec = 0;
  static long base_usec = 0;

  (void) gettimeofday(&tp,&tzp);
  if (base_sec == 0)
    {
      base_sec = tp.tv_sec;
      base_usec = tp.tv_usec;
    }

  time = (double) (tp.tv_sec - base_sec);
  mic = (double) (tp.tv_usec - base_usec);
  time = (time + mic * mega) - *t0;
  return(time);
}


/* ********************************************************************** */
int main( int argc, char *argv[]){
  return array2(argc,argv);
  }

