#ifndef HENLEY_REDUCED_STATE
#define HENLEY_REDUCED_STATE

#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"


class HenleyReducedState
{
public:
	HenleyReducedState();
	void RunModel(double cov);
	void RunModel();

};


#endif