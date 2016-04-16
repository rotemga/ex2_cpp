#include "FuncForAlgos.h"



int sumDirt(const map <Point, char> m){
int sum = 0;
int i = 0;
for (map<Point, char>::const_iterator it = m.begin(); it != m.end(); ++it)
{
	i = charToDirtLevel(it->second);
	if (i > 0){
		sum += i;
	}
}
return sum;

}

int scoring(const map <Point, char> m, Point point, int CurrBattery, int ConRateBattery){
	int dust = charToDirtLevel(m.at(point));
//	cout << "dust is: " << dust << endl;
	int addition = (dust >= 0) ? (dust) : ((dust == -1) ? -1 : 2);
	return (addition -(sumDirt(m)+50) + (canGoBackToDocking(m, point, CurrBattery, ConRateBattery) ? 100 : 0));
}





//return true if the algo have enough battery to go back to docking station.
bool canGoBackToDocking(map<Point, char> map_info, Point point, int currBattery, int ConRateBattery){
	if (point.isInDocking()) return true;
	int minPath = findMinPathToDocking(map_info, point, 0, 0);
	return ((currBattery - minPath*ConRateBattery) > 0) ? true : false;

}


//return the length of the minimal path from current point to docking.
int findMinPathToDocking(map<Point, char> map_info, Point point, int cnt, int ok){

	map<Point, int> m;
	int min = map_info.size();

	if (point.isInDocking() || (ok = 1)){
		ok = 1;
		return cnt;
	}
	else{
		legalMoves(map_info, point, m, 1);//update m to be only legal directions (no option to stay in place)
		map<Point, int>::iterator it;
		for (it = m.begin(); it != m.end(); it++) {//iterate over all legal moves
			cnt = findMinPathToDocking(map_info, (it->first), ++cnt, ok);

			if (cnt < min){
				min = cnt;
				if (ok == 1) return min;
			}
		}
	}
	return min;

}
//update wanted_point to be best point to move to it.
int getBestMove(map<Point, char> map_info, Point point, Point* wanted_point, int currBattery, int conRateBattery, int BatteryRechargeRate, int depth, int steps, int cnt){
	map<Point, int> m;
	int tmp, max_score = 0;
	int Scoring = 0;
	//cout << endl << endl;
	//printCurrMap(map_info);

	if (cnt != 0) {
		char c = static_cast<char>(map_info[point] - 1);
	//	cout << "	" << map_info[point] << "	##########	" << c <<  endl;
		if (charToDirtLevel(map_info[point]) > 0) { map_info[point] = c; }//we have dirt in current point
		if ((charToDirtLevel(map_info[point]) == -1)) { currBattery += BatteryRechargeRate; } //we on docking station
		else { currBattery -= conRateBattery; } //we not in docking 
	}



	//cout << "The depth is: " << depth << endl;
	if ((depth == 0)){
		Scoring = scoring(map_info, point, currBattery, conRateBattery);
	//	cout << "CurrPoint is: (" << point.getX() << " ," << point.getY() << ")" << " we have in it :" << map_info[point] << endl;
	//	cout << "The score is: " << Scoring << endl;
		return Scoring;

	}

	else if ((currBattery <= 0) || (steps == 0)){
		return scoring(map_info, point, currBattery, conRateBattery);

	}
	else {
		legalMoves(map_info, point, m, 0);//update m to be only legal directions
		//printMap(m, point);
		map<Point, int>::iterator it;
		for (it = m.begin(); it != m.end(); it++) {//iterate over all legal moves
			//if ((cnt < 10) && (it->first == point))
			//	continue;
			if (steps > 0)	steps--;

			tmp = getBestMove(map_info, it->first, wanted_point, currBattery, conRateBattery, BatteryRechargeRate, depth - 1, steps, cnt + 1);

			if (max_score < tmp){
				max_score = tmp;
				wanted_point->setPoint(it->first);
			
			}

		}
	}

	return max_score;

}



