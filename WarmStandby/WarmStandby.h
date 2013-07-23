#ifndef WARM_STANDBY
#define WARM_STANDBY

#include <boost/numeric/ublas/vector.hpp>
#include <boost/function.hpp>

#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"

using namespace boost::numeric::ublas;

class WarmStandby
{
	SemiMarkovModel* _smp;
	vector<double> _phi;
	vector<double> _time;

	double mttf;
	double cov;
	double ttrCov;
	double mttr;
	double mission;


	JaggedMatrix* _oldModel;
public:
	WarmStandby();
	void RunModel();
	void BuildOldModel();
	void RunSystem();
	void RunComponent();

	matrix<double> KernelHandler(double t);
	vector<double> WaitingHandler(double t);
};
#endif