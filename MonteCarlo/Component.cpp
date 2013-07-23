#include "Component.h"

Component::Component(int componentID, std::string componentName, 
		double failuremean, double failurecov,
		double repairmean, double repaircov, 
		DistributionFactory::DistributionType failuredist,
		DistributionFactory::DistributionType repairdist)

{
	_componentID = componentID;
	_componentName = componentName;
	_failuremean = failuremean;
	_failurecov =  failurecov;
	_repairmean = repairmean;
	_repaircov = repaircov;

	_failureDist = DistributionFactory::GetDistribution(failuredist, _failuremean, _failurecov);
	_repairDist = NULL;

	if(_repairmean != 0)
		_repairDist = DistributionFactory::GetDistribution(repairdist, _repairmean, _repaircov);

}

std::string Component::ComponentName()
{
	return _componentName;
}


double Component::NextFailureTime()
{
	return _failureDist->InverseTransform();
}

double Component::NextRepairCompletionTime()
{
	return _repairDist->InverseTransform();
}

void Component::AddCutsetAffiliation(Cutset* cutset)
{
	_cutsetAffiliation.push_back(cutset->CutsetID());
}