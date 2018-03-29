#include <stdlib.h>
#include <stdio.h>

#include <GL/gl.h>                                              
#include <GL/glu.h>
#include <GL/glut.h>

void display()                                                  
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         

    /* Draw something here */

    glutSwapBuffers();                                          
}

int main(int argc, char **argv)                                 
{
    glutInit(&argc, argv);                                      
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Assignment 1");                             
    glutDisplayFunc(display);                                   
    glutMainLoop();                                             
}
