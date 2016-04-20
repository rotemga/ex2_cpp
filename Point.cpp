#include "Point.h"



Point::Point(int _x, int _y) :
	x(_x), y(_y) {
}

bool Point::isInDocking() {
	return (x == 0 && y == 0);
}

Point* Point::newCopyOfPoint() {
	Point* p = new Point(x, y);
	return p;
}

bool Point::operator<(const Point& point)const {
	return std::make_tuple(x, y) < std::make_tuple(point.getX(), point.getY());
}
bool Point::operator==(const Point& point)const {
	return (x == point.getX() && y == point.getY());
}