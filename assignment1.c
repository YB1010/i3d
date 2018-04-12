// assignment1.cpp : Defines the entry point for the console application.
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <sys/time.h>

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
#define WATER_SPEED 0.01;
#endif

#define STRENGTH 25.0
#define BOAT_M_CD 0
//#define ISLAND_M_CD		//value of Missile's speed
#define LBOAT_M_NUM 100		//max Missile number
#define L_MISSILE 1			//
#define RBOAT_M_NUM 100
#define R_MISSILE 2
#define ISLAND_M_NUM 100
#define ISLAND_MISSILE 0
#define BOAT_M_P 0.1

typedef enum { tangent, normal }visualisation;



typedef struct
{
	float x,y;
}vec2f;

typedef struct
{
	vec2f r,v;
	bool shooted;
	int type;
	int damage;
} Missile;




Missile islandMissile[ISLAND_M_NUM];
Missile lboatMissile[LBOAT_M_NUM];
Missile rboatMissile[RBOAT_M_NUM];



typedef struct
 {
 	int tess;
    float waterM;
	bool iCannonMove;
    bool rCannonMove;
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
    float dt;
    float t;
    float lastT;

} global_t;
  global_t global={
  	1024,0,false,false,true,false,false,false,30.0,-0.5,0.5,30.0,150.0,0,0,0,0,0,0.2,0,0,0,-1
  };

typedef struct
{
	float life;
	bool alive;
	//int x1;
	//int x2;
  float lastFiringTime;
}boat;
boat leftBoat = {10,true,-(BOAT_M_CD)};
boat rightBoat = {10, true,-(BOAT_M_CD)};

boat island = {100,true,-(BOAT_M_CD)};

typedef struct
{
	bool alive;

}cannon;


const int milli = 1000;
const float g = -4.9;


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

void healthBar(float r, float g, float b, float length)
{
	glColor3f(r,g,b);
	glBegin(GL_POLYGON);
	glVertex2f(0,0.0);
	glVertex2f(length,0.0);
	glVertex2f(length,0.1);
	glVertex2f(0,0.1);
	glEnd();
}

//health for island and boats
void islandHealth()
{
	if(island.life == 0)
	{
		island.alive = false;
	}
	else
	{
	    island.alive = true;
	}

	if(island.alive)
	{
		glPushMatrix();
		glTranslatef(-0.9f,0.7f,0.0f);
		glScalef(0.5f,0.5f,0.0f);
		healthBar(1.0,1.0,0.0,(island.life)*0.01);
		glPopMatrix();
	}
}
	   /* if()//be hitted
	    {
		   island.life-=1;
		   glBegin(GL_POLYGON);
	       glColor3f(1.0,1.0,0.0);
	       glVertex2f(-0.5,0.0);
	       glVertex2f(island.x1-=0.1,0.0);
	       glVertex2f(island.x2-=0.1,0.1);
	       glVertex2f(-0.5,0.1);
	       glEnd();
		  //island.lifeMinus += 1;
	    }
	}
	else//died
	{
		char buffer[30];
		snprintf(buffer, sizeof buffer,"Game Over\n");
		global.waterM_bool=false;
	}
}
*/

void lboatHealth()
{
	drawAxes(10.0);
    if(leftBoat.life == 0)
	{
		leftBoat.alive = false;
	}
	else
	{
		leftBoat.alive = true;
	}

	if(leftBoat.alive)
	{
      glPushMatrix();
			glTranslatef(-0.9f,0.9f,0.0f);
			glScalef(0.5f,0.5f,0.0f);
			healthBar(1.0,0.0,0.0,(leftBoat.life)*0.1);
			glPopMatrix();
		}
	}
  /*  if(collionsion())//be hitted
		{
		    leftBoat.life-=1;
		}


	else//died
	{
		char buffer[30];
		snprintf(buffer, sizeof buffer,"Game Over\n");
		global.waterM_bool=false;
	}
}*/

void rboatHealth()
{
	if(rightBoat.life==0)
	{
		rightBoat.alive = false;
	}
	else if(rightBoat.life>0)
	{
		rightBoat.alive = true;
	}
	if(rightBoat.alive)
	{
		glPushMatrix();
		glTranslatef(-0.9f,0.8f,0.0f);
		glScalef(0.5f,0.5f,0.0f);
		healthBar(0.0,0.0,1.0,(rightBoat.life)*0.1);
		glPopMatrix();
	}
}
/*
	if(rightBoat.alive)
	{
		if()//be hitted
		{
			rightBoat.life-=1;
		}
	}
	else//died
	{
		char buffer[30];
		snprintf(buffer, sizeof buffer,"Game Over\n");
		global.waterM_bool=false;
	}
}*/

