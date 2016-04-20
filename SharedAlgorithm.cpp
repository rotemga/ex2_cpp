#include "SharedAlgorithm.h"


SharedAlgorithm::SharedAlgorithm()
{
	//thisSensor = new AbstractSensor();
	currPoint = new Point(0, 0);//docking station is in (0,0)
	StepsTillFinishing = -1;
	num_of_steps = 0;
	cnt = 0;

}


SharedAlgorithm::~SharedAlgorithm()
{
	delete thisSensor;
	delete currPoint;
}


void SharedAlgorithm::setSensor(const AbstractSensor& sensor){
	map_info.clear();
	moves.clear();
	thisSensor = &sensor;
	//set configuration
	CurrBattery = thisConfig.at("BatteryCapacity");
	ConRateBattery = thisConfig.at("BatteryConsumptionRate");
	BatteryRechargeRate = thisConfig.at("BatteryRechargeRate");
	StepsTillFinishing = -1;
	num_of_steps = 0;
	cnt = 0;
	currPoint->setPoint(0, 0);



}

void SharedAlgorithm::setConfiguration(map<string, int> config){
	thisConfig = config;
	CurrBattery = thisConfig.at("BatteryCapacity");
	ConRateBattery = thisConfig.at("BatteryConsumptionRate");
	BatteryRechargeRate = thisConfig.at("BatteryRechargeRate");

}
Direction SharedAlgorithm::step(){
	return Direction::Stay;
}


void SharedAlgorithm::aboutToFinish(int stepsTillFinishing){
	StepsTillFinishing = stepsTillFinishing;
}







// '*' we know it's not a wall
void SharedAlgorithm::updateMapInfo(SensorInformation info){
	int x = currPoint->getX();
	int y = currPoint->getY();
	if (currPoint->isInDocking()){ 
		map_info[Point(x, y)] = 'D'; } //in docking
	else{ 
		int i = info.dirtLevel;
		if (i > 0){ i = i - 1; }
		char c = i + '0';
		map_info[Point(x, y)] = c; } //stay
	if (!map_info.count(Point(x, y + 1)))
		map_info[Point(x, y + 1)] = ((info.isWall[0]) ? 'W' : '*'); //east
	if (!map_info.count(Point(x, y - 1)))
		map_info[Point(x, y - 1)] = ((info.isWall[1]) ? 'W' : '*'); // west
	if (!map_info.count(Point(x + 1, y)))
		map_info[Point(x + 1, y)] = ((info.isWall[2]) ? 'W' : '*'); //south
	if (!map_info.count(Point(x - 1, y)))
		map_info[Point(x - 1, y)] = ((info.isWall[3]) ? 'W' : '*'); //north
}

void  SharedAlgorithm::updatePointDirt(){
	int dirt = (int)(map_info[*currPoint] - '0');
	if ((map_info[*currPoint] != 'D') && (dirt > 0)){
		map_info[*currPoint]--;
	}
}


Direction SharedAlgorithm::directionToDocking( Point* wanted_point){
	int x = currPoint->getX(), y = currPoint->getY();
	map<Point, int> m;
	legalMoves(m, 1);
	//cout << "return to docking: currPoint (" << currPoint->getX() << " ," << currPoint->getY() << "), currBattery: " << CurrBattery << ", num_of_steps: " << num_of_steps << endl;
	if (x == 0 && y == 0) {
		num_of_steps = 0;
		return Direction::Stay;
	}
	else if (y > 0){//need to go west
		if (m.count(Point(x, y - 1))){
			wanted_point->setPoint(x, y - 1);
			return Direction::West;
		}
		return forDirectionToDockingFunc(x, y, m, wanted_point);


	}
	else if (y < 0){//nead to go east
		if (m.count(Point(x, y + 1))){
			wanted_point->setPoint(x, y + 1);
			return Direction::East;
		}
		return forDirectionToDockingFunc(x, y, m, wanted_point);
	}
	else {
		return forDirectionToDockingFunc(x, y, m, wanted_point);
	}
}

