// assignment1.cpp : Defines the entry point for the console application.
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif


typedef enum { tangent, normal }visualisation;

typedef struct
{ 
	float t, lastT, dt; 
	visualisation visualisationMode;
} Globals;

Globals global;

float waterM=0;




void drawAxes(float length)
{
  glBegin(GL_LINES);
  glColor3f(1,0,0);
  glVertex3f(0, 0, 0);
  glVertex3f(length, 0, 0);
  glEnd();
  glBegin(GL_LINES);
  glColor3f(0,1,0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, length, 0);
  glEnd();
  glBegin(GL_LINES);
  glColor3f(0,0,1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, length);
  glEnd();
}
void displayWater()
{
	float y;
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
	glColor4f(0.0, 1.0, 1.0,0.6);//light blue color with transparency

	float left=-1.0;
	float right=1.0;
	float range=2;

    int seg=1000;
    float stepSize=range/seg;

    for (float x = left; x <= right; x+=stepSize) {
      float a,b;

      y=0.25*sin(2*M_PI*x+waterM);
      glVertex3f(x, y, 0);
      glVertex3f(x, -1, 0);
    }


	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void updateWater()
{
	waterM+=0.05;
	glutPostRedisplay();
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
	

	glutIdleFunc(updateWater);

	myinit();

	glutMainLoop();
}


