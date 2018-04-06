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


float lmove = 0.0;
float rmove = 0.0;
int rotate = 0;
void drawAxes(float length)
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(length, 0, 0);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, length, 0);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, length);
	glEnd();
}


void myWireRect(float l, float h)
{
	drawAxes(0.5);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glScalef(l, h, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.5, -0.5);
	glVertex2f(0.5, -0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(-0.5, 0.5);
	glEnd();
	glPopMatrix();
}
void myWireRhombus(float l, float h)
{
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glScalef(l, h, 1.0);
	glScalef(1.0 / sqrt(2.0), 1.0 / sqrt(2.0), 1.0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	myWireRect(0.5, 0.5);
	glPopMatrix();
}

void drawLeftBoat(float l, float h)
{

  glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	drawAxes(10.0);
  /*boatTop*/
	glPushMatrix();
	glTranslatef(-0.8,0.0,0.0);
	glTranslatef(lmove,0.0,0.0);
	drawAxes(0.5);
	glRotatef((GLfloat)rotate,0.0,0.0,1.0);
	drawAxes(0.5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	//glScalef(0.8f,0.8f,0.8f);
	glVertex3f(-0.25, 0.25,0.0);
	glVertex3f(0.25, 0.25,0.0);
	glVertex3f(0.25, 0.5,0.0);
	glVertex3f(-0.25, 0.5,0.0);
	glEnd();
	//glutWireCube (1.0);

  /* boat bottom*/
	glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINE_LOOP);
  glVertex2f(-0.35,-0.15);
  glVertex2f(0.35,-0.15);
  glVertex2f(0.5,0.25);
  glVertex2f(-0.5,0.25);
  glEnd();
	/*boat cannon*/

  glPopMatrix();
	glPopMatrix();
}

void drawRightBoat(float l, float h)
{
	drawAxes(10.0);
	glPushMatrix();
	glTranslatef(0.8,0.0,0.0);
	glTranslatef(rmove,0.0,0.0);
	glRotatef(0.0,0.0,0.0,1.0);

	drawAxes(0.5);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glScalef(l, h, 1.0);
	glScalef (0.2,0.2,0.2);
	glVertex2f(-0.25, 0.25);
	glVertex2f(0.25, 0.25);
	glVertex2f(0.25, 0.5);
	glVertex2f(-0.25, 0.5);
	glEnd();
  glPopMatrix();
}

void display()
{
	/* Put drawing code here*/
	//myWireRect(0.05,0.125);
	//myWireRhombus(0.5, 0.6);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawLeftBoat(1.0,1.0);
	drawRightBoat(1.0,1.0);
	glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -6.0);
}

/* You can ignore this for now, it just lets you exit when you press 'q' or ESC */
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

  case 'd':
	   glutPostRedisplay();
	   lmove += 0.2;
		 glutPostRedisplay();
     break;
	case 'a':
	    glutPostRedisplay();
			lmove-=0.2;
			glutPostRedisplay();
			break;
	case 'j':
	    glutPostRedisplay();
	    rmove -= 0.2;
			break;
	case 'l':
	    glutPostRedisplay();
			rmove +=0.2;
			break;
	case 'r':
   	 glutPostRedisplay();
	   rotate = (rotate + 5)%360;
		 glutPostRedisplay();
	   break;
	case 'R':
	    glutPostRedisplay();
	    rotate = (rotate -5)%360;
			glutPostRedisplay();
	    break;
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
	glutCreateWindow("exercise");
	init();
  //glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}
