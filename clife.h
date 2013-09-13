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

	unsigned char getold(int x, int y) const {
		if(x < 0) x += width;
		else if (x >=width) x -= width;
		if(y < 0) y += height;
		else if (y >= height) y -= height;
		return gridold[y * width + x];
	}

	unsigned char getoldunsafe(int x, int y) const {
		return gridold[y * width + x];
	}

	bool isalive(unsigned int _x, unsigned int _y) const {
		if(!alive) return false;
		unsigned int x = _x % (width - 1);
		unsigned int y = _y % (height - 1);
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

	__inline void setnew(unsigned int x, unsigned int y, unsigned char val) {
		if(!alive) return;
		gridnew[y * width + x] = val;
	}

	__inline static unsigned char decide(unsigned int val, unsigned int neighbors) {
		if(val == LIVE) {
			if(neighbors < 2 || neighbors > 3) {
				val = DEAD;
			}
		} else if(neighbors == 3) {
			val = LIVE;
		}
		return val;
	}

	__inline unsigned char countneighbors(int x, int y) const {
		unsigned char c = 0;
		c += getold(x - 1, y - 1);
		c += getold(x, y - 1);
		c += getold(x + 1, y - 1);
		c += getold(x - 1, y);
		c += getold(x + 1, y);
		c += getold(x - 1, y + 1);
		c += getold(x, y + 1);
		c += getold(x + 1, y + 1);
		return c;
	}

	__inline unsigned char countneighborsunsafe(int x, int y) const {
		unsigned char c = 0;
		c += getoldunsafe(x - 1, y - 1);
		c += getoldunsafe(x, y - 1);
		c += getoldunsafe(x + 1, y - 1);
		c += getoldunsafe(x - 1, y);
		c += getoldunsafe(x + 1, y);
		c += getoldunsafe(x - 1, y + 1);
		c += getoldunsafe(x, y + 1);
		c += getoldunsafe(x + 1, y + 1);
		return c;
	}

	void iterate() {
		if(!alive) return;
		bool changed = false;
		// there's an opportunity for optimization in here
		for(int y = 1; y + 1 < height; ++y) {
			for(int x = 1; x + 1 < width; ++x) {
				setnew(x, y, decide(getoldunsafe(x, y), countneighborsunsafe(x, y)));
			}
		}
		for(int y = 0; y < height; ++y) {
			setnew(0, y, decide(getoldunsafe(0, y), countneighbors(0, y)));
			setnew(width - 1, y, decide(getoldunsafe(width - 1, y), countneighbors(width - 1, y)));
		}
		for(int x = 0; x < width; ++x) {
			setnew(x, 0, decide(getoldunsafe(x, 0), countneighbors(x, 0)));
			setnew(x, height - 1, decide(getoldunsafe(x, height - 1), countneighbors(x, height - 1)));
		}

		unsigned char *temp = gridnew;
		gridnew = gridold;
		gridold = temp;
	}

	void clear() {
		memset(gridold, 0, width * height * sizeof(unsigned char));
	}

	void randomize(unsigned int bias_in_thousandths = 500) {
		clear();
		unsigned int bias = MAX(1, MIN(999, bias_in_thousandths));
		for(unsigned int y = height / 3; y < (height * 2) / 3; ++y) {
			for(unsigned int x = width / 3; x < (width * 2) / 3; ++x) {
				//if((rand() % 1000) >= bias)
				if((rand() % 1000) >= bias) setlive(x, y);
				//setold(x, y, rand() % 1000 >= bias ? LIVE : DEAD);
			}
		}
		stalled = false;
	}

private:
	void cosperglider(unsigned int x, unsigned int y) {
		if(x + 37 >= width || y + 11 >= height) return; // not enough room
		for(unsigned int _y = y; _y < y + 11; ++_y) {
			for(unsigned int _x = x; _x < x + 37; ++_x) {
				setold(_x, _y, DEAD);
			}
		}
		setlive(x + 1, y + 4);
		setlive(x + 2, y + 4);
		setlive(x + 1, y + 5);
		setlive(x + 2, y + 5);

		setlive(x + 13, y + 1);
		setlive(x + 14, y + 1);
		setlive(x + 12, y + 2);
		setlive(x + 16, y + 2);
		setlive(x + 11, y + 3);
		setlive(x + 17, y + 3);
		setlive(x + 11, y + 4);
		setlive(x + 15, y + 4);
		setlive(x + 17, y + 4);
		setlive(x + 18, y + 4);
		setlive(x + 11, y + 5);
		setlive(x + 17, y + 5);
		setlive(x + 12, y + 6);
		setlive(x + 16, y + 6);
		setlive(x + 13, y + 7);
		setlive(x + 14, y + 7);

		setlive(x + 25, y + 3);
		setlive(x + 23, y + 4);
		setlive(x + 25, y + 4);
		setlive(x + 21, y + 5);
		setlive(x + 22, y + 5);
		setlive(x + 21, y + 6);
		setlive(x + 22, y + 6);
		setlive(x + 21, y + 7);
		setlive(x + 22, y + 7);
		setlive(x + 23, y + 8);
		setlive(x + 25, y + 8);
		setlive(x + 25, y + 9);

		setlive(x + 35, y + 6);
		setlive(x + 36, y + 6);
		setlive(x + 35, y + 7);
		setlive(x + 36, y + 7);

	}
public:
	void special(unsigned char which) {
		switch(which) {
			case 0:
				clear();
				cosperglider(10, 10);
				break;
			default:
				randomize();
		}
	}

	bool isstalled() const {
		return stalled;
	}
};
