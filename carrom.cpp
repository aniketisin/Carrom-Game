#include "Board.h"
Board board(5.0);
Board replayBoard(5.0);

int main(int argc, char **argv) {

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w * 2 / 3;
	int windowHeight = h * 2 / 3;

	glutInitWindowSize(windowWidth, windowHeight);
	//cout<<windowWidth<<" "<<windowHeight<<"\n"; 
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);		

	glutCreateWindow("Carrom");  // Setup the window
	initRendering();

	// Register callbacks
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseclick);
	glutPassiveMotionFunc(passiveMotion);
	glutReshapeFunc(handleResize);
	glutTimerFunc(1000, clock, 0);
	glutTimerFunc(rate, update, 0);

	glutMainLoop();
	return 0; 
} 
  
// Function to draw objects on the screen
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -8.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	if(welcome || gameOver)
	{
		string s;
		if(welcome) s = "Welcome";
		if(gameOver) s = "Game Over Score " + to_string(score);
		welcomeRendering(s);
		if(timer>=2)
		{
			welcome = 0;
			if(gameOver) exit(0);
			timer = 0;
			glClearColor(0.87f, 0.89f, 0.0f, 1.0f);
		}
	}
	else
	{
		board.draw();
		shotPower();
		drawClock(timer);
		printtext(board.getLen() - 1, board.getLen()/2,"Score");
		printtext(board.getLen() - 1, board.getLen()/2 - 0.4,to_string(score).c_str());
		string s = "No Coin";
		if(myCoin==2) s = "Black";
		if(myCoin==1) s = "Green";
		printtext(board.getLen() - 1, board.getLen()/2 - 1,"Your Coin");
		printtext(board.getLen() - 1, board.getLen()/2 - 1 - 0.4,s);
	}
	glPopMatrix();
	glutSwapBuffers();
}

void clock(int value)
{
	if(play && !replay && !gameOver)
	{
		timer++;
		score--;
	}
        glutTimerFunc(1000, clock, 0);
}

// Function to handle all calculations in the scene
// updated evry 12 milliseconds
void update(int value) {

	if(!play && !welcome)
	{
		for(int i=0;i<10;i++)
		{
			if(board.coins[i].pocketed) continue;
			board.coins[i].handlePocket(board.pkts,board.pkt);
			board.coins[i].handleWall(board.getLen(),board.getCOR());
		}
		board.handleCollision();
		ff friction = board.getFriction();
		play = 1;
		replay = 0;
		for(int i=0;i<10;i++)
		{
			if(board.coins[i].pocketed) continue;
			pair<ff,ff> v = board.coins[i].getVel();
			ff mag = v.first*v.first + v.second*v.second;
			if(mag!=0)
			{
				play = 0;
				mag = sqrt(mag);
				ff xx = v.first/mag;
				ff yy = v.second/mag;
				v.first-=friction*xx;
				v.second-=friction*yy;
				mag = v.first*v.first + v.second*v.second;
				if(mag<=0.001) 
					v.first = 0,v.second = 0;
				if(mag>=0.001) play = 0; 
				board.coins[i].modifyVel(v);
			}
			pair<ff,ff> p = board.coins[i].getPos();
			p.first += v.first;
			p.second += v.second;
			board.coins[i].modifyPos(p);
		}
		if(play) 
		{
			replay = 0;
			rate = 6;
			board.coins[9].pocketed = 0;
			board.arrow.first = 1.3;
			ff p1 = -board.getLen()/2+board.getBase()+board.coins[9].getRad();
			board.coins[9].modifyPos(mp(0,p1));
		}
	}
	else
	{
		disabled = 0;
		for(int i=0;i<9;i++)
			if(board.coins[9].colliding(board.coins[i]))
			{ disabled = 1; break; }
		int cnt = 0;
		for(int i=0;i<8;i++)
			if(myCoin == board.coins[i].getType() && board.coins[i].pocketed) cnt++;
		if(board.coins[8].pocketed) cnt++;
		if(cnt==5) {gameOver = 1; timer = 0;}
			
						
	}
	if(play && mousePressed) 
	{
		power+=0.3;
		if(power>=maxPower+0.5) power=0;
	}
	glutTimerFunc(rate, update, 0);
}

