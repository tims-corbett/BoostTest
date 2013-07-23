#ifndef SYSTEM_TRIAL
#define SYSTEM_TRIAL

#include "System.h"
#include "ComponentEvent.h"
#include "TrialResults.h"

class SystemTrial
{
	System* _sys;

public:
	SystemTrial(System* sys) { _sys = sys; }
	void ConstructTimeLine(boost::ptr_vector<ComponentEvent>& componentEventList, double missionTime);
	TrialResults AnalyzeTimeline(boost::ptr_vector<ComponentEvent>& timeline, double missiontime, bool needOnlyFirstFailure);

	//Diagnostics
	void DisplayEvents(boost::ptr_vector<ComponentEvent>& componentEventList);

};

#endif