#ifndef SPARES_H
#define SPARES_H

#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"


class Spares
{
	double failureMean, failureCOV, repairMean;

public:
	Spares();
	void TestPolyWeibull();
	void RunModel();
	void RunModel(int nTransformers, int nSpares);
};


#endif