Direction SharedAlgorithm::forDirectionToDockingFunc(int x, int y, map <Point, int> m, Point* wanted_point){
	if (x > 0) {//need to go north
		if (m.count(Point(x - 1, y))){
			wanted_point->setPoint(x - 1, y);
			return Direction::North;
		}
	}
	else if (x < 0){//need to go south
		if (m.count(Point(x + 1, y))){
			wanted_point->setPoint(x + 1, y);
			return Direction::South;
		}
	}

	//wanted_point->setPoint(m.begin()->first);
	//Direction direct = changePointToDirection(m.begin()->first);
	//if (direct != Direction::Stay){
	//	return direct;
	//}
	wanted_point->setPoint(x, y);
	return Direction::Stay;
}




//return true if the algo have enough battery to go back to docking station.
bool SharedAlgorithm::canGoBackToDocking(){
	if (currPoint->isInDocking()) return true;
	int minPath = findMinPathToDocking(0, 0);
	return ((CurrBattery - minPath*ConRateBattery) > 0) ? true : false;

}


//return the length of the minimal path from current point to docking.
int SharedAlgorithm::findMinPathToDocking(int cnt, int ok){

	map<Point, int> m;
	int min = map_info.size();

	if (currPoint->isInDocking()){
		ok = 1;
		return cnt;
	}
	if (ok == 1)
		return cnt;
	if (cnt >= min)
		return cnt;
	else{
		legalMoves(m, 1);//update m to be only legal directions (no option to stay in place)
		map<Point, int>::iterator it;
		for (it = m.begin(); it != m.end(); it++) {//iterate over all legal moves
			cnt = findMinPathToDocking(++cnt, ok);

			if (cnt < min){
				min = cnt;
				if (ok == 1) return min;
			}
		}
	}
	return min;

}


void SharedAlgorithm::printMap(map<Point, int> m, Point point){
	cout << "CurrPoint is: (" << point.getX() << " ," << point.getY() << ")" << " we have in it :" << m[point] << endl;
	map<Point, int>::iterator it;
	cout << "Size of Map: " << m.size() << " ,legal moves: ";
	for (it = m.begin(); it != m.end(); it++){
		cout << DirectionToString(changePointToDirection(it->first)) << " (" << it->first.getX() << " ," << it->first.getY() << ")" << "   ";
	}
	cout << endl;
}



int SharedAlgorithm::getMaxCoordinate(map<Point, char> m, int coordinate) {
	int max = 0;
	for (auto iterator = m.begin(); iterator != m.end(); iterator++) {
		Point p1 = iterator->first;
		int value = (coordinate == 1) ? p1.getX() : p1.getY();
		max = (abs(value) > max) ? abs(value) : max;
	}
	return max;
}
char SharedAlgorithm::interputForPrint(char ch) {
	if (ch == 'D' || ch == 'W' || ch == '*' || (ch >= '0' && ch <= '9') || ch == ' ')
		return ch;
	return '?';
}

void SharedAlgorithm::printCurrMap(map <Point, char> m) {
	map<Point, char>::iterator it;
	int maxX = getMaxCoordinate(m, 1);
	int maxY = getMaxCoordinate(m, 2);
	for (int i = maxX; i >= -maxX; i--) {
		for (int j = -maxY; j <= maxY; j++)
			cout << interputForPrint(m[Point(-i, j)]);
		cout << "  y = " << i << endl;
	}
}

