#include "FuncForMain.h"

void createSimple1()
{
	std::cout << "Creating file: simple1.txt" << endl;
	ofstream fout("simple1.txt");
	fout << "Simple1" << endl;
	fout << "2 Bedrooms + Kitchen Isle" << endl;
	fout << 8 << endl;
	fout << 10 << endl;
	fout << "WWWWWWWWWW" << endl;
	fout << "W22  DW59W" << endl;
	fout << "W  W 1119W" << endl;
	fout << "W WWW3WW W" << endl;
	fout << "W6   3W  W" << endl;
	fout << "W78W  W  W" << endl;
	fout << "W99W  W  W" << endl;
	fout << "WWWWWWWWWW" << endl;
	fout.close();
}

void writeConfigFile(const string &iniPath)
{
	ofstream fout(iniPath.c_str());
	fout << "BatteryConsumptionRate  =    1" << endl;
	fout << "MaxSteps   = 1200" << endl;
	fout << "MaxStepsAfterWinner=200" << endl;
	fout << "BatteryCapacity=400" << endl;
	fout << "BatteryRachargeRate=20" << endl;
	fout.close();

	ifstream fin(iniPath.c_str());
	string line;

}

bool hasEnding(string const &fullString, string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

bool IsPrefix(string const& fullstring, string const& prefix)
{
	return equal(
		fullstring.begin(),
		fullstring.begin() + min(fullstring.size(), prefix.size()),
		prefix.begin());
}

string ConvertHouseStatetoString(houseState state) {
	string msg;
	if (state == NO_DOCKING)
		msg = "missing docking station (no D in house)";
	if (state == MORE_THAN_ONE_DOCKING)
		msg = "too many docking stations (more than one D in house)";
	return msg;
}

void PrintErrors(map<string, string> errorMap) {
	for (auto it = errorMap.cbegin(); it != errorMap.cend(); ++it) {
		cout << "file name" << it->first << "problem : " << it->second << endl;
	}
}

void checkVectorByMap(map<string, int> map1, vector<string> vector1, vector<bool>* boolV) {
	for (size_t i = 0; i < vector1.size(); i++) {
		(*boolV)[i] = !(map1.find(vector1[i]) == map1.end());
	}
}

void  printConfiError(int numberOfMissing, vector<bool> checkAllConfi, vector<string> confiVector) {
	int cnt = 0;
	cout << "config.ini missing " << numberOfMissing << " parameter(s): ";
	for (size_t i = 0; i < checkAllConfi.size(); i++) {
		if (!checkAllConfi[i]) {
			cnt++;
			(cnt == numberOfMissing) ? cout << confiVector[i] << "."
				: cout << confiVector[i] << ", ";
		}
	}
	cout << endl;

}

bool checkConfig(string fileName, map<string, int>* config) {
	SimpleIniFileParser iniParser(fileName);
	if (!iniParser) {
		//print error msg;
		return false;
	}
	*config = iniParser.getMap();
	vector<string> confiVector = { "MaxStepsAfterWinner",
		"BatteryCapacity", "BatteryConsumptionRate",
		"BatteryRechargeRate" };
	vector<bool> checkAllConfi(confiVector.size());
	checkVectorByMap(*config, confiVector, &checkAllConfi);
	unsigned int numberOfMissingConfi;
	numberOfMissingConfi = std::count(checkAllConfi.begin(), checkAllConfi.end(), false);
	if (numberOfMissingConfi > 0) {
		printConfiError(numberOfMissingConfi, checkAllConfi, confiVector);
		return false;
	}
	return true;
}

void findHousesAndConfigFiles(vector<fs::path> fileName_currDir, vector<string>* houses_fileName, string& config_fileName,
	bool input_config, bool input_house, vector<string>* houseOnlyName) {
	for (vector<string>::size_type j = 0; j != fileName_currDir.size(); j++) {
		if ((hasEnding(fileName_currDir[j].string(), ".ini") && (input_config))) {
			config_fileName = fileName_currDir[j].string();
		}
		else if ((hasEnding(fileName_currDir[j].string(), ".house") && (input_house))) {
			(*houses_fileName).push_back(fileName_currDir[j].string());
			(*houseOnlyName).push_back(fileName_currDir[j].stem().string());
		}
	}
}
int checkHouses(vector<string> houseNames, vector<House*>* houses, map<string, string>* errorHouse) {
	string stringErrorMSGHouse("");
	for (vector<string>::size_type j = 0; j != houseNames.size(); j++) {
		House *house = new House();
		stringErrorMSGHouse = house->fillHouseData(houseNames[j]);
		if (stringErrorMSGHouse == "") {
			houseState state;
			Point point(-1, -1);
			if ((state = house->checkIfHouseLegal(point)) != GOOD_HOUSE)
				(*errorHouse)[houseNames[j]] = ConvertHouseStatetoString(state);
			else
				(*houses).push_back(house);
		}
		else
			(*errorHouse)[houseNames[j]] = stringErrorMSGHouse;
	}
	return houses->size();
}

void checkArguments(int argc, char** argv, string& config_fileName, vector<string>* houses_fileName, bool* input_house,
	bool* input_config) {
	if (argc > 1) { //there are command-line arguments
		for (int i = 1; i < argc; ++i) {

			if (hasEnding(argv[i], ".ini")) {
				fs::path p(argv[i]);
				fs::path full_p = fs::complete(p); // complete == absolute
				config_fileName = full_p.stem().string();
				*input_config = false;
			}
			else if (hasEnding(argv[i], ".house")) {
				fs::path p(argv[i]);
				fs::path full_p = fs::complete(p); // complete == absolute
				(*houses_fileName).push_back(full_p.stem().string());
				*input_house = false;
			}
		}
	}
}

void updateCurrDir(vector<fs::path>* fileName_currDir) {
	//at least one of the arguments were missing
	//create vector that containe all files in the current directory
	fs::path full_path_dir(fs::current_path());
	fs::directory_iterator end_iter;
	if (fs::exists(full_path_dir) && fs::is_directory(full_path_dir))
	{
		for (fs::directory_iterator dir_iter(full_path_dir); dir_iter != end_iter; ++dir_iter)
		{
			if (fs::is_regular_file(dir_iter->status()))
			{
				fs::path tmp1 = *dir_iter;
				(*fileName_currDir).push_back(tmp1);
			}
		}
	}
}