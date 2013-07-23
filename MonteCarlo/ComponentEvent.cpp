#include "ComponentEvent.h"
#include "Component.h"

ComponentEvent::ComponentEvent(double eventEndTime, int eventType, Component* component,  boost::ptr_vector<ComponentEvent>& componentEventList)
{
		_eventEndTime = eventEndTime;
        _eventType = eventType;
		_component = component;
        _componentId = component->GetComponentID();
}

void ComponentEvent::Display()
{
	std::cout << this << "\t" << _component->ComponentName() << "\t" << _eventType << "\t" << _eventEndTime  << std::endl;
}


bool ComponentEvent::Compare(const ComponentEvent& e1, const ComponentEvent& e2)
{
	return ( e1._eventEndTime < e2._eventEndTime);
}

int ComponentEvent::ComponentID() 
{ 
	return _componentId; 
}

double ComponentEvent::EventEndTime() 
{
	return _eventEndTime; 
}

int ComponentEvent::EventType() 
{ 
	return _eventType; 
}

Component* ComponentEvent::GetComponent()
{
	return _component;
}