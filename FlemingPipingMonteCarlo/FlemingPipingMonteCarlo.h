#ifndef FLEMING_PIPING_MONTECARLO
#define FLEMING_PIPING_MONTECARLO

class TestResults;

class FlemingPipingMonteCarlo
{
	double	omega,
			phi,
			lambdaF,
			rhoF,
			mu,
			rhoL;

	int S,F,L,R;


public:
	void RunModel();
	FlemingPipingMonteCarlo();
	void RunModel(double cov, TestResults& results);
};
#endif