#ifndef MAXMAX_H_
#define MAXMAX_H_

#include "House.h"
#include "Direction.h"
#include "OurSensor.h"
#include "Battery.h"
#include <map>
#include <algorithm>
#include <set>

using namespace std;

int scoring(const map <Point, char> m, Point point, int CurrBattery, int ConRateBattery);
void checkLegalDirection(OurSensor* sensor, set<int>& s);
Direction getBestDirection(House* house, OurSensor* sensor, Point* point, Battery* battery, int steps);
bool NeedGoBackToDocking(House* house, OurSensor* sensor, Point* point, Battery* battery);
vector<Direction> findMinPathToDocking(House* house, Point* point, Point* docking, OurSensor* sensor, int cnt, vector<Direction> path);
int getDirt(House* house, Point* point, Direction direction);
int sumDirt(const map <Point, char> m);
bool canGoBackToDocking(map<Point, char> map_info, Point point, int currBattery, int ConRateBattery);
int findMinPathToDocking(map<Point, char> map_info, Point point, int cnt, int ok);
Direction changePointToDirection(Point currPoint, Point newPoint);
void legalMoves(map<Point, char> map_info, Point currPoint, map<Point, int> &m, int for_min_path);
int charToDirtLevel(char c);
int getBestMove(map<Point, char> map_info, Point point, Point* wanted_point, int currBattery, int conRateBattery, int BatteryRechargeRate, int depth, int steps, int cnt);
Direction get_Move(map<Point, char> map_info, Point point, Point* wanted_point);
char* DirectionToString(Direction d);
void printMap(map<Point, int> m, Point point);
int getMaxCoordinate(map<Point, char> m, int coordinate);
char interputForPrint(char ch);
void printCurrMap(map <Point, char> m);
Direction mostStars(map<Point, char> map_info, Point point, Point* wanted_point);
#endif /* MAXMAX_H_ */