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

#ifndef WATER_SPEED
#define WATER_SPEED 0.02;
#endif

typedef enum { tangent, normal }visualisation;

typedef struct
{
	float t, lastT, dt;
	visualisation visualisationMode;
} Globals;

typedef struct { float x, y; } vec2f;


typedef struct {

  float waterM;
  bool waterM_bool;
  bool debug_normal;
  bool debug_tangent;
  bool debug_water;
  int islandCannon;
  float lmove;
  float rmove;
  float lrotate;
  float rrotate;
  int leftBoat;
  int rightBoat;
  int boatCannon;
  float startTime;
  int frames;
  float frameRate;
  float frameRateInterval;
  float lastFrameRateT;

} global_t;

typedef struct
{
	int life;
	bool alive;
	//to be fill
}boat;


typedef struct
{
	bool alive;

}cannon;

typedef struct
{
	int life;
	bool alive;
	//to be fill
}island;

const int milli = 1000;
global_t global={0,true,false,false,false,0,-0.5,0.5,30.0,150.0,0,0,0,0,0,0.2,0};

void updateWater()
{

	if (global.waterM_bool==true)
	{
		global.waterM+=WATER_SPEED;
	}

	glutPostRedisplay();
}

float getSineY(float x)
{
	float y;
	y=0.25*sin(2*M_PI*x+global.waterM);
	return y;
}

float getSineSlope(float x)
{
	float slope;
	slope=0.25*2*M_PI*cos(2*M_PI*x+global.waterM);
	return slope;
}

float getRotateDegree(float x)
{
	float slope;
	float degree;
	float x1,y1;
	float y;

	y=getSineY(x);
	slope=getSineSlope(x);

	x1=x;


  	x1+=0.01;
  	y1=slope*(x1-x)+y;

  	y1-=y;
  	x1-=x;

  	degree=atan2(y1,x1);

  	return degree;
}

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
    y=0.25*sin(2*M_PI*x+global.waterM);
    slope=getSineSlope(x);
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
    y=getSineY(x);
    slope=getSineSlope(x);
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

void displayOSD()
{
  char buffer[30];
  char *bufp;
  int w, h;

  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  /* Set up orthographic coordinate system to match the
     window, i.e. (0,0)-(w,h) */
  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);
  glOrtho(0.0, w, 0.0, h, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  /* Frame rate */
  glColor3f(1.0, 1.0, 0.0);
  glRasterPos2i(250, 380);
  snprintf(buffer, sizeof buffer, "fr (f/s): %6.0f", global.frameRate);
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

  /* Time per frame */
  glColor3f(1.0, 1.0, 0.0);
  glRasterPos2i(250, 360);
  snprintf(buffer, sizeof buffer, "ft (ms/f): %5.0f", 1.0 / global.frameRate * 1000.0);
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

  /* tess */
  glColor3f(1.0, 1.0, 0.0);
  glRasterPos2i(250, 340);
  snprintf(buffer, sizeof buffer, "tess: %10.0f", 1.0 / global.frameRate * 1000.0);
  for (bufp = buffer; *bufp; bufp++)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

  /* Pop modelview */
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);

  /* Pop projection */
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  /* Pop attributes */
  glPopAttrib();
}

void displayWater()
{
	 float y;
	 drawAxes(10.0);
	 glBegin(GL_QUAD_STRIP);
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


	  glColor4f(0.0, 1.0, 1.0,0.5);//light blue color with transparency
	  float left=-1.0;
	  float right=1.0;
	  float range=2;
    int seg=1000;
    float stepSize=range/seg;

    for (float x = left; x <= right; x+=stepSize)
		{
	      y=getSineY(x);
	      glVertex3f(x, y, 0);

	      if(!global.debug_water)
	      {
    		  glVertex3f(x,-1,0);
	      }
    }
	  glEnd();

	  float tx,ty;
	  if (global.debug_tangent==true)
	  {
		  for (float x = left; x <= right; x+=0.05)
		  {
	      y=getSineY(x);
		    drawTan(x,y,0.1,1,0,0);
	  	}
	  }

	if (global.debug_normal==true)
	{
		for (float x = left; x <= right; x+=0.05)
		{
			y=getSineY(x);
		  drawNormal(x,y,0.05,0,1,0);
		}
	}
}

void drawLeftBoat(float l, float h)
{

	float rotateDegree=getRotateDegree(global.lmove);
	drawAxes(10.0);
  /*boatTop*/
	glPushMatrix();
	glTranslatef(global.lmove,getSineY(global.lmove),0.0);
	glRotatef((180/M_PI)*rotateDegree,0.0,0.0,1.0);
	drawAxes(0.5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.025, 0.025,0.0);
	glVertex3f(0.025, 0.025,0.0);
	glVertex3f(0.025, 0.075,0.0);
	glVertex3f(-0.025, 0.075,0.0);
	glEnd();

  /* boat bottom*/
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.1,0.025,0.0);
	glVertex3f(0.05,-0.025,0.0);
	glVertex3f(-0.05,-0.025,0.0);
	glVertex3f(-0.1,0.025,0.0);
	glEnd();

  /*boat cannon*/
	glPushMatrix();
	glRotatef(global.lrotate,0,0,1.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.1,0.005,0.0);
	glVertex3f(0.1,-0.005,0.0);
	glVertex3f(0,-0.005,0.0);
	glVertex3f(0,0.005,0.0);
	glEnd();

	glPopMatrix();
	glPopMatrix();
}

