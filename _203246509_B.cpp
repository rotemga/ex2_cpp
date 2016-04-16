#include "_203246509_B.h"


_203246509_B::_203246509_B()
{
	thisSensor = new OurSensor();
	currPoint = new Point(0, 0);//docking station is in (0,0)
	//num_steps = 0;
	StepsTillFinishing = -1;

}


_203246509_B::~_203246509_B()
{
	delete thisSensor;
	delete currPoint;
}


void _203246509_B::setSensor(const AbstractSensor& sensor){
	SensorInformation a = sensor.sense();
	thisSensor->setInfo(a);
	updateMapInfo(a);

}

void _203246509_B::setConfiguration(map<string, int> config){
	thisConfig = config;
	CurrBattery = thisConfig.at("BatteryCapacity");
	ConRateBattery = thisConfig.at("BatteryConsumptionRate");
	BatteryRechargeRate = thisConfig.at("BatteryRechargeRate");

}

Direction _203246509_B::step(){
	Point* wanted_point = new Point(0, 0);
	int bestMove = getBestMove(map_info, *currPoint, wanted_point, CurrBattery, ConRateBattery, BatteryRechargeRate, 2, StepsTillFinishing,0);
	Direction direct = changePointToDirection(*currPoint, *wanted_point);
	currPoint->setPoint(*wanted_point);
	delete wanted_point;
	return direct;
	//return get_Move(map_info, *currPoint, CurrBattery, ConRateBattery, BatteryRechargeRate, StepsTillFinishing);
}

void _203246509_B::aboutToFinish(int stepsTillFinishing){
	StepsTillFinishing = stepsTillFinishing;
}







// '*' we know it's not a wall
void _203246509_B::updateMapInfo(SensorInformation info){
	int x = currPoint->getX();
	int y = currPoint->getY();
	if (currPoint->isInDocking()){ 
		map_info[Point(x, y)] = 'D'; } //in docking
	else{ 
		const char char_max = -1;
		int i = info.dirtLevel;
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

void  _203246509_B::updatePointDirt(){
	int dirt = (int)(map_info[*currPoint] - '0');
	if ((map_info[*currPoint] != 'D') && (dirt > 0)){
		map_info[*currPoint]--;
	}
}