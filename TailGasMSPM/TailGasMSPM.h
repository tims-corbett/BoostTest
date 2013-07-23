#ifndef TAIL_GAS_MSPM			//Multi-State
#define TAIL_GAS_MSPM

class TestResults;

#include<string>

class TailGasMSPM
{
	double _missionTime, _NSteps;
public:
	void RunModel();
	void  DegradedComponentTest(double _mean32, double _cv32, double _mean20, double _cv20, double _muTime13, double _muTime03, double _pmTime, TestResults& result, std::string name)	;				
};


#endif