void printMap(map<Point, int> m, Point point){
	cout << "CurrPoint is: (" << point.getX() << " ," << point.getY() << ")" << " we have in it :" << m[point] << endl;
	map<Point, int>::iterator it;
	cout << "Size of Map: " << m.size() << " ,legal moves: ";
	for (it = m.begin(); it != m.end(); it++){
		cout << DirectionToString(changePointToDirection(point, it->first)) << " (" << it->first.getX() << " ," << it->first.getY() << ")" << "   ";
	}
	cout << endl;
}



int getMaxCoordinate(map<Point, char> m, int coordinate) {
	int max = 0;
	for (auto iterator = m.begin(); iterator != m.end(); iterator++) {
		Point p1 = iterator->first;
		int value = (coordinate == 1) ? p1.getX() : p1.getY();
		max = (abs(value) > max) ? abs(value) : max;
	}
	return max;
}
char interputForPrint(char ch) {
	if (ch == 'D' || ch == 'W' || ch == '*' || (ch >= '0' && ch <= '9') || ch == ' ')
		return ch;
	return '?';
}

void printCurrMap(map <Point, char> m) {
	map<Point, char>::iterator it;
	int maxX = getMaxCoordinate(m, 1);
	int maxY = getMaxCoordinate(m, 2);
	for (int i = maxX; i >= -maxX; i--) {
		for (int j = -maxY; j <= maxY; j++)
			cout << interputForPrint(m[Point(-i, j)]);
		cout << "  y = " << i << endl;
	}
}

char* DirectionToString(Direction d){
	switch (d){
	case Direction::East:
		return "East";

	case Direction::West:
		return "West";

	case Direction::South:
		return "South";

	case Direction::North:
		return "North";

	case Direction::Stay:
		return "Stay";

	default: return "Stay";
	}
	return "Stay";
}





//update wanted_point to be best point to move to it.
Direction get_Move(map<Point, char> map_info, Point point, Point* wanted_point){
	float max_score = -2, east = -2, west = -2, south = -2, north = -2, stay = 0;
	int x = point.getX(), y = point.getY();
	Direction direct;
	map<Point, int> m;

	legalMoves(map_info, point, m, 0);//update m to be only legal directions
	if (m.size()==0)
		return  Direction::Stay;

	map<Point, int>::iterator it;
	for (it = m.begin(); it != m.end(); it++) {//iterate over all legal moves

		if ((it->first.getY() == y + 1) && ((it->first.getX() == x))){//east
			east = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : 0.5));
		}
		else if ((it->first.getY() == y - 1) && ((it->first.getX() == x))){//west
			west = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : 0.5));
		}
		else if ((it->first.getX() == x + 1) && ((it->first.getY() == y)) ){//south
			south = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : 0.5));
		}
		else if ((it->first.getX() == x - 1) && ((it->first.getY() == y))){//north
			north = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : 0.5));
		}
		else if ((it->first.getX() == x) && ((it->first.getY() == y))){//stay
			stay = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : 0.5));
		}
	}

	max_score = std::max(std::max(east, west), std::max(std::max(south, north),stay));
	if (max_score == 0){
		return mostStars(map_info, point, wanted_point);
	}

	if (east == max_score){
		wanted_point->setPoint(x, y+1);
		return  Direction::East;
	}
	else if (west == max_score){
		wanted_point->setPoint(x, y-1);
		return  Direction::West;
	}
	else if (south == max_score){
		wanted_point->setPoint(x+1, y);
		return  Direction::South;
	}
	else if (north == max_score){
		wanted_point->setPoint(x-1, y);
		return  Direction::North;
	}
	else if (stay == max_score){
		wanted_point->setPoint(x,y);
		return  Direction::Stay;
	}

	return Direction::Stay;

}

