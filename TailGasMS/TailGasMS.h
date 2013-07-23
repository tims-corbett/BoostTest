#ifndef TAIL_GAS_MS			//Multi-State
#define TAIL_GAS_MS

class TestResults;

#include<string>

class TailGasMS
{
	double _missionTime, _NSteps;
public:
	void RunModel();
	void  DegradedComponentTest(double _mean21, double _cv21, double _mean10, double _cv10, double _muTime, TestResults& result, std::string name, bool bAddTime=false);					

};


#endif