#ifndef SMP_MONTECARLO
#define SMP_MONTECARLO

#include <boost/random.hpp>

#include "../SemiMarkovModel.h"

class SMPMonteCarlo
{
	SemiMarkovModel* _smpModel;	
	int _targetState;
	int _initState;

	boost::mt19937 rng;

public:
	vector<double> _phi;
	vector<double> _time;
	
	SMPMonteCarlo() { }
	SMPMonteCarlo(SemiMarkovModel* model, int initState, int targetState);

	void RunModel(unsigned int N, int NSteps, bool bReliability = true);
	//double RunModel(unsigned int N, double missiontime);
	int Simulate(double missiontime, bool bReliability = true);
};
#endif