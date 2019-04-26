#ifndef INTERACTIONS_H
#define INTERACTIONS_H
#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#ifndef __APPLE__
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif // __APPLE__
#define MAX(x,y)(((x)>(y))?(x):(y));
#define W 640
#define H 640
#define DT 1.f//source intensity increment 1.0f is easier to read by human but the resultant is exactly the same.
float *d_temp = 0;
int iterationCount = 0;
BC bc = { W / 2, H / 2, W / 10.f, 150, 212.f, 70.f, 0.f };//boundary conds

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'S') bc.t_s += DT;
	if (key == 's') bc.t_s -= DT;
	if (key == 'A') bc.t_a += DT;
	if (key == 'a') bc.t_a -= DT;
	if (key == 'G') bc.t_g += DT;
	if (key == 'g') bc.t_g -= DT;
	if (key == 'R') bc.rad += DT;
	if (key == 'r') bc.rad = MAX(0.f, bc.rad - DT);
	if (key == 'C') ++bc.chamfer;
	if (key == 'c') --bc.chamfer;
	if (key == 'z') resetTemperature(d_temp, W, H, bc);
	if (key == 27) exit(0);
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	bc.x = x, bc.y = y;
	glutPostRedisplay();
}

void idle(void)
{
	++iterationCount;
	glutPostRedisplay();
}

void printInstructions()
{
	printf("Temperature Visualizer:\n"
		   "Relocate source with mouse click\n"
		   "Change source temperature (-/+): s/S\n"
		   "Change air temperature    (-/+): a/A\n"
		   "Change ground temperature (-/+): g/G\n"
		   "Change pipe radius	      (-/+): r/R\n"
		   "Chnage chamfer			  (-/+): c/C\n"
		   "Reset to air temperature	   : z\n"
		   "Exit						   : Esc\n");
}
#endif // !INTERACTIONS_H