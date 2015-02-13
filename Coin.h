#include "__Header__.h"

class Coin
{
	private:
		ff rad;
		int type;
		pair<ff, ff> vel, pos;

	public:
		bool pocketed; 
		Coin() {}
		Coin(int _type) { rad = 0.095 * ((_type==3) ? 1.5 : 1); }
		Coin(pair<ff, ff> _pos, int _type)
		{
			pos = _pos;
			type = _type;
			vel = mp(0,0);
			rad = 0.095 * ((type==3) ? 1.5 : 1);
			pocketed = 0;
		}
		pair<ff, ff> getVel() { 
			return vel;
		}
		pair<ff,ff> getPos() { 
			return pos;
		}
		int getType() { 
			return type;
		}
		ff getRad() {
			return rad;
		}
		ff getMass() {
			return mass[type];
		}
		void draw();
		bool colliding(Coin& a);
		void handleWall(ff len, ff e);
		void handlePocket(pair<ff,ff> p[],ff r);
		void modifyPos(pair<ff,ff> _pos) {
			pos = _pos;
		}
		void modifyVel(pair<ff,ff> _vel) {
			vel = _vel;
		}
		void copy(Coin &a)
		{
			a.modifyPos(pos);
			a.modifyVel(vel);
			a.pocketed = pocketed;
		}
};

void Coin::draw()
{
	ff r = color[type][0];
	ff g = color[type][1];
	ff b = color[type][2];
	if(disabled && type == 3)
		r = 1,b = g = 0;
	drawCircle(rad,pos,1,r,g,b);
}

bool Coin::colliding(Coin& a)
{
	ff rad1 = this->rad;
	ff rad2 = a.getRad();
	ff p1 = this->pos.first - a.getPos().first;
	ff p2 = this->pos.second - a.getPos().second;
	if(sqrt(p1*p1 + p2*p2) - rad1 - rad2 <= 0.01) return true;
	return false;
}

void Coin::handleWall(ff len, ff e)
{
	if((pos.first + rad - len/2 >= -0.1 && vel.first>0) || (pos.first - rad + len/2 <= 0.1 && vel.first<0))
	            this->vel.first *= -1*e;
	if((pos.second + rad - len/2 >= -0.1 && vel.second>0) || (pos.second - rad + len/2 <= 0.1 && vel.second<0))
	            this->vel.second *= -1*e;
}

void Coin::handlePocket(pair<ff,ff> p[],ff r)
{
	for(int i=0;i<4;i++)
	{
		ff d = sqrt(pow(pos.first-p[i].first,2) + pow(pos.second-p[i].second,2));
		ff mag = sqrt(pow(vel.first,2) + pow(vel.second,2));
		ff d1 = r + rad;
		if(d1>=d && mag<=0.2) {
			if(myCoin==-1 && type!=0) myCoin = type;
			if(type==0) score+=50;
			else if(type==myCoin) score+=10;
			else score-=5;
			pocketed = 1; 
			return;
		}
	}
}


