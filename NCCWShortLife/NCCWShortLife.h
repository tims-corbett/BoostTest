#ifndef NCCWShortLife_H
#define NCCWShortLife_H

#include "../TestResults.h"
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;


class SemiMarkovModel;


class NCCWShortLife
{
	double lambda_1;
	double lambda_H;
	double mu;

	vector<double> time;
	TestResults results;

public:
	NCCWShortLife();
	void TestTruncatedDistribution();
	void RunModel();
	void BuildModel();
};

#endif