void drawRightBoat(float l, float h)
{
	 float RBoatUpdate;
	 float rotateDegree=getRotateDegree(global.rmove);
	 drawAxes(10.0);
	 /*boat top*/
	 glPushMatrix();
	 glTranslatef(global.rmove,getSineY(global.rmove),0.0);
	 glRotatef((180/M_PI)*rotateDegree,0.0,0.0,1.0);
	 drawAxes(0.5);
	 glColor3f(0.0, 0.0, 1.0);
	 glBegin(GL_POLYGON);
	 glVertex3f(-0.025, 0.025,0.0);
	 glVertex3f(0.025, 0.025,0.0);
	 glVertex3f(0.025, 0.075,0.0);
	 glVertex3f(-0.025, 0.075,0.0);
	 glEnd();

	 /*boat bottom*/
	 glColor3f(0.0, 0.0, 1.0);
	 glBegin(GL_POLYGON);
	 glVertex3f(0.1,0.025,0.0);
	 glVertex3f(0.05,-0.025,0.0);
	 glVertex3f(-0.05,-0.025,0.0);
	 glVertex3f(-0.1,0.025,0.0);
	 glEnd();

     /* boat cannon*/
	 glPushMatrix();
	 glRotatef(global.rrotate,0,0,1.0);
	 glBegin(GL_POLYGON);
	 glVertex3f(0.1,0.005,0.0);
	 glVertex3f(0.1,-0.005,0.0);
	 glVertex3f(0,-0.005,0.0);
	 glVertex3f(0,0.005,0.0);
	 glEnd();

	 glPopMatrix();
	 glPopMatrix();
}

void drawIsand()
{
	   drawAxes(10.0);
	   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	   /*island body*/
	   glPushMatrix();
	   glBegin(GL_POLYGON);
	   glColor4f(1.0,1.0,0.0,0.7);
	   glVertex2f(-0.2,-2);
	   glVertex2f(0.2, -2);
	   glVertex2f(0.2, 0.25);
	   glVertex2f(-0.2, 0.25);
	   glEnd();
	   /*island cannon*/
	   glTranslatef(0.0, 0.25, 0.0);
	   glRotatef(global.islandCannon, 0.0, 0.0, 1.0);
	   glPushMatrix();
	   glColor4f(1.0, 1.0, 0.0,0.7);
	   glBegin(GL_POLYGON);
	   glVertex2f(0, 0);
	   glVertex2f(0.25, 0);
	   glVertex2f(0.25, 0.1);
	   glVertex2f(0, 0.1);
	   glEnd();
	   /*misslie*/
	   glPushMatrix();
	   glTranslatef(0.0, 0.05, 0.0);//missile position
	   glColor3f(1.0,1.0,1.0);
	   glutWireSphere(0.05, 16, 10);//missile
	   glPopMatrix();
	   glPopMatrix();
	   glPopMatrix();
}

void display()
{

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	/* Draw something here */

  drawLeftBoat(1.0,1.0);
  drawRightBoat(1.0,1.0);
  displayWater();
  displayOSD();
  drawIsand();
  gluErrorString(glGetError());
  glutSwapBuffers();
  global.frameRate++;
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
}

void keyboardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(EXIT_SUCCESS);
		break;
		/*control water motion*/
	case '`':
		if (global.waterM_bool==false)
		{
			global.waterM_bool=true;
		}
		else
		{
			global.waterM_bool=false;
		}
		break;
		/*control wireframe*/
	case 'w':
		if (global.debug_water==false)
		{
			global.debug_water=true;
		}
		else
		{
			global.debug_water=false;
		}
		break;
		/*normal vector for water*/
	case 'n':
		if (global.debug_normal==false)
		{
			global.debug_normal=true;
		}
		else
		{
			global.debug_normal=false;
		}
		break;
		/*tangent vector for water*/
	case 't':
		if (global.debug_tangent==false)
		{
			global.debug_tangent=true;
		}
		else
		{
			global.debug_tangent=false;
		}
		break;
		/*control left boat moves left*/
  case 'a':
	   if(global.lmove > -0.9)
		 {
			 global.lmove -= 0.1;
		 }
		 break;
		 /*control left boat moves right*/
	case 'd':
	   if(global.lmove < -0.35)
		 {
			  global.lmove += 0.1;
		 }
		 break;
		 /*control right boat moves left*/
	case 'j':
	   if(global.rmove > 0.35)
		 {
			  global.rmove -=0.1;
		 }
		 break;
		 /*control right boat moves right*/
	case 'l':
	   if(global.rmove < 0.9)
		 {
			  global.rmove += 0.1;
		 }
		 break;
		 /*control left boat's cannon rotates anti clockwise*/
	case 'q':
		if (global.lrotate<180)
		{
			global.lrotate+=5;
		}
		break;
		/*control left boat's cannon rotates clockwise*/
	case 'Q':
		if (global.lrotate>0)
		{
			global.lrotate-=5;
		}
		break;
		/*contol right boat's cannon rotates clockwise*/
	case 'o':
		if (global.rrotate>0)
		{
			global.rrotate-=5;
		}
		break;
		/*control right boat's cannon rotates anti clockwise*/
	case 'O':
		if (global.rrotate<180)
		{
			global.rrotate+=5;
		}
		break;
		/*control island cannon rotates anti clockwise*/
    case 'i':
	  if (global.islandCannon < 147)
       {
	     global.islandCannon += 1;
       }
    break;
		/*control island cannon rotates clockwise*/
	case 'I':
	  if (global.islandCannon > 0)
	  {
		   global.islandCannon -= 1;
	  }
	  break;
	default:
		break;
	}
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
	//glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(updateWater);
	myinit();
	glutMainLoop();
}
