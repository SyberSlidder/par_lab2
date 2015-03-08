#include <cuda_runtime.h>
#include <helper_cuda.h>
#include <helper_functions.h>
#include <algorithm>
#include <vector>
#include "reduction.h"

// change this if you want to experiment with different kernels, see reduction_kernel and the GPU manual
#define REDUCE_KERNEL_TYPE 0

extern "C"
bool isPow2(unsigned int x){
    return ((x&(x-1))==0);
}


/******************************************************/
float gpu_reduce(int size, float *h_idata)
{
	int devID=0;
  cudaSetDevice(devID);
	cudaDeviceProp deviceProp;
	checkCudaErrors(cudaGetDeviceProperties(&deviceProp,devID));
  printf("GPU Device %d: \"%s\" with compute capability %d.%d\n\n", 
				 devID, deviceProp.name, deviceProp.major, deviceProp.minor); 

	int threads_per_block = 64;
	int num_blocks = size/threads_per_block;
	const int which_kernel = REDUCE_KERNEL_TYPE;

	//printf("%d GPU is blocks\n\n", num_blocks);

	//	getNumBlocksAndThreads(whichKernel, size, maxBlocks, maxThreads, num_blocks, numThreads);
	
	// allocate mem for the result on host side
	float *h_odata = (float *) malloc(num_blocks*sizeof(float));
	// allocate device memory and data
	float *d_idata = NULL;
	float *d_odata = NULL;

	unsigned int bytes = size * sizeof(float);

	// move memory from device to host
	checkCudaErrors(cudaMalloc((void **) &d_idata, bytes));
	checkCudaErrors(cudaMalloc((void **) &d_odata, num_blocks*sizeof(float)));
	checkCudaErrors(cudaMemcpy(d_idata, h_idata, bytes, cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(d_odata, h_idata, num_blocks*sizeof(float), cudaMemcpyHostToDevice));

	reduce<float>(size, threads_per_block, num_blocks, which_kernel, d_idata, d_odata);
	checkCudaErrors(cudaMemcpy(h_odata, d_odata, num_blocks*sizeof(float), cudaMemcpyDeviceToHost));
	
	float gpu_result=0;
	for (int i=0; i<num_blocks; i++)	gpu_result += h_odata[i];

	free(h_odata);
 	checkCudaErrors(cudaFree(d_idata));
	checkCudaErrors(cudaFree(d_odata));
	cudaDeviceReset();
	return gpu_result;
}

/******************************************************/
/* Example on  how to use the code */
void TestGPUReduce(void){
	int size = 256*64*32;

	std::vector<float>data; data.resize(size);
	for (int i=0; i<size; i++) 
		data[i] = (rand() & 0xFF) / (float)RAND_MAX;

	printf("size = %d\n", size);

	float res =  gpu_reduce(size,&data[0]);

	float cpu_res=0;	
	for (int i=0;i<size;i++) cpu_res+=data[i];
	
	printf("gpu_error_calculation=%f %f %f\n", cpu_res, res, (res-cpu_res)/cpu_res);

	printf("ok\n");
}
