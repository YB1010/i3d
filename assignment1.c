// assignment1.cpp : Defines the entry point for the console application.
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

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

typedef struct { float x, y; } vec2f;

Globals global;

float waterM=0;
bool waterM_bool=true;
bool debug_normal=false;
bool debug_tangent=false;
bool debug_water=false;


void drawVector(float x, float y,float a, float b, float s, bool normalize, float red, float green, float blue)
{
	glBegin(GL_LINES);
	glColor3f(red,green,blue);
	glVertex3f(x,y,0);
	if (normalize==true)
	{
		float v=sqrt((a*a)+(b*b));
		a=a/v;
		b=b/v;
		a*=s;
		b*=s;
		glVertex3f(x+a,y+b,0);
	}
	else
	{
		a*=s;
		b*=s;
		glVertex3f(x+a,y+b,0);
	}


}
void drawTan(float x, float y, float s, float red, float green, float blue)
{
	glBegin(GL_LINES);
	glColor3f(red,green,blue);
	float slope;
    float y1,x1;

    y=0.25*sin(2*M_PI*x+waterM);

    slope=0.25*2*M_PI*cos(2*M_PI*x+waterM);
    x1=x;


  	x1+=s;
  	y1=slope*(x1-x)+y;



 	drawVector(x,y,x1-x,y1-y,1,false,red,green,blue);

 	glEnd();

}
void drawNormal(float x, float y, float s, float red, float green, float blue)
{
	glBegin(GL_LINES);
	glColor3f(red,green,blue);
	float slope;
    float y1,x1;

    y=0.25*sin(2*M_PI*x+waterM);

    slope=0.25*2*M_PI*cos(2*M_PI*x+waterM);
    x1=x;


  	x1+=s;
  	y1=(slope)*(x1-x)+y;


 	drawVector(x,y,-(y1-y),x1-x,0.1,true,0,0,1);


 	glEnd();
}

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

	glBegin(GL_LINES);
/*
	if (debug_water==false)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			printf("wireframe off.\n");
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
			printf("wireframe on.\n");
		}
*/


	glColor4f(0.0, 1.0, 1.0,0.6);//light blue color with transparency

	float left=-1.0;
	float right=1.0;
	float range=2;

    int seg=1000;
    float stepSize=range/seg;

    

    for (float x = left; x <= right; x+=stepSize) {

	    y=0.25*sin(2*M_PI*x+waterM);
	    glVertex3f(x, y, 0);

    	glVertex3f(x,-1,0);

    }

	glEnd();

	float tx,ty;

	if (debug_tangent==true)
	{
		for (float x = left; x <= right; x+=0.05)
		{

	        y=0.25*sin(2*M_PI*x+waterM);
			
		    drawTan(x,y,0.05,1,0,0);
		}

	}

	if (debug_normal==true)
	{
		for (float x = left; x <= right; x+=0.05)
		{
			y=0.25*sin(2*M_PI*tx+waterM);
		    drawNormal(x,y,0.05,0,1,0);
		}
	}	


			
	

	
}
void updateWater()
{
	if (waterM_bool==true)
	{	
		waterM+=0.05;
	}
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	/* Draw something here */

	glPolygonMode(GL_BACK, GL_LINES);
	glPolygonMode(GL_FRONT, GL_LINES);
	displayWater();

    gluErrorString(glGetError());
	glutSwapBuffers();
}
void keyboardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	case '`':
		if (waterM_bool==false)
		{
			waterM_bool=true;
		}
		else
		{
			waterM_bool=false;
		}
		break;
	case 'w':
		if (debug_water==false)
		{
			debug_water=true;
		}
		else
		{
			debug_water=false;
		}
		break;
	case 'n':
		if (debug_normal==false)
		{
			debug_normal=true;
		}
		else
		{
			debug_normal=false;
		}
		break;
	case 't':
		if (debug_tangent==false)
		{
			debug_tangent=true;
		}
		else
		{
			debug_tangent=false;
		}
		

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


