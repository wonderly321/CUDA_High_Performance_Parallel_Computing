#include "kernel.h"
#include <cmath>
#include <cstdio>

int main() {
	const float	PI = 3.1415927;
	const int N = 150;
	const float h = 2 * PI / N;

	float x[N] = { 0.0 };
	float u[N] = { 0.0 };
	float result_parallel[N] = { 0.0 };

	for (int i = 0; i < N; ++i) {
		x[i] = i*h;
		u[i] = sinf(x[i]);
	}
	
	ddParallel(result_parallel, u, N, h);
	//printf("%f",result_parallel[0]);
	FILE *outfile = fopen("dd_ld_global_results.csv", "w");
	for (int i = 1; i < N-1; ++i) {
		fprintf(outfile, "%f,%f,%f,%f\n", x[i], u[i], result_parallel[i], result_parallel[i] + u[i]);
	}
	fclose(outfile);
}