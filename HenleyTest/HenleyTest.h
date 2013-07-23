#ifndef HENLEY_TEST
#define HENLEY_TEST


#include<string>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/function.hpp>

#include "../RBDTools.h"
#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"
#include "../RBDTools.h"
#include "../TestResults.h"


using namespace boost::numeric::ublas;

class HenleyTest
{
	double _missionTime;
	int _NSteps;
	std::string _desc;


public:
	vector<double> _time;
	vector<double> _phi;		//availability
	vector<double> _phiMC;
	vector<double> _timeMC;

	enum resulttype { type_montecarlo, type_smp };

	HenleyTest() { }
	HenleyTest(std::string desc, double missiontime, int N);

	void RunModel();

	void  SingleComponentTest(double _mean, double _cv, double _muTime);
	void ExplosionModel(int n,double mean[],double cv[], double muTime[]);

	static vector<double> ComposeSeries(HenleyTest& subsys1, HenleyTest& subsys2, int type);
	static void HandleComponents(double mean[], double cv[], double rep[],double missionTime, int nsteps);
	static void HandleSubsystems(double mean[], double cv[], double rep[], double missionTime, int nsteps);

	void TwoIdenticalComponentsThreeStates(double mean, double cov, double muTime);

	void OneRepairCrew_1o2(double mean[], double cv[], double muTime[]);

};



#endif