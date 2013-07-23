#ifndef PM_MODEL
#define PM_MODEL

#include <boost/numeric/ublas/vector.hpp>
#include <boost/function.hpp>

#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"
#include "../TestResults.h"


class PreventiveMaintenanceModel
{
public:

	void RunModel();
	void RunModel(double lambda_m);

	double _C(double val);


void RunModel(int k, double lambdam_inv, TestResults& results);

};

#endif