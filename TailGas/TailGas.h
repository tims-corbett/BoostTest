#ifndef TAIL_GAS
#define TAIL_GAS

class TestResults;

#include<string>

class TailGas
{
	double _missionTime, _NSteps;
public:
	void RunModel();
	void  SingleComponentTest(double _mean, double _cv, double _muTime, TestResults& result, std::string name, bool bAddTime=false);					

};


#endif