#ifndef COMPONENT_EVENT
#define COMPONENT_EVENT

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/numeric/ublas/vector.hpp>
using namespace boost::numeric::ublas;

class Component;

class ComponentEvent
{

private:
	double _eventEndTime;
	int _eventType;
	int _componentId;
	Component* _component;

public:
	ComponentEvent(double _eventEndTime, int _eventType, Component* component,  boost::ptr_vector<ComponentEvent>& componentEventList);
	static bool Compare(const ComponentEvent& e1, const ComponentEvent& e2);
	int ComponentID() ;
	Component* GetComponent();
	int EventType();
	double EventEndTime();

    static enum CONSTANTS
    {
        NO_FAILURE = -1,
        NO_REPAIR = -1,
        COMPONENT_DOWN = 1,
        COMPONENT_UP = -1,
        NULL_EVENT = 0,
        FIRST_FAILURE = 1
    };

	//Diagnostics
	void Display();
};

#endif