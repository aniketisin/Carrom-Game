#include <cmath>
#include <GL/glut.h>
#include<utility>
#include<iostream>
using namespace std;


#define mp make_pair
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
#define QUEEN 0
#define WHITE 1

typedef float ff;

//Coins...Striker
ff mass[4] = {10, 10, 10, 20};

//Queen, White, Black, Striker
ff color[4][3] = {{1,0,0},{0,1,0},{0,0,0},{0,0,1}};

ff power = 0;
ff maxPower = 20;
int timer = 0,score = 30,myCoin = -1;
int rate = 6;
bool play = 1,mousePressed = 0,welcome = 1,gameOver = 0,disabled = 0,replay = 0;
bool mark[11][11];
bool reMark[11][11];
void drawScene();
void update(int value);
void drawBox(float len);
void drawBall(float rad);
void drawTriangle();
void initRendering();
void welcomeRendering(string s);
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void drawCircle(ff rad, pair<ff,ff> pos,int type, ff r, ff g, ff b);
void shotPower();
void clock(int value);
void drawClock(int t);
void passiveMotion(int x, int y);
void printtext(ff x, ff y, string String)
{
	//(x,y) is from the bottom left of the window
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1,0,0);
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int WindowWidth = w * 2 / 3;
	int WindowHeight = h * 2 / 3;
	ff scalex = 5.9;
	ff scaley = 3.3;
	x+=scalex;
	y+=scaley;
	x*=1.0*WindowWidth/11.8;
	y*=1.0*WindowHeight/6.6;	
	glOrtho(0, WindowWidth, 0, WindowHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glRasterPos2i(x,y);
	for (int i=0; i<String.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, String[i]);
	}
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
