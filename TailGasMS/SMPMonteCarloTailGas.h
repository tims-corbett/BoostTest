#ifndef SMPMONTECARLO_TAILGAS
#define SMPMONTECARLO_TAILGAS

#include<string>

class TestResults;

class SMPMonteCarloTailGas
{
	double _missionTime;
	double _NSteps;

public:
	void RunModel();
	void  DegradedComponentTest(double _mean21, double _cv21, double _mean10, double _cv10, double _muTime, TestResults& result, std::string name);					

};
#endif