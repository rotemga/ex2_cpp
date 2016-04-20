#include "FuncForMain.h"
#include <dlfcn.h>

typedef AbstractAlgorithm* (*algoCreator)();
int main(int argc, char* argv[]) {
	string config_path,house_path,algo_path;
	map<string, string> houses_mapName, algo_mapName, config_mapName;
	vector <fs::path> fileName_currDir;
	vector <House*> houses;
	vector <AbstractAlgorithm*> algos;
	map<string, int> config;
	map<string, string> ErrorMSGHouse;
	map<string, string> ErrorMSGAlgo;
	checkArguments(argc, argv, config_path,algo_path,house_path);
	if (!updateFilesFromDirectory(houses_mapName, ".house", house_path) || !updateFilesFromDirectory(algo_mapName, ".so", algo_path) ||
		!updateFilesFromDirectory(config_mapName, ".ini", config_path)) {
		Usage(house_path, config_path, algo_path);
		return 1;
	}
	int numberOfValidHouses = checkHouses(houses_mapName, &houses, &ErrorMSGHouse);
	if (config_mapName.size() < 1) {
		cout << "config file does not exist" << endl;
		return 1;
	}
	if (!checkConfig(config_mapName.begin()->first, &config))
		return 1;
	for (auto algoSoFilesNames : algo_mapName) {
		const char* tmpname = algoSoFilesNames.first.c_str();
		void* handler = dlopen(tmpname, RTLD_NOW);
		if (handler == nullptr) {
			ErrorMSGAlgo[algoSoFilesNames.first] = "file cannot be loaded or is not a valid .so";
			continue;
		}
		algoCreator alg;
		*(void **)(&alg) = dlsym(handler, "maker");
		if (alg == nullptr) {
			ErrorMSGAlgo[algoSoFilesNames.first] = "valid .so but no algorithm was registered after loading it";
			continue;
		}
		AbstractAlgorithm* loadedAlgo = alg();
		algos.push_back(loadedAlgo);
	}
	int numberOfValidAlgo = algos.size();
//	cout << numberOfValidHouses << numberOfValidAlgo << endl;
	if (numberOfValidAlgo && numberOfValidHouses) {
		Simulator sim(houses, algos, config);
		vector<string> algoOnlyNames, houseonlyNames;
		createVcetorFromMapValues(algoOnlyNames, algo_mapName,ErrorMSGAlgo);
		createVcetorFromMapValues(houseonlyNames, houses_mapName,ErrorMSGHouse);
		sim.run(algoOnlyNames,houseonlyNames);
	}
	if (ErrorMSGHouse.size() > 0 || ErrorMSGAlgo.size() > 0) {
		cout << "Errors:" << endl;
		PrintErrors(ErrorMSGHouse, houses_mapName, "house");
		if (algos.size() == 0)
			cout << "All algorithm files in target folder " << " cannot be opened or are invalid :" << endl;
		PrintErrors(ErrorMSGAlgo, algo_mapName, "so");
	}
	for (auto it = houses.begin(); it != houses.end(); it++)
		delete *it;
	houses.clear();
	algos.clear();

	return 0;
}
