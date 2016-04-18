#include "Simulator.h"



Simulator::Simulator(vector <House*> houses1, vector <AbstractAlgorithm*> algos1)
{
	houses = houses1;
	algorithms = algos1;
}
Simulator::Simulator(vector <House*> houses1, vector <AbstractAlgorithm*> algos1, map<string, int> config1) :
houses(houses1), algorithms(algos1), config(config1){

}



Simulator::~Simulator() {
}

void Simulator::setInputHouses(vector<House*> input_houses){
	houses = input_houses;
}
void Simulator::setInputConfig(map<string, int> input_config){
	config = input_config;
}
void Simulator::setInputAlgo(vector <AbstractAlgorithm*> input_algorithms){
	algorithms = input_algorithms;
}


//update the input score with all the parameters
void Simulator::createScore(int winner_num_stepsint, int num_steps, int pos_in_comeptition, bool is_back_docking, int dirt_collected, int sum_Dirth_House, Score *score){
	score->setWinnerNumSteps(winner_num_stepsint);
	score->setNumSteps(num_steps);
	score->setIsBackInDocking(is_back_docking);
	score->setDirtCollected(dirt_collected);
	score->setPosition(pos_in_comeptition);
	score->setSumDirtInHouse(sum_Dirth_House);
}




//This function gets as input vector of robots and check if all the robots finished running
bool Simulator::allRobotsFinished(vector <Robot*> robots){
	unsigned int cnt = 0;
	for (vector<Robot*>::size_type k = 0; k != robots.size(); k++){
		if (!(robots[k]->isCanRun()))
			cnt++;
	}
	if (cnt == robots.size())
		return true;
	return false;
}
//return true if the robot win (i.e it's finished cleaning the house and it's back to docking station)
bool Simulator::robotWin(Robot* robot){
	return ((robot->isHouseClean()) && (robot->areWeInDocking()));
}


void PrintTable(vector<Robot*> robots, vector<string> houseNames, vector<string> algoNames) {

}
//simulate the running of the simulator
void Simulator::run(vector<string> houseNames) {

	vector <Robot*> robots;
	Score score;


	for (vector<AbstractAlgorithm*>::size_type i = 0; i != algorithms.size(); i++) {
		algorithms[i]->setConfiguration(config);
	}


	Point* point = new Point(-1, -1);

	for (vector<House*>::size_type i = 0; i != houses.size(); i++) {
		int Steps = houses[i]->getmaxSteps(), pos_in_competition = 1, actual_position_in_copmetition = 1;
		int simulation_steps = 0, winner_num_steps = 0, num_of_wins_this_iter = 0;
		houses[i]->findDockingStation(*point);
		for (vector<AbstractAlgorithm*>::size_type j = 0; j != algorithms.size(); j++){
			// robot for all pair <house,algorithm>
			Battery* battery = new Battery(config.at("BatteryCapacity"), config.at("BatteryConsumptionRate"), config.at("BatteryRechargeRate"));
			Robot* robot = new Robot(houses[i], algorithms[j], point, battery);
			robots.push_back(robot);
		}
		houses[i]->output();
		while (Steps > 0){//each iteration simulate each robot one step
			for (vector<Robot*>::size_type k = i*algorithms.size(); k != robots.size(); k++){
				if (!robots[k]->isCanRun())
					continue;
				robots[k]->runRobot();
				if (robotWin(robots[k])){//robot win, update Steps and it's score
					Steps = config.at("MaxStepsAfterWinner") + 1;
					if (winner_num_steps == 0){
						winner_num_steps = simulation_steps;
					}
					num_of_wins_this_iter++;
					pos_in_competition = min(4, actual_position_in_copmetition);
					createScore(winner_num_steps, simulation_steps, pos_in_competition, true, robots[k]->DirtCollected(), 0, &score);
					robots[k]->setCanRun(false);
					robots[k]->setScore(score);

				}

			}
			if (allRobotsFinished(robots))//check if there are robots that can run
				break;
			simulation_steps++;
			Steps--;
			if (num_of_wins_this_iter > 0){
				actual_position_in_copmetition += num_of_wins_this_iter;
			}
			houses[i]->output();

		}

		if (winner_num_steps == 0){
			winner_num_steps = simulation_steps;
		}



		for (vector<Robot*>::size_type k = 0; k != robots.size(); k++){

			//the robot crashed a wall or the battery is empty 
			//or:
			//the robot  didn't finish cleaning the house and it can run
			if ((robots[k]->isCanRun() || robots[k]->isBrokedDown())){
				createScore(winner_num_steps, simulation_steps, 10, robots[k]->areWeInDocking(), robots[k]->DirtCollected(), robots[k]->sumDirtInHouse(), &score);
				robots[k]->setScore(score);
				//cout <<"the robot didnt win:"  <<score.calcResult() << endl;


			}
			/*	if (robots[k]->isBrokedDown())
			cout << "battery empty" << endl;*/
		}
		for (vector<Robot*>::size_type k = 0; k != robots.size(); k++){
			/*	if (robotWin(robots[k])){//robot win
			cout << "the robot win" << endl;
			}
			robots[k]->printHouse();*/
			Score s = robots[k]->getScore();
			cout << s.calcResult() << endl;
		}

	}

	vector<string> algnames = { "algo1_304978372" };
	Table table(houseNames, algnames, robots);
	table.printTable();

	delete point;
	for (auto it = robots.begin(); it != robots.end(); ++it){
		delete *it;
	}

	robots.clear();



}