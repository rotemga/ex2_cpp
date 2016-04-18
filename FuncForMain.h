#ifndef FUNCFORMAIN_H_
#define FUNCFORMAIN_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "House.h"
#include <string>
#include "Simulator.h"
#include "SimpleIniFileParser.h"
#include "_203246509_B.h"
#include "_203246509_A.h"
#include "_203246509_C.h"



#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations

using namespace std;
namespace fs = boost::filesystem;

void createSimple1();
bool hasEnding(string const &fullString, string const &ending);
bool IsPrefix(string const& fullstring, string const& prefix);
void writeConfigFile(const string &iniPath);
string ConvertHouseStatetoString(houseState state);
void PrintErrors(map<string, string> errorMap);
void checkVectorByMap(map<string, int> map1, vector<string> vector1, vector<bool>* boolV);
void  printConfiError(int numberOfMissing, vector<bool> checkAllConfi, vector<string> confiVector);
bool checkConfig(string fileName, map<string, int>* config);
void findHousesAndConfigFiles(vector<fs::path> fileName_currDir, vector<string>* houses_fileName, string& config_fileName,
	bool input_config, bool input_house, vector<string>* houseOnlyName);
int checkHouses(vector<string> houseNames, vector<House*>* houses, map<string, string>* errorHouse);
void checkArguments(int argc, char** argv, string& config_fileName, vector<string>* houses_fileName, bool* input_house,
	bool* input_config);
void updateCurrDir(vector<fs::path>* fileName_currDir);
#endif /* FUNCFORMAIN_H_ */