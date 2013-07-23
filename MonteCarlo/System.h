#ifndef MC_SYSTEM
#define MC_SYSTEM

#include <boost/ptr_container/ptr_vector.hpp>
#include<list>

#include "Component.h"

class TestResults;

class System
{
	std::vector<boost::shared_ptr<Component>>	_components;
	std::vector<boost::shared_ptr<Cutset>>		_mincutsets;

	size_t _componentCount;

public:
	System() { _componentCount=0;}

	vector<double> SimulateReliability(double missiontime, unsigned int innerLoopCount, unsigned int outerLoopCount);
	void SimulateReliability(double init, double step, double missiontime, unsigned int innerLoopCount, unsigned int outerLoopCount, TestResults& results);


	void AddComponent(Component* component);
	Component& GetComponent(int i);
	void SetMinimalCutsets(std::vector<boost::shared_ptr<Cutset>> cutsets);
	void AddMinimalCutset(Cutset* cutset);
	size_t ComponentCount() { return _componentCount; }
	std::vector<int> GetCutsetAffiliation(Component* component);

	size_t MinimalCutsetLength() { return _mincutsets.size(); }
	int CutsetLength(int cutsetID);
};

#endif