void drawCircle(ff rad, pair<ff,ff> pos,int type,ff r,ff g,ff b) {

	glPushMatrix();
	glTranslatef(pos.first, pos.second, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(r, g, b);
	if(type==1)
		glBegin(GL_TRIANGLE_FAN);
	if(type==0)
		glBegin(GL_TRIANGLE_STRIP);
	for(ff i=0 ; i<360 ; i++) {
		glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
	}
	glLineWidth(1);
	glEnd();
	glPopMatrix();
}

void welcomeRendering(string s) {

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Setting a background color
	printtext(-1.2,0,s);
}

// Initializing some openGL 3D rendering options
void initRendering() {

	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	glClearColor(0.87f, 0.89f, 0.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
 
void shotPower()
{
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glColor3f(1,1,1);
	glRectf( 0.7*board.getLen(),-board.getLen()/2 , 0.7*board.getLen() + 0.3, -board.getLen()/2 + 2.5);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	for(ff i=1;i<=2*power;i++)
	{
		glColor3f(i/20,0,1-i/20);
		glRectf(0.7*board.getLen()+0.00001,-board.getLen()/2 + (i)/16, 0.7*board.getLen() + 0.3 - 0.0025, -board.getLen()/2 + (i-1)/16);
	}
	glPopMatrix();
}

void drawClock(int t)
{
	printtext(-0.7*board.getLen() - 0.65, -board.getLen()/2 + 3.1,"Time");
	printtext(-0.7*board.getLen() - 0.65, -board.getLen()/2 + 2.7,to_string(timer).c_str());

}


void handleKeypress1(unsigned char key, int x, int y) {

	if (key == 27) {
		exit(0);     // escape key is pressed
	}
	if(!play ) return;
	if (key == 's') {
		pair<ff,ff> tmp = board.coins[9].getPos();
		tmp.first+=0.1;
		if(tmp.first < board.getLen()/2 - board.getBase())
			board.coins[9].modifyPos(tmp);
	}
	if (key == 'a') {
		pair<ff,ff> tmp = board.coins[9].getPos();
		tmp.first-=0.1;
		if(tmp.first > -board.getLen()/2 + board.getBase())
			board.coins[9].modifyPos(tmp);
	}
	if(key == 'r')
	{
		for(int i=0;i<11;i++)
			for(int j=0;j<11;j++) mark[i][j] = reMark[i][j];
		replay = 1;
		rate = 40;
		play = 0;
		replayBoard.copy(board);
	}
}

void handleKeypress2(int key, int x, int y) {

	if (key == GLUT_KEY_LEFT) {board.arrow.second++;}
	if (key == GLUT_KEY_RIGHT) {board.arrow.second--;}
	/*if (key == GLUT_KEY_UP) {
		if(board.arrow.first<=2.4)
			board.arrow.first+=0.1;
	}
	if (key == GLUT_KEY_DOWN){
		if(board.arrow.first>=1.3) 
			board.arrow.first-=0.1;
	}*/
}

void handleMouseclick(int button, int state, int x, int y) {

	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
			mousePressed = 1;
		else if (button == GLUT_RIGHT_BUTTON) {;} 
	}
	if(state == GLUT_UP)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			mousePressed = 0;
			if(!disabled)
			{
				play = 0;
				ff theta = board.arrow.second;
				ff v = (power+40)*0.004;
				power = 0;
				board.arrow.first = 0;
				board.coins[9].modifyVel(mp(v*cos(DEG2RAD(theta)),v*sin(DEG2RAD(theta))));
				board.copy(replayBoard);
				for(int i=0;i<11;i++)
				for(int j=0;j<11;j++) reMark[i][j] = mark[i][j];
		
			}			
		}
		else if (button == GLUT_RIGHT_BUTTON) {;}
	}
} 

void passiveMotion(int x, int y)
{
	if(play && !replay)
	{
		GLdouble ox=0.0,oy=0.0,oz=0.0;
		GLint viewport[4];
		GLdouble modelview[16],projection[16];
		GLfloat wx=x,wy,wz = 0.0;
		glGetIntegerv(GL_VIEWPORT,viewport);
		y=viewport[3]-y;
		wy=y;
		glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
		glGetDoublev(GL_PROJECTION_MATRIX,projection);
		glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
		gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
		glutPostRedisplay();
		if(play && !disabled)
		{
			pair<ff,ff> p = board.coins[9].getPos();
			ox*=80;
			oy*=80;
			board.arrow.second = atan((oy-p.second)/(ox-p.first))*(1.0*180/PI);
			if(board.arrow.second<0) board.arrow.second +=180;		
			//cout<<"pop "<<ox<<" "<<oy<<" "<<p.first<<" "<<p.second<<" "<<board.arrow.second<<"\n";
		}
	}	
}
