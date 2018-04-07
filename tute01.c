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

/*void drawVector(float x, float y, float a, float b, float s, bool normalize, float r, float g, float B)
{
	int n = 20;
	float left = -1.0;
	float right = 1.0;
	float range = 0.0;
	range = right - left;
	float stepSize = 0.0;
	stepSize = range / n;
	glBegin(GL_LINES);
	for (int i = 0; i <= n; i++)
	{
		x = i * stepSize + left;
		y = 1 * sin(M_PI*x);
		b = M_PI * 1 * cos(M_PI*x);
		s = b * (x + 0.1 - x) + y;
		glVertex2f(x, y);
		glVertex2f(x + 0.1, s);
	}
	glEnd();
}*/
void square()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	glVertex3f(-0.25, -0.25, -0.75);
	glVertex3f(0.75, -0.25, -0.75);
	glVertex3f(0.75, 0.75, -0.75);
	glVertex3f(-0.25, 0.75, -0.75);
	glEnd();
}
void quardic()
{
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
	int n = 20;
	float left = -1.0;
	float right = 1.0;
	float range = 0.0;
	range = right - left;
	float stepSize = 0.0;
	stepSize = range / n;
	float x;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= n; i++)
	{
		x = i * stepSize + left;
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(x, 2 * x*x, 0.0);//diplay curve:y=x*x
	}
	glEnd();
}
void sineWave()
{
	float a, b, s;
	float r, g, B;
//	bool normalize;
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
	int n = 40;
	float left = -1.0;
	float right = 1.0;
	float range = 0.0;
	range = right - left;
	float stepSize = 0.0;
	stepSize = range / n;
	float x, y;
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i <= n; i++)
	{
		x = i * stepSize + left;
		y = 1 * sin(M_PI*x);//sineWave: y = A*sin(pie*x)
		glVertex3f(x, y, 0.0);//display sineWave
	}
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	float x1, y1;
	for (int i = 0; i <= n; i++)
	{
		x1 = i * stepSize + left;
		y1 = 1 * sin(M_PI*x1);
		b = M_PI * 1 * cos(M_PI*x1);
		s = b * (x1 + 0.1 - x1) + y1;

		glVertex2f(x1+0.1,s);
	}
	glEnd();
}

void display()
{
	/* Put drawing code here 
	glBegin(GL_POINTS);
	glVertex3f(0, 0, 0);
	glEnd();*/
	
	//square();
	//quardic();
	sineWave();
	

	glutSwapBuffers();
}

/* You can ignore this for now, it just lets you exit when you press 'q' or ESC */
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	default:
		break;
	}
}

void init()
{
	/* In this program these OpenGL calls only need to be done once,
	but normally they would go elsewhere, e.g. display */

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Tutorial 1");

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}