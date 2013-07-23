#ifndef UGFLIBRARY
#define UGFLIBRARY

#include <boost/numeric/ublas/vector.hpp>
#include<boost/function.hpp>

#include<math.h>

using namespace boost::numeric::ublas;

typedef boost::function<int (vector<double>, vector<double>, int, int)> UGFHandler;


class UFunction
{

public:
	vector<vector<double>> p;
	vector<double> perf;

	UFunction() { };
	UFunction(int size);
	UFunction(vector<vector<double>> psample);
	UFunction(vector<vector<double>> psample, vector<double> perfsample);

	static UFunction UProduct(UFunction& u1, UFunction u2, UGFHandler handler);
	static double MinRule(vector<double> perf1, vector<double> perf2, int i, int j) { return (perf1[i] < perf2[j]) ? i : j; }
	static double MaxRule(vector<double> perf1, vector<double> perf2, int i, int j) { return (perf1[i] > perf2[j]) ? i : j; }

};

#endif