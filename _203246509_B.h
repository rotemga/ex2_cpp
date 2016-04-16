#ifndef _203246509_B_H_
#define _203246509_B_H_

#include "AbstractAlgorithm.h"
#include "OurSensor.h"
#include "FuncForAlgos.h"
#include <list>
#include <algorithm>
using namespace std;
class _203246509_B :
	public AbstractAlgorithm
{
	OurSensor* thisSensor;
	map<string, int> thisConfig;
	map<Point, char> map_info;
	Point *currPoint;
	int CurrBattery;
	int ConRateBattery;
	int BatteryRechargeRate;
	int StepsTillFinishing;



public:
	_203246509_B();
	virtual ~_203246509_B();
	virtual void setSensor(const AbstractSensor& sensor) override;
	virtual void setConfiguration(map<string, int> config) override;
	virtual Direction step() override;
	virtual void aboutToFinish(int stepsTillFinishing) override;

	void updateMapInfo(SensorInformation info);
	void  updatePointDirt();
};

#endif /* _203246509_B_H_ */