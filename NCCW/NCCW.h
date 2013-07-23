#ifndef NCCW_H
#define NCCW_H

#include "../TestResults.h"
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;


class SemiMarkovModel;


class NCCW
{
	double lambda_1;
	double lambda_H;
	double mu;

	vector<double> time;
	TestResults results;

public:
	NCCW();
	void RunModel();
	void BuildModel(double cov);
};

#endif