Direction mostStars(map<Point, char> map_info, Point point, Point* wanted_point){
	int x = point.getX(), y = point.getY();
	int tmp, east = 0, west = 0, south = 0, north = 0;
	Direction direct;
	map<Point, int> m;


	map<Point, char>::iterator it;
	for (it = map_info.begin(); it != map_info.end(); it++) {
		tmp = charToDirtLevel(it->second);
		if ((it->first.getY() > y) && (tmp > 0 || tmp == -3)&& (map_info[Point(x,y+1)]!='W')){
			east += ((tmp > 0) ? tmp : 2);
		}
		tmp = charToDirtLevel(it->second);
		if ((it->first.getY() < y) && (tmp > 0 || tmp == -3) && (map_info[Point(x, y - 1)] != 'W')){
			west += ((tmp > 0) ? tmp : 2);
		}
		tmp = charToDirtLevel(it->second);
		if ((it->first.getX() > x) && (tmp > 0 || tmp == -3) && (map_info[Point(x+1, y)] != 'W')){
			south += ((tmp > 0) ? tmp : 2);
		}
		tmp = charToDirtLevel(it->second);
		if ((it->first.getX() < x) && (tmp > 0 || tmp == -3) && (map_info[Point(x-1, y)] != 'W')){
			north += ((tmp > 0) ? tmp : 2);
		}
	}
	int max_score = std::max(std::max(east, west), std::max(south, north));
	if (east == max_score){
		wanted_point->setPoint(x, y + 1);
		return  Direction::East;
	}
	else if (west == max_score){
		wanted_point->setPoint(x, y - 1);
		return  Direction::West;
	}
	else if (south == max_score){
		wanted_point->setPoint(x + 1, y);
		return  Direction::South;
	}
	else if (north == max_score){
		wanted_point->setPoint(x - 1, y);
		return  Direction::North;
	}

	return Direction::Stay;
}








Direction changePointToDirection(Point currPoint, Point newPoint){
	int x_cur = currPoint.getX();
	int y_cur = currPoint.getY();
	int x_new = newPoint.getX();
	int y_new = newPoint.getY();

	if (x_cur == x_new){
		if (y_cur == y_new) { return Direction::Stay; }
		return ((y_cur + 1 == y_new) ? Direction::East : Direction::West);
	}
	else {
		return ((x_cur + 1 == x_new) ? Direction::South : Direction::North);

	}


}

void legalMoves(map<Point, char> map_info, Point currPoint, map<Point, int> &m, int for_min_path){
	int x = currPoint.getX();
	int y = currPoint.getY();
	int val;
	if (!for_min_path) m[currPoint] = charToDirtLevel(map_info[currPoint]);//insert stay. for func findMinPathToDocking (int for_min_path==1) don't insert stay!
	currPoint.setPoint(x, y + 1);
	if (map_info.count(currPoint)){
		if ((val = charToDirtLevel(map_info[currPoint])) != -2)//East not wall
			m[currPoint] = val;
	}
	currPoint.setPoint(x, y - 1);
	if (map_info.count(currPoint)){
		if ((val = charToDirtLevel(map_info[currPoint])) != -2)//West not wall
			m[currPoint] = val;
	}
	currPoint.setPoint(x + 1, y);
	if (map_info.count(currPoint)){
		if ((val = charToDirtLevel(map_info[currPoint])) != -2)//South not wall 
			m[currPoint] = val;
	}
	currPoint.setPoint(x - 1, y);
	if (map_info.count(currPoint)){
		if ((val = charToDirtLevel(map_info[currPoint])) != -2)//North not wall
			m[currPoint] = val;
	}
}



int charToDirtLevel(char c){
	if (c == 'W') { return -2; }// -2 is wall
	if (c == 'D') { return -1; }// -1 is docking
	int tmp = c - '0';
	return (c == '*') ? -3 : tmp; // -3 is '*' (not Wall, but dirtlevel is unknown), else 0-9 dirt level
}




//get the dirtLevel in direction's point
int getDirt(House* house, Point* point, Direction direction){
	int x = point->getX();
	int y = point->getY();
	char dirt;
	switch (direction){

	case Direction::East:
		dirt = house->findDirtLevel(x, y++);
		return (dirt == 'D') ? 0 : (int)dirt - '0';

	case Direction::West:
		dirt = house->findDirtLevel(x, y--);
		return (dirt == 'D') ? 0 : (int)dirt - '0';

	case Direction::South:
		dirt = house->findDirtLevel(x++, y);
		return (dirt == 'D') ? 0 : (int)dirt - '0';

	case Direction::North:
		dirt = house->findDirtLevel(x--, y);
		return (dirt == 'D') ? 0 : (int)dirt - '0';

	case Direction::Stay:
		dirt = house->findDirtLevel(x, y);
		return (dirt == 'D') ? 0 : (int)dirt - '0';

	default:
		return -2;
	}
}

