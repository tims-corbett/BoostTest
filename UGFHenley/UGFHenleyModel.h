#ifndef UGF_HENLEY_MODEL
#define UGF_HENLEY_MODEL

#include "../UGFLibrary/UGFLibrary.h"

class UGFHenleyModel
{
	UFunction u1, u2, u3, u4;
	vector<double> time;



public:
	void RunModel();
	vector<vector<double>> RunSubsystem(double mean[], double cov[], double rep[]);

};

#endif