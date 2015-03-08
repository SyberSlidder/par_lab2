#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <mpi.h>

int main (int argc, char* argv[])
{
  char hostname[1024];
  int rank;
  int size;

  MPI_Init (&argc, &argv);

  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

  bzero (hostname, sizeof (hostname));
  gethostname (hostname, sizeof(hostname)-1);

  printf ("[%s:%d/%d] hello, world!\n", hostname, rank, size);

  MPI_Finalize ();
  return 0;
}

/* eof */


