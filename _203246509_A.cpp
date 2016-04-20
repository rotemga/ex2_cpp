#include "_203246509_A.h"





Direction _203246509_A::step() {
	return SharedAlgorithm::chooseStep(5);
}


extern "C" AbstractAlgorithm* maker() {
	return new _203246509_A();
}