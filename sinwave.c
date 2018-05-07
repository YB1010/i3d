/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 * robot.c
 * This program shows how to composite modeling transformations
 * to draw translated and rotated hierarchical models.
 * Interaction:  pressing the s and e keys (shoulder and elbow)
 * alters the rotation of the robot arm.
 */

/*
 * Extended example:
 *    - mouse for interaction
 *    - glut menus
 *    - variables to control debugging output
 *
 *  Geoff Leach
 *  3/4/2007
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef enum { debug_keyboard,  debug_special, debug_motion } debug_flags;
bool debug[] = { true, true, true, true };
   // Create light components

static int shoulder = 0, elbow = 0;
static float lastX = 0, lastY = 0;
static float rotateX = 0.0, rotateY = 0.0;
static bool lighting = false;
static float zoom = 0.5;

typedef enum { solid, wireframe } renderMode_t;
renderMode_t renderMode = solid;

void init(void) 
{
  GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
  GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
  GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_SMOOTH);
// Somewhere in the initialization part of your program…

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

    // Assign created components to GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}


void drawAxes(float length)
{

  glPushAttrib(GL_CURRENT_BIT);

  glBegin(GL_LINES);

  /* x axis */
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(length, 0.0, 0.0);

  /* y axis */
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, length, 0.0);

  /* z axis */
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, length);

  glEnd();

  glPopAttrib();
}

void water()
{
    float y;
    float x;


    printf("display\n");



    drawAxes(1.0);
    glBegin(GL_LINE_STRIP);
    glColor3f(1,1,1);
    float left=-1.0;
    float right=1.0;
    float range=right-left;
    int seg=100;
    float stepSize=range/seg;

    for (float x = left; x <= right; x+=stepSize) {
      float a,b;

      y=sin(M_PI*x);
      

      a=x;
      
      for (; y < right;y+=stepSize)
      {
        float z;
        z=sin(M_PI*y);
        glVertex3f(x, y, z);
      }

      

    }
    glEnd();
}

void drawVector(float x, float y,float z,float a, float b,float c, float s, bool normalize, float red, float green, float blue)
{
  glPushAttrib(GL_CURRENT_BIT);
  glBegin(GL_LINES);
  glColor3f(red,green,blue);
  glVertex3f(x,y,z);
  if (normalize==true)
  {
    float v=sqrt((a*a)+(c*c));
    float v2=sqrt((v*v)+(b*b));
    a=a/v2;
    b=b/v2;
    c=c/v2;
    a*=s;
    b*=s;
    c*=s;
    glVertex3f(x+a,y+b,z+c);
  }
  else
  {
    a*=s;
    b*=s;
    c*=s;
    glVertex3f(x+a,y+b,z+c);
  }

  glEnd();
  glPopAttrib();
}
void display(void)
{

  glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  glDisable (GL_LIGHTING);
  /* Global transformations */
  glScalef(zoom,zoom,zoom);
  glTranslatef (-1.0, 0.0, 0.0);
  glRotatef (rotateX, 1.0, 0.0, 0.0);
  glRotatef (rotateY, 0.0, 1.0, 0.0);

  /* Global coordinate system */
  drawAxes(10.0);
  float n=8;
  float x,y,z;

  float xStep = 2.0 / n;                
  float zStep = 2.0 / n; // xStep and zStep are the same, but could be different
          for (int j = 0; j < n; j++) {
            glBegin(GL_QUAD_STRIP);
            z = -1.0 + j * zStep;
            for (int i = 0; i <= n; i++) {
              /* What are these calculations doing? */
              x = -1.0 + i * xStep;

              y = 0.25*sin(4 *x);
              glVertex3f(x,y,z);

              // Replace this comment with calculation to work out next z value
              
              glVertex3f(x,y,z+zStep);
            }
            glEnd();                 
          }
          for (int j = 0; j < n; j++) {
          
            z = -1.0 + j * zStep;
            for (int i = 0; i <= n; i++) {
              /* What are these calculations doing? */
              x = -1.0 + i * xStep;
          
              // Replace this comment with calculation to work out next z value
              drawVector(x,0,z,0,1,0,1,true,1,1,0);
          
            }
                        
          }

 // glEnable (GL_LIGHTING);

  /* Upper arm */

  glPopMatrix();
  glFlush();
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
  glTranslatef (0.0, 0.0, -5.0);
}

bool leftone =false;
bool rightone = false;

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    lastX = x;
    lastY = y;
    leftone = true;
    rightone =false;
  }

  else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
    
    lastY = y;
    leftone = false;
    rightone =true;
  }
 
}

void motion(int x, int y)
{
  float dx, dy;

  if (debug[debug_motion]) {
    printf("motion: x y %d %d\n", x, y);
    printf("motion: rotateX rotateY %f %f\n", rotateX, rotateY);
  }

  if (rightone)
  {
    dy = y - lastY;
    zoom+=dy/100;

    lastY=y;

  }
  if (leftone)
  {
      dx = x - lastX;
      dy = y - lastY;


      rotateX += dy;
      rotateY += dx;

      lastX = x;
      lastY = y;
  }


  glutPostRedisplay();
}

void special (int key, int x, int y)
{
  if (debug[debug_special])
    printf("special: key = %d\n", key);
}

/* ARGSUSED1 */
void keyboard (unsigned char key, int x, int y)
{
  
  if (debug[debug_keyboard]) 
    printf("keyboard: key = %d\n", key);

  switch (key) {
  case 's':
    shoulder = (shoulder + 5) % 360;
    glutPostRedisplay();
    break;
  case 'S':
    shoulder = (shoulder - 5) % 360;
    glutPostRedisplay();
    break;
  case 'e':
    elbow = (elbow + 5) % 360;
    glutPostRedisplay();
    break;
  case 'E':
    elbow = (elbow - 5) % 360;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}

void menu(int value)
{
    switch (value) {
    case 0:
      lighting = false;
      break;
    case 1:
      lighting = true;
      break;
    case 2:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      renderMode = wireframe;
      break;
    case 3:
      renderMode = solid;
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    default:
	break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{

  /* Create window and initialise */
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500); 
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init ();

  /* Register callbacks */
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  glutKeyboardUpFunc(keyboard);
  glutSpecialUpFunc(special);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  /* Create menus */
   glutCreateMenu(menu);
   glutAddMenuEntry("Lighting off", 0);
   glutAddMenuEntry("Lighting on", 1);
   glutAddMenuEntry("Wireframe", 2);
   glutAddMenuEntry("Solid", 3);
   glutAddMenuEntry("Flat", 4);
 glutAddMenuEntry("Smooth", 5);
 glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return 0;
}
