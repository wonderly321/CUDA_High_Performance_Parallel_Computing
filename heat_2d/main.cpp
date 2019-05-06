#include "interactions.h"
#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif // _WIN32
#ifdef __APPLE__
#else 
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif // __APPLE__

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#define ITERS_PER_RENDER 50

//texture and pixel objects
GLuint pbo = 0;
GLuint tex = 0;
struct cudaGraphicsResource *cuda_pbo_resource;

void render() {
	uchar4 *d_out = 0;
	cudaGraphicsMapResources(1, &cuda_pbo_resource, 0);
	cudaGraphicsResourceGetMappedPointer((void **)&d_out, NULL, cuda_pbo_resource);

	for (int i = 0; i < ITERS_PER_RENDER; ++i) {
		kernelLauncher(d_out, d_temp, W, H, bc);
	}
	cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0);
	char title[128];
	sprintf(title, "Temperature Visualizer - Iteration=%4d, ""T_s=%3.0f, T_a=%3.0f,T_g=%3.0f", iterationCount, bc.t_s, bc.t_a, bc.t_g);
	glutSetWindowTitle(title);
}

void draw_Texture() {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0, H);
	glTexCoord2f(1.0f, 1.0f); glVertex2f(W, H);
	glTexCoord2f(1.0f, 0.0f); glVertex2f(W, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
void display() {
	render();
	draw_Texture();
	glutSwapBuffers();
}
void initGLUT(int *argc, char **argv) {
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutCreateWindow("Temp. Vis.");
#ifndef __APPLE__
	glewInit();
#endif // !__APPLE__
}

void initPixelBuffer() {
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, W*H * sizeof(GLubyte)*4, 0, GL_STREAM_DRAW);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, pbo, cudaGraphicsMapFlagsWriteDiscard);


}
void exitfunc() {
	if (pbo) {
		cudaGraphicsUnregisterResource(cuda_pbo_resource);
		glDeleteBuffers(1, &pbo);
		glDeleteTextures(1, &tex);
	}
	cudaFree(d_temp);
}
int main(int argc, char** argv) {
	cudaMalloc(&d_temp, W*H * sizeof(float));
	resetTemperature(d_temp, W, H, bc);
	printInstructions();
	initGLUT(&argc, argv);
	gluOrtho2D(0, W, H, 0);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	initPixelBuffer();
	glutMainLoop();
	atexit(exitfunc);
	return 0;
}

