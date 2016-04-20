#include "_203246509_C.h"



Direction _203246509_C::step() {
	return SharedAlgorithm::chooseStep(1);


}


extern "C" AbstractAlgorithm* maker() {
	return new _203246509_C();
}