//get direction and return char* of it
char* SharedAlgorithm::DirectionToString(Direction d){
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




//get score_for_star and wanted_point as inputs
//update wanted_point to be best point to move to it.
Direction SharedAlgorithm::get_Move(Point* wanted_point, float score_for_star){
	float max_score = -2, east = -2, west = -2, south = -2, north = -2, stay = 0;
	int x = currPoint->getX(), y = currPoint->getY();
	map<Point, int> m;
	Direction direct;
	legalMoves(m, 0);//update m to be only legal directions
	if (m.size() == 0)
		return  Direction::Stay;
	//if (checkIfAlgoStack() == true){//algo stack in place
	//	direct = moves[moves.size() - 1];
	//	return reverseDirection(direct, wanted_point);
	//}

	if (StepsTillFinishing != -1){
		if (StepsTillFinishing*ConRateBattery >= CurrBattery){//if we don't have enough steps, we must go back to docking
			num_of_steps++;
			//return directionToDocking(wanted_point);
			direct = moves[moves.size() - 1];
			return reverseDirection(direct, wanted_point);
		}
	}
	if (moves.size()*ConRateBattery + 1 >= CurrBattery){//(CurrBattery <= (map_info.size() / 2)))//if we don't have enough battery, we must go back to docking
		num_of_steps++;
		//return directionToDocking(wanted_point);
		direct = moves[moves.size() - 1];
		return reverseDirection(direct, wanted_point);

	}
	map<Point, int>::iterator it;
	for (it = m.begin(); it != m.end(); it++) {//iterate over all legal moves

		if ((it->first.getY() == y + 1) && ((it->first.getX() == x))){//east
			east = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
		else if ((it->first.getY() == y - 1) && ((it->first.getX() == x))){//west
			west = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
		else if ((it->first.getX() == x + 1) && ((it->first.getY() == y))){//south
			south = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
		else if ((it->first.getX() == x - 1) && ((it->first.getY() == y))){//north
			north = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
		else if ((it->first.getX() == x) && ((it->first.getY() == y))){//stay
			stay = ((it->second >= 0) ? it->second : ((it->second == -1) ? 0 : score_for_star));
		}
	}
	num_of_steps++;
	max_score = std::max(std::max(east, west), std::max(std::max(south, north), stay));
	if (max_score == 0){//if all legal direction with no dirt, check in which direction there are most stars
		return mostStars(wanted_point);
	}

	if (east == max_score){
		wanted_point->setPoint(x, y + 1);
		moves[num_of_steps - 1] = Direction::East;
		return  Direction::East;
	}
	else if (west == max_score){
		wanted_point->setPoint(x, y - 1);
		moves[num_of_steps - 1] = Direction::West;
		return  Direction::West;
	}
	else if (south == max_score){
		wanted_point->setPoint(x + 1, y);
		moves[num_of_steps - 1] = Direction::South;
		return  Direction::South;
	}
	else if (north == max_score){
		wanted_point->setPoint(x - 1, y);
		moves[num_of_steps - 1] = Direction::North;
		return  Direction::North;
	}
	else if (stay == max_score){
		wanted_point->setPoint(x, y);
		num_of_steps--;
		return  Direction::Stay;
	}

	return Direction::Stay;

}

Direction SharedAlgorithm::mostStars(Point* wanted_point){
	int x = currPoint->getX(), y = currPoint->getY();
	int tmp, east = 0, west = 0, south = 0, north = 0;
	Direction direct;
	map<Point, int> m;


	map<Point, char>::iterator it;
	for (it = map_info.begin(); it != map_info.end(); it++) {
		tmp = charToDirtLevel(it->second);
		if ((it->first.getY() > y) && (tmp > 0 || tmp == -3) && (map_info[Point(x, y + 1)] != 'W')){
			east += ((tmp > 0) ? tmp : 2);
		}
		tmp = charToDirtLevel(it->second);
		if ((it->first.getY() < y) && (tmp > 0 || tmp == -3) && (map_info[Point(x, y - 1)] != 'W')){
			west += ((tmp > 0) ? tmp : 2);
		}
		tmp = charToDirtLevel(it->second);
		if ((it->first.getX() > x) && (tmp > 0 || tmp == -3) && (map_info[Point(x + 1, y)] != 'W')){
			south += ((tmp > 0) ? tmp : 2);
		}
		tmp = charToDirtLevel(it->second);
		if ((it->first.getX() < x) && (tmp > 0 || tmp == -3) && (map_info[Point(x - 1, y)] != 'W')){
			north += ((tmp > 0) ? tmp : 2);
		}
	}
	int max_score = std::max(std::max(east, west), std::max(south, north));
	if (max_score == 0){
		legalMoves(m, 0);//update m to be only legal directions
		if (m.size() == 1){
			wanted_point->setPoint(m.begin()->first);
			return  changePointToDirection(m.begin()->first);
		}
		else{//if we can move, we are not to stay in place.
			map<Point, int>::iterator it;
			for (it = m.begin(); it != m.end(); it++) {//iterate over all legal moves
				wanted_point->setPoint(it->first); 
				Direction direct = changePointToDirection(it->first);
				if (direct != Direction::Stay){
					return direct;
				}
			}
		}
	}

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

// get direction and wanted_point,
// it's reverse the direction and update the wanted_point to be the point is this direction
Direction SharedAlgorithm::reverseDirection(Direction direct, Point *wanted_point){
	int x = currPoint->getX(), y = currPoint->getY();
	//cout << "Back to docking, currPoint: (" << x << " ," << y << ")" << endl;
	moves.erase(moves.size()-1);
	switch (direct){
	case Direction::East:
		wanted_point->setPoint(x, y - 1);
		return Direction::West;
	case Direction::West:
		wanted_point->setPoint(x, y + 1);
		return Direction::East;
	case Direction::North:
		wanted_point->setPoint(x + 1, y);
		return Direction::South;
	case Direction::South:
		wanted_point->setPoint(x - 1, y);
		return Direction::North;
	default:
		return Direction::Stay;
	}
	return Direction::Stay;
}


//check if algorithm stack in loop. (for example: west east west east...)
bool SharedAlgorithm::checkIfAlgoStack(){
	if (moves.size() >= 3){
		Direction last_direction = moves[moves.size() - 1];
		Direction second_last_direction = moves[moves.size() - 2];
		Direction third_last_direction = moves[moves.size() - 3];
		if (last_direction != third_last_direction)
			return false;
		Point* tmp = new Point(0, 0);
		if (reverseDirection(third_last_direction, tmp) == second_last_direction){
			delete tmp;
			return true;
		}
	}
	return false;

}
//get point as input and return the direction it's provides
Direction SharedAlgorithm::changePointToDirection(Point newPoint){
	int x_cur = currPoint->getX();
	int y_cur = currPoint->getY();
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
//get as input map and update it with legal moves (points as key and current information as int)
//-1 is docking, -2 is wall, -3 is not wall, 0-9 is dirt level.
void SharedAlgorithm::legalMoves(map<Point, int> &m, int for_min_path){
	int x = currPoint->getX();
	int y = currPoint->getY();
	int val;
	Point* tmp = new Point(x, y);
	if (!for_min_path) m[*currPoint] = charToDirtLevel(map_info[*currPoint]);//insert stay. for func findMinPathToDocking (int for_min_path==1) don't insert stay!
	tmp->setPoint(x, y + 1);
	if (map_info.count(*tmp)){
		if ((val = charToDirtLevel(map_info[*tmp])) != -2)//East not wall
			m[*tmp] = val;
	}
	tmp->setPoint(x, y - 1);
	if (map_info.count(*tmp)){
		if ((val = charToDirtLevel(map_info[*tmp])) != -2)//West not wall
			m[*tmp] = val;
	}
	tmp->setPoint(x + 1, y);
	if (map_info.count(*tmp)){
		if ((val = charToDirtLevel(map_info[*tmp])) != -2)//South not wall 
			m[*tmp] = val;
	}
	tmp->setPoint(x - 1, y);
	if (map_info.count(*tmp)){
		if ((val = charToDirtLevel(map_info[*tmp])) != -2)//North not wall
			m[*tmp] = val;
	}
	delete tmp;
}


//get char as input and return the value that represents it in int.
// -1 = docking
// -2 = wall
// -3 = we know it's not wall
// 0 - 9 = dirt level
int SharedAlgorithm::charToDirtLevel(char c){
	if (c == 'W') { return -2; }// -2 is wall
	if (c == 'D') { return -1; }// -1 is docking
	int tmp = c - '0';
	return (c == '*') ? -3 : tmp; // -3 is '*' (not Wall, but dirtlevel is unknown), else 0-9 dirt level
}



//get score_for_star as input and return direction.
Direction SharedAlgorithm::chooseStep( float score_for_star){
	SensorInformation a = thisSensor->sense();
	//thisSensor->setInfo(a);
	updateMapInfo(a);
	Point* wanted_point = new Point(0,0);
	Direction direct = get_Move(wanted_point, score_for_star);
	currPoint->setPoint(*wanted_point);
	if (currPoint->isInDocking()){
		if (CurrBattery < 0){
			CurrBattery = 0;
		}
		CurrBattery += BatteryRechargeRate;
		num_of_steps = 0;
	}
	else{
		CurrBattery -= ConRateBattery;
	}
	//cout << "currPoint: (" << currPoint->getX() << " ," << currPoint->getY() << ")" << endl;
	//printCurrMap(map_info);
	//if ((currPoint->getX() == 1) && (currPoint->getY() == 2)) {
	//	cout << "cnt: " << cnt << endl;
	//}
	delete wanted_point;
	cnt++;
	return direct;
}

