#include "SystemTrial.h"
#include "Component.h"

void SystemTrial::ConstructTimeLine(boost::ptr_vector<ComponentEvent>& componentEventList, double missionTime)
{

    double cumTime = 0.0, eventTime = 0;
    int eventType;
    int Id;
	ComponentEvent* anEvent;

    for (size_t k = 0; k < _sys->ComponentCount(); k++)        //for each component of the system
	{
		cumTime = 0.0;
		eventType = ComponentEvent::COMPONENT_DOWN;
        Component& component = _sys->GetComponent(k);
        Id = component.GetComponentID();

		while (cumTime <= missionTime)      //simulate failure and repair until mission time
		{
			eventTime = (eventType == ComponentEvent::COMPONENT_DOWN) ?
                component.NextFailureTime() : component.NextRepairCompletionTime();

            //register the event if it occurs within the mission time
            if (cumTime + eventTime <= missionTime)
            {
                anEvent =  new ComponentEvent(cumTime + eventTime,
							eventType, &component, componentEventList);

                componentEventList.push_back(anEvent);
            }
            else
                break;

            if (component.IsRepairable() == false)
                break;

            cumTime += eventTime;
			eventType = (eventType == ComponentEvent::COMPONENT_DOWN) ? 
				ComponentEvent::COMPONENT_UP : ComponentEvent::COMPONENT_DOWN;
		}

	}


	//construct the timeline
	std::sort(componentEventList.begin(), componentEventList.end(),
		ComponentEvent::Compare);

	//DisplayEvents(componentEventList);

}

//Diagnostics
void SystemTrial::DisplayEvents(boost::ptr_vector<ComponentEvent>& componentEventList)
{
	//return;

		for(size_t i=0; i<componentEventList.size();i++)
			componentEventList[i].Display();
		if(componentEventList.size()!=0)
			std::cout << std::endl;
}

TrialResults SystemTrial::AnalyzeTimeline(boost::ptr_vector<ComponentEvent>& componentEventList, double missiontime, bool needOnlyFirstFailure)
{
	TrialResults trialResult;
	trialResult.missiontime = missiontime;
	trialResult.systemFailed = false;
	trialResult.downtime = 0;
	trialResult.uptime = missiontime;
	trialResult.firstfailure = missiontime;
	trialResult.failureCount = 0;
	trialResult.operatingAtEndOfMission = 1;


	if(componentEventList.empty())
		return trialResult;

    //No. of component failures occurring for each cutset
	vector<int> eventCount(_sys->MinimalCutsetLength());
	std::fill(eventCount.begin(), eventCount.end(),0);

	std::vector<int> cutsetMemberList; 
    
    bool systemFailed = false;
	int sysFailureCutsetID=-1;
	int increment=0;
    
    unsigned int NSystemRepairsEncountered = 0; //No. of system repairs - to find average MTTR

	//nth occurrence of the failure. (=1 for non-repairable system)
    unsigned int nthFailure = 0;

	double ttf = 0.0, ttr = 0.0;
	double ttff = ComponentEvent::NO_FAILURE;
    double tbf = 0.0;

	double TimeOfLastSysFailure = 0.0;
	double TimeOfLastSysRepair = 0.0;

    double remainingUT = 0.0;
    double remainingDT = 0.0;

    unsigned int tbfCount = 0;

	ComponentEvent* anEvent;

	for (size_t i = 0; i < componentEventList.size(); i++)
	{
		anEvent = &componentEventList.at(i);
		cutsetMemberList = _sys->GetCutsetAffiliation(anEvent->GetComponent());

        for (size_t j = 0; j < cutsetMemberList.size() ; j++)      //For each cutset
        {
            int cutsetID = cutsetMemberList[j];
			int cutsetlen = _sys->CutsetLength(cutsetID);

            increment = anEvent->EventType(); // +1: Failure; -1: Repair; 0: Null Event

            //Just for debugging, another component fails when system down
			//if (systemFailed && anEvent->EventType() == ComponentEvent::COMPONENT_DOWN)
			//{
			//}


            //if there was a system failure due to this cutset and repaired
            //which means that atleast one component of the cutset has been repaired
			if (cutsetID == sysFailureCutsetID && eventCount[cutsetID]==cutsetlen && increment == ComponentEvent::COMPONENT_UP)
            {
                systemFailed = false;   //system repaired, so restore its status

				//time since last system failure is MTTR
                trialResult.downtime += anEvent->EventEndTime() - TimeOfLastSysFailure;
				
                remainingDT = 0.0;
                TimeOfLastSysRepair = anEvent->EventEndTime();
                remainingUT = missiontime - anEvent->EventEndTime();
                NSystemRepairsEncountered++;
                trialResult.operatingAtEndOfMission = 1;

				//DisplayEvents(componentEventList);

            }

            eventCount[cutsetID] += increment;

			//If all components of this cutset have failed, the system has failed
            if (systemFailed==false && eventCount[cutsetID] == cutsetlen)     //System failure encountered
            {

                nthFailure++;       //Keep a count of system failures

                systemFailed = true;  //system failure irrespective of the cutset
				sysFailureCutsetID = cutsetID;
				trialResult.systemFailed = true;

                TimeOfLastSysFailure = anEvent->EventEndTime();
                trialResult.uptime += anEvent->EventEndTime() - TimeOfLastSysRepair;

                remainingDT = missiontime - anEvent->EventEndTime();

                trialResult.operatingAtEndOfMission = 0;

				if (nthFailure == ComponentEvent::FIRST_FAILURE)     //First occurrence of the failure
				{
                    ttff = anEvent->EventEndTime();
					trialResult.uptime = ttff;
					trialResult.firstfailure = ttff;
					if(needOnlyFirstFailure)
					{
						trialResult.downtime = missiontime-ttff;
						trialResult.systemFailed = true;
						return trialResult;
					}
				}

			}

		}
	}

	trialResult.downtime += remainingDT;
	trialResult.uptime += remainingUT;
	trialResult.failureCount = nthFailure;
	return trialResult;
}