void MissileInit()
{
	for (int i = 0; i < LBOAT_M_NUM; i++)
	{
		lboatMissile[i].r.x=0;
		lboatMissile[i].r.y=0;
		lboatMissile[i].v.x=0;
		lboatMissile[i].v.y=0;
		lboatMissile[i].type=L_MISSILE;
		lboatMissile[i].shooted=false;
		lboatMissile[i].damage=0;
	}

	for (int i = 0; i < ISLAND_M_NUM; i++)
	{
		islandMissile[i].r.x=0;
		islandMissile[i].r.y=0;
		islandMissile[i].v.x=0;
		islandMissile[i].v.y=0;
		islandMissile[i].type=ISLAND_MISSILE;
		islandMissile[i].shooted=false;
		islandMissile[i].damage=0;
	}
	for (int i = 0; i < RBOAT_M_NUM; i++)
	{
		rboatMissile[i].r.x=0;
		rboatMissile[i].r.y=0;
		rboatMissile[i].v.x=0;
		rboatMissile[i].v.y=0;
		rboatMissile[i].type=R_MISSILE;
		rboatMissile[i].shooted=false;
		rboatMissile[i].damage=0;
	}

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

int a,b,c; //for printing test.
bool collionsion(Missile aim)
{
	if (aim.shooted)
	{
		float ly;
		float lx;
		float rx;
		float ry;

		lx=global.lmove;
		ly=getSineY(lx);


		rx=global.rmove;
		ry=getSineY(rx);

		float rdistance = sqrt(((rx-aim.r.x)*(rx-aim.r.x))+((ry-aim.r.y)*(ry-aim.r.y)));


		float ldistance = sqrt(((lx-aim.r.x)*(lx-aim.r.x))+((ly-aim.r.y)*(ly-aim.r.y)));


		if (aim.r.y<=0.25 && aim.r.x<=0.2 && aim.r.x>=-0.2)
		{

			island.life -= aim.damage;
			printf("hitting island %i\n",a++);
			return false;
		}
		if (ldistance<=0.1 && aim.r.x<0)
		{
			printf("hitting lboat %i\n",b++);

			leftBoat.life -= aim.damage;
			return false;
		}
		if (rdistance<=0.1 && aim.r.x>0)
		{

			rightBoat.life -= aim.damage;

			printf("hitting rboat %i\n",c++);
			return false;
		}


			if (aim.r.y<getSineY(aim.r.x))
			{
				return false;
			}



		return aim.shooted;
	}
	else
	{

		return aim.shooted;
	}

}
void UpdateMissile(Missile *object)
{
	if (object[0].type == 0)
	{
		for (int i = 0; i < ISLAND_M_NUM; i++)
		{
			if (!(object[i].shooted))
			{
				object[i].r.y=0.25+0.15*sin((M_PI/180)*(global.islandCannon));
				//0.25 IS FROM THE ISLAND BODY
				object[i].r.x=0.0+0.15*cos((M_PI/180)*(global.islandCannon));

			   	object[i].v.y=15*(object[i].r.y-0.25);
			   	object[i].v.x=15*(object[i].r.x);
			}
				if ((object[i].shooted=collionsion(object[i]))==true)
				{
					object[i].damage=1;
				}
		}

	}

	if (object[0].type == 2)
	{
		for (int i = 0; i < RBOAT_M_NUM; i++)
		{
			if (!object[i].shooted)
			{

				object[i].r.y=getSineY(global.rmove)+sin(getRotateDegree(global.rmove)+((M_PI/180)*global.rrotate))*0.1;
					//reverse the previsou translatef and rotates.
				object[i].r.x=global.rmove+cos(getRotateDegree(global.rmove)+((M_PI/180)*global.rrotate))*BOAT_M_P;

				object[i].v.y=STRENGTH*(-getSineY(global.rmove)+object[i].r.y);//get the distance between the cannon end and the center of boat.
				object[i].v.x=STRENGTH*(-global.rmove+object[i].r.x);


			}

			if ((object[i].shooted=collionsion(object[i]))==true)
			{
			
				object[i].damage=1;
			}
		}

	}
	if (object[0].type == 1)
	{
		for (int i = 0; i < LBOAT_M_NUM; i++)
		{
			if (!(object[i].shooted))
			{

				object[i].r.y=getSineY(global.lmove)+sin(getRotateDegree(global.lmove)+((M_PI/180)*global.lrotate))*0.1;
					//reverse the previsou translatef and rotates.
				object[i].r.x=global.lmove+cos(getRotateDegree(global.lmove)+((M_PI/180)*global.lrotate)) * 0.1;

				object[i].v.y=STRENGTH*(-getSineY(global.lmove)+object[i].r.y);//get the distance between the cannon end and the center of boat.
				object[i].v.x=STRENGTH*(-global.lmove+object[i].r.x);


			}

			if ((object[i].shooted=collionsion(object[i]))==true)
			{
				object[i].damage=1;
			}
		}

	}
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
	glBegin(GL_QUAD_STRIP);
	glColor4f(0.0, 1.0, 1.0,0.5);//light blue color with transparency
	float left=-1.0;
	float right=1.0;
	float range=2;
    int seg=global.tess;
    float stepSize=range/seg;

    for (float x = left; x <= right; x+=stepSize)
	{
		y=getSineY(x);
		glVertex3f(x, y, 0);
		if (global.debug_water==false)
    		glVertex3f(x,-1,0);


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


	UpdateMissile(lboatMissile);

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

		UpdateMissile(rboatMissile);

}

void drawIsand()
{
	drawAxes(10.0);
	/*island body*/
	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor4f(1.0,1.0,0.0,1);
	glVertex2f(-0.2,-2);
	glVertex2f(0.2, -2);
	glVertex2f(0.2, 0.25);
	glVertex2f(-0.2, 0.25);
	glEnd();
	/*island cannon*/
	glTranslatef(0.0, 0.25, 0.0);
	glRotatef(global.islandCannon, 0.0, 0.0, 1.0);



	glPushMatrix();
	glColor4f(1.0, 1.0, 0.0,7);
	glBegin(GL_POLYGON);
	glVertex2f(0, -0.025);
	glVertex2f(0.2, -0.025);
	glVertex2f(0.2, 0.025);
	glVertex2f(0, 0.025);
	glEnd();
	glPopMatrix();
	glPopMatrix();

	UpdateMissile(islandMissile);



}

void displayPrediction(Missile object)
{

	if (object.shooted)
	{
		float t=0;
		glBegin(GL_LINE_STRIP);
		glColor3f(1,1,1);

		object.damage=0;
		while(collionsion(object))
		{

			glVertex2f(object.r.x,object.r.y);

			object.r.x += object.v.x * t;
			object.r.y += object.v.y * t;
			object.v.y += g * t;



			t+=0.0001;


		}
		glEnd();
	}


}

void projectile2DMotion(Missile *object,float dt)
{
	if (object[0].type==1)
	{
		for (int i = 0; i < LBOAT_M_NUM; i++)
		{

			object[i].r.x += object[i].v.x * dt ;
			object[i].r.y += object[i].v.y * dt ;
			object[i].v.y += g * dt;
		}
	}
	if (object[0].type==2)
	{
		for (int i = 0; i < RBOAT_M_NUM; i++)
		{

			object[i].r.x += object[i].v.x * dt;
			object[i].r.y += object[i].v.y * dt;
			object[i].v.y += g * dt;
		}
	}
	if (object[0].type==0)
	{
		for (int i = 0; i < ISLAND_M_NUM; i++)
		{

			object[i].r.x += object[i].v.x * dt;
			object[i].r.y += object[i].v.y * dt;
			object[i].v.y += g * dt;
		}
	}


}

void displayIslandMissile(Missile *object)
{
	for (int i = 0; i < ISLAND_M_NUM; i++)
	{
		if (object[i].shooted)
		{
			glPushMatrix();
			glTranslatef(object[i].r.x, object[i].r.y, 0.0);
			//glRotatef(global.islandCannon, 0.0, 0.0, 1.0);
			glColor3f(1.0,1.0,1.0);
			glutWireSphere(0.01, 16, 10);
			glPopMatrix();


		}
	}

}

void displayLboatMissile(Missile *object)
{
	for (int i = 0; i < LBOAT_M_NUM; i++)
	{
		if (object[i].shooted)
		{
			glPushMatrix();
			glTranslatef(object[i].r.x, object[i].r.y, 0.0);
			//glRotatef(global.islandCannon, 0.0, 0.0, 1.0);
			glColor3f(1.0,1.0,1.0);
			glutWireSphere(0.005, 16, 10);
			glPopMatrix();


		}
	}

}
void displayRboatMissile(Missile *object)
{
	for (int i = 0; i < RBOAT_M_NUM; i++)
	{
		if (object[i].shooted)
		{
			glPushMatrix();
			glTranslatef(object[i].r.x, object[i].r.y, 0.0);
			//glRotatef(global.islandCannon, 0.0, 0.0, 1.0);
			glColor3f(1.0,1.0,1.0);
			glutWireSphere(0.005, 16, 10);
			glPopMatrix();


		}
	}

}

void update()
{
    /*update water*/
	if (global.waterM_bool==true)
	{
		global.waterM+=WATER_SPEED;
	}
    glutPostRedisplay();
  /*update island missle*/


    global.t = (glutGet(GLUT_ELAPSED_TIME) / (float)milli) - global.startTime;

    if (global.lastT < 0.0)
    {
	   global.lastT = global.t;
	   return;
    }
    global.dt = global.t - global.lastT;
    if (global.dt < 0)
    {
    	global.lastT = global.t;
    	return;
    }

 /*if (global.debug)
	 printf("%f %f\n", t, dt);*/

	projectile2DMotion(islandMissile,global.dt);
	projectile2DMotion(rboatMissile,global.dt);

	projectile2DMotion(lboatMissile,global.dt);

    global.lastT = global.t;
/*problem here is the continuous firing and missile direction cannot follow the cannon direction*/
	glutPostRedisplay();
}
void display()
{


 	glEnable(GL_BLEND);
 	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 	glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	/* Draw something here */

	drawAxes(10.0);


	displayIslandMissile(islandMissile);

	displayRboatMissile(rboatMissile);

	displayLboatMissile(lboatMissile);

 	drawLeftBoat(1.0,1.0);
	lboatHealth();
	islandHealth();
 	drawRightBoat(1.0,1.0);
  rboatHealth();
 	for (int i = 0; i < LBOAT_M_NUM; i++)
 	{
 		displayPrediction(lboatMissile[i]);
 	}
	for (int i = 0; i < RBOAT_M_NUM; i++)
 	{
 		displayPrediction(rboatMissile[i]);
 	}
 	for (int i = 0; i < ISLAND_M_NUM; i++)
 	{
 		displayPrediction(islandMissile[i]);
 	}

	drawIsand();
 	displayWater();
  displayOSD();

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
	case '-':
	if (global.tess>=4)
		global.tess/=2;
		break;
	case '+':
		global.tess*=2;

		break;
	case 27:
		exit(EXIT_SUCCESS);
		break;
		/*control water motion*/
	case '`':
		if (global.waterM_bool==false)
			global.waterM_bool=true;
		else
			global.waterM_bool=false;
		break;
		/*control wireframe*/
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		global.debug_water=true;
		break;
		/*normal vector for water*/

	case 'p':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		global.debug_water=false;
		break;
	case 'n':
		if (global.debug_normal==false){
			global.debug_normal=true;
		}else{
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
    case 'f':
	   if (global.islandCannon < 180)
       {
	     global.islandCannon += 1;
       }
    break;
		/*control island cannon rotates clockwise*/
	case 'h':
	  if (global.islandCannon > 0)
	  {
		   global.islandCannon -= 1;
	  }
	  break;
	    /*firing island cannon*/
	case ' ':
		for (int i = 0; i < ISLAND_M_NUM; i++)
		{
			if (!islandMissile[i].shooted)
	 		{
				global.startTime = glutGet(GLUT_ELAPSED_TIME) / (float)milli;

				islandMissile[i].shooted = true;
				break;
			}
		}
		break;
	case 'e':

		if (((glutGet(GLUT_ELAPSED_TIME)/(float)milli)-(leftBoat.lastFiringTime))<=BOAT_M_CD)
			break;


		for (int i = 0; i < LBOAT_M_NUM; i++)
		{
			if (!lboatMissile[i].shooted)
			{

				global.startTime = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
				leftBoat.lastFiringTime=global.startTime;

				lboatMissile[i].shooted = true;
				break;
			}
		}
		break;

	case 'i':

		if (((glutGet(GLUT_ELAPSED_TIME)/(float)milli)-rightBoat.lastFiringTime)<=BOAT_M_CD)
			break;

		for (int i = 0; i < RBOAT_M_NUM; i++)
		{
			if (!rboatMissile[i].shooted)
			{
				global.startTime = glutGet(GLUT_ELAPSED_TIME) / (float)milli;
				rightBoat.lastFiringTime=global.startTime;
				rboatMissile[i].shooted = true;
				break;
			}
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
	MissileInit();
	glutKeyboardFunc(keyboardCB);
	//glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(update);
	
	glutMainLoop();
}
