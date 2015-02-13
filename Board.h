#include "Coin.h"

class Board
{
	private:
		ff friction, cor, len, width, base, start, innermost_circle;
		ff outer_len,inner_len,innermost_len;

	public:
		Coin coins[10];
		pair<ff, ff> arrow, pkts[4];
		ff pkt;
		Board() {}
		Board(ff _len)
		{
			friction = 0.0015;
			cor = 0.8;
			len = _len;
			width = 0.38;
			base = 0.6;
			start = 0.3;
			pkt = 0.15;
			innermost_circle = 0.1;
			arrow.first = 1.3;
			arrow.second = 90;
			outer_len = len + 2*width;
			inner_len = len - 2*pkt -0.002;
			innermost_len = len - 2*base;
			pkts[0] = mp(inner_len/2,inner_len/2);
			pkts[1] = mp(inner_len/2,-inner_len/2);
			pkts[2] = mp(-inner_len/2,-inner_len/2);
			pkts[3] = mp(-inner_len/2,inner_len/2);
			ff ii;
			int jj;
			for(ii=22.5,jj=0;jj<4;ii+=90,jj++)
				coins[jj] = Coin(mp(start*cos(DEG2RAD(ii)),start*sin(DEG2RAD(ii))),1);
			for(ii=-22.5,jj=4;jj<8;ii+=90,jj++)
				coins[jj] = Coin(mp(start*cos(DEG2RAD(ii)),start*sin(DEG2RAD(ii))),2);
			coins[8] = Coin(mp(0,0),0);
			coins[9] = Coin();
			coins[9] = Coin(mp(0,-len/2+base+coins[9].getRad()),3);

		}
		void handleCollision();
		void draw();
		ff getBase() { return base;}
		ff getLen() { return len;}
		ff getFriction() { return friction;}
		ff getCOR() { return cor;}
		ff getPkt() { return pkt;}
		void copy(Board &a)
		{
			a.arrow.first = arrow.first;
			a.arrow.second = arrow.second;
			for(int i=0;i<10;i++)
				coins[i].copy(a.coins[i]);
		}

};

void Board::handleCollision()
{
	ff e = cor;
	for(int i=0;i<10;i++)
	{
		if(coins[i].pocketed) continue;
		for(int j=i+1;j<10;j++)
		{
			if(coins[j].pocketed) continue;
			if(coins[i].colliding(coins[j]) && mark[i][j]==0) mark[i][j] = 1;
			else if(mark[i][j] && coins[i].colliding(coins[j])) continue;
			else {mark[i][j]=0; continue;}

			pair<ff,ff> pos1,pos2,vel1,vel2;
			pos1 = coins[i].getPos();
			pos2 = coins[j].getPos();
			vel1 = coins[i].getVel();
			vel2 = coins[j].getVel();
			ff m1 = coins[i].getMass();
			ff m2 = coins[j].getMass();
			ff tmp = sqrt(pow((pos1.first-pos2.first),2) + pow((pos1.second-pos2.second),2));
			ff xx = (pos1.first - pos2.first)/tmp;
			ff yy = (pos1.second - pos2.second)/tmp;

			ff u1x = vel1.first*xx + vel1.second*yy;
			ff u2x = vel2.first*xx + vel2.second*yy;

			ff u1y = -vel1.first*yy + vel1.second*xx;
			ff u2y = -vel2.first*yy + vel2.second*xx;

			ff v1x = (m1*u1x + m2*u2x + m2*e*(u2x - u1x))/(m1+m2);
			ff v2x = (m1*u1x + m2*u2x + m1*e*(u1x - u2x))/(m1+m2);

			vel1.first = v1x*xx - u1y*yy;
			vel1.second = v1x*yy + u1y*xx;
			vel2.first = v2x*xx - u2y*yy;
			vel2.second = v2x*yy + u2y*xx;

			coins[i].modifyVel(vel1);
			coins[j].modifyVel(vel2);
			
			ff d = coins[i].getRad() + coins[j].getRad() - tmp + 0.001;
			ff dx = d*xx/2;
			ff dy = d*yy/2;
			pos1.first += dx;
			pos1.second += dy;
			pos2.first -= dx;
			pos2.second -= dy;
			coins[i].modifyPos(pos1);
			coins[j].modifyPos(pos2);
		}
	}


}


void Board::draw()
{
	//ff len1 = len + 2*width;
	ff _r = innermost_circle;
	ff _r1 = pkt;
	//ff len2 = len - 2*base;
	ff len3 = innermost_len - 2*_r;
	//ff len4 = len - 2*_r1 - 0.002;

	//outer square of board for boundary
	glColor3f(0.5, 0.35, 0.05);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-outer_len / 2 , -outer_len / 2);
	glVertex2f(outer_len / 2 , -outer_len / 2);
	glVertex2f(outer_len / 2 , outer_len / 2);
	glVertex2f(-outer_len / 2, outer_len / 2);
	glEnd();


	//inner board (a polygon)
	glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glVertex2f(-inner_len / 2, -len / 2);
	glVertex2f(-len / 2, -inner_len / 2);

	glVertex2f(-len / 2, inner_len / 2);
	glVertex2f(-inner_len / 2, len / 2);


	glVertex2f(inner_len / 2, len / 2);
	glVertex2f(len / 2, inner_len / 2);


	glVertex2f(len / 2, -inner_len / 2);
	glVertex2f(inner_len / 2, -len / 2);


	glEnd();

	//pockets
	for(int i=0;i<4;i++)
		drawCircle(_r1,pkts[i],1,0,0,0); //radius, pos, type(0-> line, 1-> fill), r, g, b

	//drawing tangents to inner circles
	glColor3f(0, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex2f(-len3 / 2 , -innermost_len / 2);
	glVertex2f(len3 / 2, -innermost_len / 2);
	glVertex2f(innermost_len / 2 , -len3 / 2);
	glVertex2f(innermost_len / 2, len3 / 2);
	glVertex2f(len3 / 2, innermost_len / 2);
	glVertex2f(-len3 / 2, innermost_len / 2);
	glVertex2f(-innermost_len / 2 , -len3 / 2);
	glVertex2f(-innermost_len / 2, len3 / 2);
	glEnd();

	//inner circles
	drawCircle(_r,mp(len3/2,len3/2),0,1,0,0); //radius, pos, type(0-> line, 1-> fill), r, g, b
	drawCircle(_r,mp(-len3/2,-len3/2),0,1,0,0); //radius, pos, type(0-> line, 1-> fill), r, g, b
	drawCircle(_r,mp(-len3/2,len3/2),0,1,0,0); //radius, pos, type(0-> line, 1-> fill), r, g, b
	drawCircle(_r,mp(len3/2,-len3/2),0,1,0,0); //radius, pos, type(0-> line, 1-> fill), r, g, b

	//center circle
	drawCircle(0.55,mp(0,0),0,1,0,0);

	//draw coins
	for(int i=0;i<9;i++) 
	{
		if(coins[i].pocketed) continue;
		coins[i].draw();
	}

	//draw striker
	coins[9].draw();

	//arrow
	glEnable(GL_LINE_STIPPLE);
	glLineWidth(3.0);
	glLineStipple(1, 0x00FF);
	glBegin(GL_LINES);
	glColor3f(0,1,1);
	pair<ff,ff> p = coins[9].getPos();
	glVertex2f(p.first,p.second);
	glVertex2f(p.first + arrow.first*cos(DEG2RAD(arrow.second)),p.second + arrow.first*sin(DEG2RAD(arrow.second)));
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glLineWidth(1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

