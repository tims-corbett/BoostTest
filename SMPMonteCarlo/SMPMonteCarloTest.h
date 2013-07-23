#ifndef SMPMONTECARLOTEST
#define SMPMONTECARLOTEST

#include<string>

class TestResults;

class SMPMonteCarloTest
{
public:
	void RunModel();
	void SingleComponentModel();
	void SingleComponentModel(double _mean , double _cv , double _muTime, TestResults& results);
	void NCCWTest();
	void NCCWTest(double cov, TestResults& results);
	
	void DegradedComponentTest();
	void DegradedComponentTest(double _mean32, double _cv32, double _mean20, double _cv20, double _muTime13, double _muTime02, double _pmTime, TestResults& result, std::string name);					
};
#endif