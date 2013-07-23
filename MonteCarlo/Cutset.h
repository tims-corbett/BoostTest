#ifndef CUTSET
#define CUTSET

#include <boost/shared_ptr.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include<list>

using namespace boost::numeric::ublas;

class Component;
//#include "Component.h"

class Cutset
{
	std::vector<int> _components;
	int _cutsetLength;
	int _cutsetID;

public:

	Cutset(int id);

	void AddComponent(Component* component);

	int length();
	int CutsetID();

	

};



#endif