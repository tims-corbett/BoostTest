#ifndef BIROLINI_HENLEY
#define BIROLINI_HENLEY

class SemiMarkovModel;
class JaggedMatrix;

#include <boost/numeric/ublas/vector.hpp>


using namespace boost::numeric::ublas;


class BiroliniHenley
{
	SemiMarkovModel* _smp;
	JaggedMatrix* _oldModel;
	JaggedMatrix* _newModel;

	double _mission;
	int _nsteps;

	double mttf;
	double ttfCov;
	double mttr;
	double ttrCov;

public:
	BiroliniHenley();
	void RunModel();
	void BuildOldModel();
	void BuildNewModel();
	void RunSystem();
	void RunComponent();


	matrix<double> KernelHandler(double t);
	vector<double> WaitingHandler(double t);

	~BiroliniHenley();
};

#endif