#include "System.h"
#include "TrialResults.h"
#include "SystemTrial.h"
#include "../TestResults.h"

void System::AddComponent(Component* component) 
{ 
	_components.push_back(boost::shared_ptr<Component>(component)); 
	_componentCount = _components.size();
}

void System::SimulateReliability(double init, double step, double missiontime, unsigned int innerLoopCount, unsigned int outerLoopCount, TestResults& results)
{
	int arrSize = (int) ((missiontime-init)/step);

	if(init == 0 || missiontime==init)
		arrSize++;

	vector<double> time(arrSize);
	vector<double> steadyUnavail(arrSize);
	vector<double> variance(arrSize);
	vector<double> pointUnavail(arrSize);

	int i=0;
	vector<double> result(3);

	for(double t=init; t<=missiontime; t+= step)
	{
		result = SimulateReliability(t, innerLoopCount, outerLoopCount);
		pointUnavail[i]=result[0];
		steadyUnavail[i] = result[1];
		variance[i] = result[2];
		time(i++)=t;
	}

	results.AddResult(time);
	results.AddResult(pointUnavail);
	results.AddResult(steadyUnavail);
	results.AddResult(variance);
}

vector<double> System::SimulateReliability( double missiontime, unsigned int innerLoopCount, unsigned int outerLoopCount)
{
	vector<double> result(3);
	std::fill(result.begin(),result.end(),0);
	
		if(missiontime==0)
		return result;

	TrialResults trialresult;
	unsigned int failures = 0;
	double pointAvail, 
			steadyAvail, avgSteadyAvail, varSteadyAvail, sqSumSteadyAvail;
	double downtime, avgDowntime=0, varDowntime, sqSumDowntime=0;
	double nFailures, avgNFailures=0, varNFailures, sqSumNFailures=0;
	double mtbf, avgMtbf=0, varMtbf, sqSumMtbf=0;
	double ttff, avgTtff=0, varTtff, sqSumTtff=0;

//	boost::threadpool::pool tpool(100);

     //for (unsigned int i = 0; i < outerLoopCount; i++)
	 //{
		pointAvail = 0;
		sqSumSteadyAvail=0;
		avgSteadyAvail=0;
        for (unsigned int j = 0; j <= innerLoopCount; j++ )
		{
			SystemTrial systrial(this);
			boost::ptr_vector<ComponentEvent> timeline;
			systrial.ConstructTimeLine(timeline, missiontime);
			trialresult = systrial.AnalyzeTimeline(timeline, missiontime, false);
			
			steadyAvail = trialresult.downtime / (trialresult.uptime + trialresult.downtime);
			avgSteadyAvail += steadyAvail;
			sqSumSteadyAvail += steadyAvail*steadyAvail; 

			pointAvail += trialresult.operatingAtEndOfMission;

			nFailures = trialresult.failureCount;
			avgNFailures += nFailures;
			sqSumNFailures += nFailures * nFailures;

			downtime = trialresult.downtime;
			avgDowntime += downtime;
			sqSumDowntime += downtime * downtime;

			if(nFailures != 0)
			{
				failures++;
				mtbf = missiontime / nFailures;
				avgMtbf += mtbf;
				sqSumMtbf += mtbf * mtbf;

				ttff = trialresult.firstfailure;
				avgTtff += ttff;
				sqSumTtff += ttff * ttff;
			}
		}

		pointAvail /=  innerLoopCount;

		avgSteadyAvail /= innerLoopCount;
		varSteadyAvail = sqrt(sqSumSteadyAvail/innerLoopCount - avgSteadyAvail*avgSteadyAvail);

		avgNFailures /= innerLoopCount;
		varNFailures = sqrt(sqSumNFailures/innerLoopCount - avgNFailures*avgNFailures);

		avgDowntime /= innerLoopCount;
		varDowntime = sqrt(sqSumDowntime/innerLoopCount - avgDowntime*avgDowntime);

		avgMtbf /= failures;
		varMtbf = sqrt(sqSumMtbf/failures - avgMtbf*avgMtbf);

		avgTtff /= innerLoopCount;
		varTtff = sqrt(sqSumTtff/innerLoopCount - avgTtff*avgTtff);

		std::cout << "mt  Av  steady  N  DT  mtbf ttff" << std::endl;

		std::cout << missiontime << " " << 1-pointAvail << " " << 
						avgSteadyAvail << "(" << varSteadyAvail << ") " << 
						avgNFailures << "(" << varNFailures << ") " <<
						avgDowntime << "(" << varDowntime << ") " <<
						avgMtbf << "(" << varMtbf << ")" << 
						avgTtff << "(" << varTtff << ")" <<
						std::endl;

		result[1]=avgSteadyAvail;
		result[2]=varSteadyAvail;
		result[0]=1-pointAvail;

	 //}
	 return result;

}


void System::AddMinimalCutset(Cutset* cutset) 
{
	boost::shared_ptr<Cutset> node(cutset);
	_mincutsets.push_back(node); 
}

Component& System::GetComponent(int i) 
{ 
	return *_components[i].get(); 
}


void System::SetMinimalCutsets(std::vector<boost::shared_ptr<Cutset>> cutsets) 
{ 
	_mincutsets = cutsets; 
}

std::vector<int> System::GetCutsetAffiliation(Component* component)
{
	return component->GetCutsetAffiliation();
}

int System::CutsetLength(int cutsetID)
{
	return (_mincutsets[cutsetID].get())->length();
}