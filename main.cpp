#include "FuncForMain.h"

int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	bool input_house = true, input_config = true;
	string tmp = argv[0];
	string config_fileName;
	vector<string> houses_fileName;
	vector <fs::path> fileName_currDir;
	vector <House*> houses;
	vector <AbstractAlgorithm*> algos;
	map<string, int> config;
	map<string, string> ErrorMSGHouse;
	vector<string> houseOnlyName;

	checkArguments(argc, argv, config_fileName, &houses_fileName, &input_house, &input_config);
	if ((input_house) || (input_config))
		updateCurrDir(&fileName_currDir);
	findHousesAndConfigFiles(fileName_currDir, &houses_fileName, config_fileName, input_config, input_house, &houseOnlyName);
	if (!checkConfig(config_fileName, &config))
		return 1;
	AbstractAlgorithm* Algo = new _203246509_A();
	AbstractAlgorithm* Algo2 = new _203246509_B();
	AbstractAlgorithm* Algo3 = new _203246509_C();
	algos.push_back(Algo);
	algos.push_back(Algo2);
	algos.push_back(Algo3);


	if (checkHouses(houses_fileName, &houses, &ErrorMSGHouse) > 0) {
		Simulator sim(houses, algos, config);
		sim.run(houseOnlyName);
	}
	PrintErrors(ErrorMSGHouse);
	for (auto it = houses.begin(); it != houses.end(); ++it) {
		delete *it;
	}
	houses.clear();
	algos.clear();
	return 0;
}
