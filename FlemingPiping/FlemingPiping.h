#ifndef FLEMING_PIPING
#define FLEMING_PIPING

#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"


class FlemingPiping
{

	double	omega,
			phi,
			lambdaF,
			rhoF,
			mu,
			rhoL;

	int S,F,L,R;

	vector<double> vWblS, vWblF, vWblL, vWblR;
	vector<double> vGamS, vGamF, vGamL, vGamR;

	vector<double> time;

			

public:
	FlemingPiping();

	void RunModel();
	void RunWeibullModel(double cov);
};


#endif