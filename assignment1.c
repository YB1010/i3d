// assignment1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <stdlib.h>
#include <stdio.h>

#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#define M_PI 3.1415

typedef enum { tangent, normal }visualisation;

typedef struct
{ 
	float t, lastT, dt; 
	visualisation visualisationMode;
} Globals;

Globals global;
void displayWater()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);//color red
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);//display of x axis
	glColor3f(0.0, 1.0, 0.0);//color green
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);//display of y axis
	glColor3f(0.0, 0.0, 1.0);//color blue
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);//display of z axis
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glColor4f(0.0, 0.0, 1.0,0.8);//light blue color with transparency
	glVertex2f(-1.0, -1.0);//v0
	glVertex2f(-1.0, 0);//v1
	glVertex2f(0, -1.0);//v2
	glVertex2d(0, 0);//v3
	glVertex2f(1.0, -1.0);//v4
	glVertex2f(1.0, 0);//v5
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	/* Draw something here */
	displayWater();
	glutSwapBuffers();
}
void keyboardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	/*case 'n':
		if (global.visualisationMode == tangent)
			prin
		else
			global.visualisationMode == normal;
	    break;*/

	default:
		break;
	}
}
void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void myinit(void)
{
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(500, 500);
	glutCreateWindow("assignment 1");
	glutKeyboardFunc(keyboardCB);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	

	myinit();

	glutMainLoop();
}


