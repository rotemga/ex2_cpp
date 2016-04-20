#include "_203246509_B.h"



Direction _203246509_B::step() {
	return SharedAlgorithm::chooseStep(0.5);

}

extern "C" AbstractAlgorithm* maker() {
	return new _203246509_B();
}