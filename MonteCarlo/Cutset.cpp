#include "Component.h"
#include "Cutset.h"


Cutset::Cutset(int id) 
{
	_cutsetID = id; 
	_cutsetLength = 0;
}

void Cutset::AddComponent(Component* component)
{
	_components.push_back(component->GetComponentID());
	_cutsetLength++;
	component->AddCutsetAffiliation(this);

}

int Cutset::length()
{
	 return _cutsetLength; 
}

int Cutset::CutsetID() 
{ 
	return _cutsetID; 
}
