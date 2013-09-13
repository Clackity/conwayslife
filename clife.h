#pragma once

#include <cstring>
#include <stdlib.h>
#include <time.h>

#include "macros.h"

class cLife {
private:
	static const unsigned int maxdim = 16384;
	//static const unsigned int faderate = 16;
	static const unsigned char LIVE = 1, DEAD = 0;
	bool alive, stalled;
	unsigned int width, height;
	unsigned char *gridold, *gridnew;
public:
	cLife(unsigned int _width, unsigned int _height) {
		alive = false;
		width = MAX(1, MIN(maxdim, _width));
		height = MAX(1, MIN(maxdim, _height));
		gridold = new unsigned char[width * height];
		if(gridold) {
			gridnew = new unsigned char[width * height];
			if(!gridnew) {
				delete gridold;
				return;
			}
		}
		memset(gridold, 0, (width * height) * sizeof(unsigned char));
		alive = true;
		stalled = false;
		srand(time(NULL));
	}
	virtual ~cLife() {
		if(alive) {
			delete gridnew;
			delete gridold;
		}
	}

	unsigned char getold(unsigned int _x, unsigned int _y) const {
		if(!alive) return 0;
		unsigned int x = _x % width;
		unsigned int y = _y % height;
		return gridold[y * width + x];
	}

	bool isalive(unsigned int _x, unsigned int _y) const {
		if(!alive) return false;
		unsigned int x = _x % width;
		unsigned int y = _y % width;
		return (gridold[y * width + x] == LIVE);
	}

	void setlive(unsigned int x, unsigned int y) {
		if(!alive) return;
		gridold[y * width + x] = LIVE;
	}

	void setold(unsigned int x, unsigned int y, unsigned char val) {
		if(!alive) return;
		gridold[y * width + x] = val;
	}

	void setnew(unsigned int x, unsigned int y, unsigned char val) {
		if(!alive) return;
		gridnew[y * width + x] = val;
	}

	void iterate() {
		if(!alive) return;
		bool changed = false;
		// there's an opportunity for optimization in here
		for(unsigned int y = 0; y < height; ++y) {
			for(unsigned int x = 0; x < width; ++x) {
				unsigned int live = 0;
				live += getold(x - 1, y - 1);
				live += getold(x, y - 1);
				live += getold(x + 1, y - 1);
				live += getold(x - 1, y);
				live += getold(x + 1, y);
				live += getold(x - 1, y + 1);
				live += getold(x, y + 1);
				live += getold(x + 1, y + 1);
				//if(255 == getold(x - 1,	y - 1)) live++;
				//if(255 == getold(x,		y - 1)) live++;
				//if(255 == getold(x + 1, y - 1)) live++;
				//if(255 == getold(x - 1, y))		live++;
				//if(255 == getold(x + 1, y))		live++;
				//if(255 == getold(x - 1, y + 1)) live++;
				//if(255 == getold(x,		y + 1)) live++;
				//if(255 == getold(x + 1, y + 1)) live++;
				unsigned int val = getold(x, y);
				if(val == LIVE) {
					if(live < 2 || live > 3) {
						val = DEAD;
					}
				} else if(live == 3) {
					val = LIVE;
				}
				//if(255 == val) {
				//	if(live < 2 || live > 3) {
				//		val = 255 - faderate;
				//	}
				//} else {
				//	if(live == 3) {
				//		val = 255;
				//	} else {
				//		if(val > 0 && val < 255 - faderate) { // a crude check for liveliness
				//			changed = true;
				//		}
				//		val = val > faderate ? val - faderate : 0;
				//	}
				//}
				setnew(x, y, val);
			}
		}
		unsigned char *temp = gridnew;
		gridnew = gridold;
		gridold = temp;
		//if(!changed) stalled = true;
		//else stalled = false;
	}

	void randomize(unsigned int bias_in_thousandths = 500) {
		unsigned int bias = MAX(1, MIN(999, bias_in_thousandths));
		for(unsigned int y = 0; y < height; ++y) {
			for(unsigned int x = 0; x < width; ++x) {
				//if((rand() % 1000) >= bias)
				setold(x, y, rand() % 1000 >= bias ? LIVE : DEAD);
			}
		}
		stalled = false;
	}

	bool isstalled() const {
		return stalled;
	}
};
