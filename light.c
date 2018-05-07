#include <stdlib.h>
#include <stdio.h>
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
static int lastX = 0, lastY = 0;
static float rotateX = 0.0, rotateY = 0.0;
typedef enum {debug_motion} debug_flags;
bool debug[] = {true};
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
void init()
{
    /* In this program these OpenGL calls only need to be done once,
      but normally they would go elsewhere, e.g. display */
      glClearColor(0.0,0.0,0.0,0.0);
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat mat_ambient[] = {1.0,0.0,0.0,1.0};
    GLfloat mat_diffuse[] = {1.0,0.0,0.0,1.0};
    GLfloat mat_shininess[] = { 100.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawAxes(10.0);
    // glLoadIdentity ();
    // gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // glScalef (1.0, 2.0, 1.0);
    glutSolidSphere(1, 8, 8);

    // glutSolidTorus(0.25, 1.0, 8, 8);
    glFlush();
    /* Put drawing code here */
    glutSwapBuffers();

}


void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
//    void glOrtho(GLdouble left, GLdouble right, GLdouble bottom,
// GLdouble top, GLdouble near, GLdouble far);
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   // glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
/*void mouse(int button, int state, int x, int y)
{
  lastX = x;
  lastY = y;
}
void motion(int x, int y)
{
  int dx, dy;

  if (debug[debug_motion]) {
    printf("motion: x y %d %d\n", x, y);
    printf("motion: rotateX rotateY %f %f\n", rotateX, rotateY);
  }

  dx = x - lastX;
  dy = y - lastY;

  rotateX += dy;
  rotateY += dx;

  lastX = x;
  lastY = y;

  glutPostRedisplay();
}*/
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



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Tutorial 7");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();

    return EXIT_SUCCESS;
}
