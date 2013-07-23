#ifndef MC_COMPONENT
#define MC_COMPONENT

#include <boost/numeric/ublas/vector.hpp>
using namespace boost::numeric::ublas;

class Cutset;

#include "../Distributions.h"
#include "Cutset.h"


class Component
{
	Distribution* _failureDist;
	Distribution* _repairDist;

	//Component Identification
	int _componentID;
	std::string _componentName;

	double _failuremean, _failurecov;
	double _repairmean, _repaircov;

	std::vector<int> _cutsetAffiliation;

public:

	Component(int componentID, std::string componentName, 
		double failuremean, double failurecov,
		double _repairmean = 0, double _repaircov = 1,
		DistributionFactory::DistributionType failuredist = DistributionFactory::Weibull,
		DistributionFactory::DistributionType repairdist = DistributionFactory::Weibull );


	double NextFailureTime();
	double NextRepairCompletionTime();

	void AddCutsetAffiliation(Cutset* cutset);

	bool IsRepairable() { return _repairmean!=0.0; }
	int GetComponentID() { return _componentID; }
	std::string ComponentName();
	std::vector<int> GetCutsetAffiliation() { return _cutsetAffiliation; }
};



#endif