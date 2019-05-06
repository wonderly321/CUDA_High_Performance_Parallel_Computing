#include "kernel.h"
#include <helper_cuda.h>
#include <cstdio>
//#include <time.h>
#define TPB 3200
#define M 100 //Number of times to do the data transfer

__device__ float distance(float x1, float x2) {
	return sqrt((x2 - x1)*(x2 - x1));
}
__global__ void distanceKernel(float *d_out, float *d_in, float ref) {
	const int i = blockIdx.x * blockDim.x + threadIdx.x;
	const float x = d_in[i];
	d_out[i] = distance(x, ref);
	printf("i = %2d: dist from %f to %f is %f.\n", i, ref, x, d_out[i]);
}
void distanceArray(float *out, float *in, float ref, int len) {
	//Create event variables for timing.
	cudaEvent_t startMemcpy, stopMemcpy;
	cudaEvent_t startKernel, stopKernel;
	cudaEventCreate(&startMemcpy);
	cudaEventCreate(&stopMemcpy);
	cudaEventCreate(&startKernel);
	cudaEventCreate(&stopKernel);

	float *d_in = 0;
	float *d_out = 0;

	checkCudaErrors(cudaMalloc(&d_in, len * sizeof(float)));
	checkCudaErrors(cudaMalloc(&d_out, len * sizeof(float)));
	// Record the event that "start the clock" on data transfer
	cudaEventRecord(startMemcpy);
	//clock_t memcpyBegin = clock();
	//Copy input data from host to device M times 
	for (int i = 0; i < M; i++) {
		checkCudaErrors(cudaMemcpy(d_in, in, len * sizeof(float), cudaMemcpyHostToDevice));
	}
	//Record the event that "stop the clock" on data transfer
	cudaEventRecord(stopMemcpy);
	//clock_t memcpyEnd = clock();
	//Record the event that "start the clock" on kernel execution
	cudaEventRecord(startKernel);
	//clock_t kernelBegin = clock();
	distanceKernel<<< len/TPB, TPB >>> (d_out, d_in, ref);
	//Record the event that "stop the clock" on kernel execution
	checkCudaErrors(cudaPeekAtLastError());
	checkCudaErrors(cudaDeviceSynchronize());

	cudaEventRecord(stopKernel);
	//clock_t kernelEnd = clock();

	checkCudaErrors(cudaMemcpy(out, d_out, len * sizeof(float), cudaMemcpyDeviceToHost));
	
	////Compute time in seconds between clock count readings
	//double memcpyTime = ((double)(memcpyEnd - memcpyBegin)) / CLOCKS_PER_SEC;
	//double kernelTime = ((double)(kernelEnd - kernelBegin)) / CLOCKS_PER_SEC;

	//Ensure timed events have stopped.
	cudaEventSynchronize(stopMemcpy);
	cudaEventSynchronize(stopKernel);
	//Cnovert event records to time and output.
	float memcpyTimeInMs = 0;
	cudaEventElapsedTime(&memcpyTimeInMs, startMemcpy, stopMemcpy);
	float kernelTimeInMs = 0;
	cudaEventElapsedTime(&kernelTimeInMs, startKernel, stopKernel);
	printf("Kernel time (ms): %f\n", kernelTimeInMs);
	printf("Data transfer time (ms):%f\n", memcpyTimeInMs);

	checkCudaErrors(cudaFree(d_in));
	checkCudaErrors(cudaFree(d_out));
}