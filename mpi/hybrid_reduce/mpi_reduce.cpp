/* 
	 Example of a hybrid API using OpenMP and MPI 
	 . Sums the elements of a 1D array of size n.
   . Uses openMP reduction and MPI_Reduce()
	 
	 using a gcc-based MPI
	 > mpicxx -fopenmp mpi_reduce.cpp -o reduc
   using a icc-based MPI
	 > mpicxx -openmp mpi_reduce.cpp -o reduc
	 and run with
	 > qsub -I -l nodes=2
	 > export OMP_NUM_THREADS=4; mpirun -np 2 ./reduce [array_size]
*/


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <vector>

#define N_DEFAULT 64*1024*2
#define REAL float
//#define USE_GPU

#ifdef USE_GPU
float gpu_reduce( int size, float *h_idata);
void	TestGPUReduce(void);
#endif 

int main( int argc, char *argv[]){

	MPI_Init(&argc,&argv); 	// intialize MPI and MPI-related variables 
	MPI_Comm comm=MPI_COMM_WORLD;
	int rank; MPI_Comm_rank( comm, &rank);
	int size; MPI_Comm_size( comm, &size);
	int root = 0;
	bool iamroot = (rank==root);

	// read problem size and broadcast to all processors
	int n; // problem size  per MPI TASK, should be dvisible by 2.
	if(iamroot)	n= (argc==2) ? atoi(argv[1]) : N_DEFAULT;
	n = int(floor(double(n))); 
	MPI_Bcast((void*)&n, 1, MPI_INT, root, comm);
	if(iamroot) printf("\t\n REDUCTION: global size=%ld, local size=%d\n", long(n)*size, n);
	MPI_Barrier(comm);

#ifdef USE_GPU
	if(iamroot){
		printf("/******************************************************/\n");
		printf("WARNING:\nSince you're using GPUs, make sure you're using one MPI per GPU.\nIf you don't it will fail.\n");
		printf("/******************************************************/\n");
	}
#endif

	// initialize array values
	std::vector<REAL> A; 	A.resize(n);
#pragma omp parallel for 
	for(int i=0;i<n;i++) A[i] =REAL(1e-2);  

	// OMP + GPU
	double tic = MPI_Wtime();
	double sum_gpu = 0;
	double sum=0;

	// istart: used to control load of gpu. simple devide by w hear
	int istart=0; 
#ifdef USE_GPU
	istart=n/2; // half of the load will be taken by the GPU
#endif

	//TestGPUReduce();  

#pragma omp parallel
	{
#ifdef USE_GPU
#pragma omp single nowait
		int c=1;
		sum_gpu = double(gpu_reduce(n/2,&A[0]));
#endif 

#pragma omp for reduction(+ : sum)	
		for(int i=istart; i<n; i++) sum += double(A[i]);		
	}

	sum = sum+sum_gpu;  // if no gpu, sum_gpu = 0.
	// MPI Reduction
	double sum_all;
	MPI_Reduce( (void *)&sum,			// input
							(void *)&sum_all, // final result
							1,								// data length
							MPI_DOUBLE,				// data type
							MPI_SUM,					// reduction operator
							0,								// process id that will collect the answer
							comm							// communicator
							);
	double toc = MPI_Wtime();

	int ntds;		
#pragma omp parallel
#pragma omp single
	ntds = omp_get_num_threads();

#ifdef USE_GPU
	printf("[MPI rank=%d] sum of %d elements in gpu %f\n", rank, n/2, sum_gpu);
#endif

	MPI_Barrier(comm); 
	fflush(stdout);
	if(iamroot) printf("\n\tUsing %d processes and %d threads/process\n", size,ntds);
	if(iamroot) printf("\tThe answer is %.2g. It should be:%.2g\n\n", sum_all, double(n*size)*1e-2);
	MPI_Barrier(comm);

	fflush(stdout);
	printf("Rank [%d], ellapsed time=%2.2g secs\n", rank, toc-tic);
	fflush(stdout);
	MPI_Finalize();
	return 0;
}




	
 

	
	
