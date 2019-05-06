#include "kernel.h"
#define cimg_display 0 //not use CImg to display
#include "CImg.h"
#include <cuda_runtime.h>
#include <cstdlib>

//for debug
#include <iostream>

using namespace std;
using namespace cimg_library;
int main() {
	//cimg_library::CImg<unsigned char> image("39.bmp");//as input & output  ???
	CImg<unsigned char> image("33.bmp");
	const int w = image.width();
	const int h = image.height();

	//Initialize uchar4 array for image processing
	uchar4 *arr = (uchar4*)malloc(w*h * sizeof(uchar4));
	//copy CImg data to array
	for (int r = 0; r < h; ++r) {
		for (int c = 0; c < w; ++c) {
			arr[r*w + c].x = image(c, r, 0);
			arr[r*w + c].y = image(c, r, 1);
			arr[r*w + c].z = image(c, r, 2);
		}
	}
	sharpenParallel(arr, w, h);

	//copy from array to CImg data
	for (int r = 0; r < h; ++r) {
		for (int c = 0; c < w; ++c) {
			image(c, r, 0) = arr[r*w + c].x;
			image(c, r, 1) = arr[r*w + c].y;
			image(c, r, 2) = arr[r*w + c].z;			
		}
	}
	image.save_bmp("33s.bmp");
	free(arr);
	return 0;
}