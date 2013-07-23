#ifndef RBDTOOLS
#define RBDTOOLS


#include <boost/numeric/ublas/vector.hpp>
#include<boost/function.hpp>

#include <map>		//from std
#include<vector>

using namespace boost::numeric::ublas;

typedef boost::function<double (std::vector<double>)> AvailabilityHandler;

class RBDTools
{

public:
	static void Combinations(int ComponentCount,std::vector<std::vector<double>>& Combinations);

	static void Combinations(
		int ComponentCount,
		std::vector<std::vector<double>>& Combinations, 
		std::map<double, std::vector<double> >& CutSets,
		AvailabilityHandler  Available);

	static std::vector<std::vector<double> > MinimalCutsets(std::map<double, std::vector<double> >& cutsets);



	static double Availability2ParallelComponents(std::vector<double> v);
	static double Availability4ParallelComponents(std::vector<double> v);
	static double Availability2SeriesComponents(std::vector<double> v);

	static double AtleastOneWorks(double p1, double p2);

	static void displayStates( std::vector<std::vector<double>>);

	static bool IsMultipleTransitions(std::vector<double>& v1, std::vector<double>& v2);
};

#endif