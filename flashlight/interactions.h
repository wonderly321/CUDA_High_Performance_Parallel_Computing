//ָ���ص�����������flashlightӦ�ó���Ľ���
//��ʾһ���ɽ���ʽ�ı��ͼ��ÿ��ͼ��Ԫ�ض�Ӧ��λ�õ��ο���ľ��룬����ο�����Ա�����ʽ���ƶ���
#pragma once
#ifndef INTERACTIONS_H
#define INTERACTIONS_H
#define W 600//ͼ��ά��
#define H 600
#define DELTA 5//ÿ���¼�ͷ���ο����ƶ��ľ��루������Ϊ��λ��
#define  TITLE_STRING "flashlight:distance image display app"//����������ʾ���ı�
int2 loc = { W / 2,H / 2 };//�ο����λ�ã�ͼ�������
bool dragMode = false;

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		dragMode = !dragMode;
	}
	if (key == 27)
	{
		exit(0);
	}
	glutPostRedisplay();//��ÿ���ص������Ľ�β�����ã����ݽ�����������һ���µ�ͼ��������ʾ������main.cpp�е�display()��
}

void mouseMove(int x, int y)
{
	if (dragMode)
	{
		return;
	}
	loc.x = x;
	loc.y = y;
	glutPostRedisplay();
}

void mouseDrag(int x, int y)
{
	if (!dragMode)
	{
		return;
	}
	loc.x = x;
	loc.y = y;
	glutPostRedisplay();
}

void handleSpecialKeypress(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		loc.x -= DELTA;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		loc.x += DELTA;
	}
	if (key == GLUT_KEY_UP)
	{
		loc.y -= DELTA;
	}
	if (key == GLUT_KEY_DOWN)
	{
		loc.y += DELTA;
	}
	glutPostRedisplay();
}

void printInstructions()
{
	printf("flashlight interactions\n");
	printf("a: toggle mouse tracking mode\n");
	printf("arrow keys: move ref location\n");
	printf("esc: close graphics window\n");
}
#endif // !INTERACTIONS_H