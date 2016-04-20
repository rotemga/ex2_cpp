#ifndef SHAREDALGORITHM_H_
#define SHAREDALGORITHM_H_

#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "Point.h"
//#include "FuncForAlgos.h"
#include <algorithm>
using namespace std;
class SharedAlgorithm :
	public AbstractAlgorithm
{
	const AbstractSensor* thisSensor;
	map<string, int> thisConfig;
	map<Point, char> map_info;
	map<int, Direction> moves;
	Direction last_move;
	Point *currPoint;
	int CurrBattery;
	int ConRateBattery;
	int BatteryRechargeRate;
	int StepsTillFinishing;
	int num_of_steps;
	int cnt;



public:
	SharedAlgorithm();
	virtual ~SharedAlgorithm();
	virtual void setSensor(const AbstractSensor& sensor) override;
	virtual void setConfiguration(map<string, int> config) override;
	virtual Direction step();
	virtual void aboutToFinish(int stepsTillFinishing) override;

	void updateMapInfo(SensorInformation info);
	void  updatePointDirt();

	Direction directionToDocking(Point* wanted_point);
	Direction forDirectionToDockingFunc(int x, int y, map <Point, int> m, Point* wanted_point);
	bool canGoBackToDocking();
	int findMinPathToDocking(int cnt, int ok);
	void printMap(map<Point, int> m, Point point);
	int getMaxCoordinate(map<Point, char> m, int coordinate);
	char interputForPrint(char ch);
	void printCurrMap(map <Point, char> m);
	char* DirectionToString(Direction d);
	Direction get_Move(Point* wanted_point, float score_for_star);
	Direction mostStars(Point* wanted_point);
	Direction changePointToDirection(Point newPoint);
	void legalMoves(map<Point, int> &m, int for_min_path);
	int charToDirtLevel(char c);
	Direction chooseStep(float score_for_star);
	Direction reverseDirection(Direction direct, Point *wanted_point);
	bool SharedAlgorithm::checkIfAlgoStack();

};

#endif /* SHAREDALGORITHM_H_ */