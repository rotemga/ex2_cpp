#include "Point.h"



Point::Point(int _x, int _y) :
x(_x), y(_y) {
}

bool Point::isInDocking(){
	return (x == 0 && y == 0);
}

