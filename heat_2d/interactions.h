#ifndef INTERACTIONS_H
#define INTERACTIONS_H
#include "kernel.h"
#include <cstdio>
#include <cstdlib>
#ifdef __APPLE__
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif // __APPLE__

#define MAX(x,y)((x)>(y))?(x):(y);
#define W 640//图像维度
#define H 640
#define DT 1.0f//每按下箭头键参考点移动的距离（以像素为单位）
float *d_temp = 0;
int iterationCount = 0;
BC bc = { W / 2,H / 2,W / 10.f,150,212.f,70.f,0.f };

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'S') bc.t_s += DT;
	if (key == 's') bc.t_s -= DT;
	if (key == 'A') bc.t_a += DT;
	if (key == 'a') bc.t_a -= DT;
	if (key == 'G') bc.t_g += DT;
	if (key == 'g') bc.t_g -= DT;
	if (key == 'R') bc.rad += DT;
	if (key == 'r') bc.rad -= DT;
	if (key == 'C') ++bc.chamfer;
	if (key == 'c') --bc.chamfer;
	if (key == 'z') resetTemperature(d_temp, W, H, bc);
	if (key == 27) exit(0);
	glutPostRedisplay();//在每个回调函数的结尾被调用，根据交互输入计算出一个新的图像用于显示（调用main.cpp中的display()）
}

void mouse(int button, int state, int x, int y)
{
	bc.x = x;
	bc.y = y;
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