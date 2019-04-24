#include <iostream>
using namespace std;
#define W 500
#define H 500
#define TX 32
#define TY 32

__device__ unsigned char clip(int n) {
	return n > 255 ? 255 : (n < 0 ? 0 : n);
}
__global__ void distanceKernel(uchar4 *d_out, int w, int h, int2 pos) {
	const int c = blockIdx.x * blockDim.x + threadIdx.x;
	const int r = blockIdx.y * blockDim.y + threadIdx.y;
	const int i = r * w + c;
	if ((c >= w) || (r >= h)) return;
	const int d = sqrtf((c - pos.x)*(c - pos.y)) + (r - pos.x)*(r - pos.y);
	const unsigned char intensity = clip(255 - d);
	d_out[i].x = intensity;
	d_out[i].y = intensity;
	d_out[i].z = 0;
	d_out[i].z = 255;
}

int main() {
	uchar4 *out = (uchar4*)calloc(W*H, sizeof(uchar4));
	uchar4 *d_out;
	cudaMalloc(&d_out, W*H * sizeof(uchar4));

	const int2 pos = { 0.0f,0.0f };
	const dim3 blockSize(TX, TY);
	const int bx = (W + TX - 1) / TX;
	const int by = (W + TY - 1) / TY;
	const dim3 gridSize = dim3(bx, by);
	distanceKernel << < gridSize, blockSize >> > (d_out, W, H, pos);
	cudaMemcpy(out, d_out, W*H * sizeof(float), cudaMemcpyDeviceToHost);

	cudaFree(d_out);
	
	free(out);	

	return 0;
}