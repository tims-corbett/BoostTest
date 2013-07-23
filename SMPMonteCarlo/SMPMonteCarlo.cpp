
#include "SMPMonteCarlo.h"
#include "../SMPUtilities.h"
#include "../threadpool/boost/threadpool.hpp"



SMPMonteCarlo::SMPMonteCarlo(SemiMarkovModel* model, int initState, int targetState)
{
	_initState = (initState==-1) ? model->StateCount-1 : initState;
	_targetState = targetState;
	_smpModel = model;

	
	rng = boost::mt19937( (static_cast<unsigned int>(std::time(0))));

}
 
void SMPMonteCarlo::RunModel(unsigned int repeatCount, int NSteps, bool bReliability)
{
	double missiontime = _smpModel->MissionTime;
	double dt = missiontime / NSteps;
	_time = vector<double>(NSteps + 1);
	_phi = vector<double>(NSteps + 1);
	int idx = 0;

	double avail, avgavail=0, sqSumavail=0, sdavail;

	_time[idx++]=0; 

	int count = 0;

	for(double time = dt; time <= missiontime; time += dt)
	{
		avgavail=0;
		sqSumavail=0;

		for(unsigned long j=0; j < repeatCount; j++)
		{
			avail = Simulate(time, bReliability);
			avgavail += avail;
			sqSumavail += avail * avail;
		}

		avgavail /= repeatCount;
		sdavail = sqrt(sqSumavail/repeatCount - avgavail*avgavail);
		_phi[idx] = 1-avgavail;
		_time[idx++] = time;
		

		//if(idx % 10 == 0)
			std::cout << time << "\t" << 1-avgavail << "(" << sdavail << ")" << std::endl;
	}
}

//returns unavailability truncated to N steps with in missiontime
//double SMPMonteCarlo::RunModel(unsigned int N, double missiontime)

int SMPMonteCarlo::Simulate(double missiontime, bool bReliability)
{
	static boost::uniform_01<boost::mt19937> zeroone(rng);

	unsigned int currentState = _initState;
	unsigned int nextState;
	double downtime = 0;
	double sojournTime;		//time spent in a state	
	double time=0;
	double minSojournTime;		//min. time spent in current state
	double p;
	bool nextStateFound = false;
	double doublemax = 999999.0;

	while(time < missiontime)
	{
		p = zeroone();
		minSojournTime = doublemax;
		nextStateFound = false;

		for(unsigned int s = 0; s < _smpModel->StateCount; s++)
		{
			if(s == currentState)
				continue;

			sojournTime = _smpModel->SampleTimeSpent(currentState, s, p);		
			
			if(sojournTime != 0 && sojournTime < minSojournTime)
			{
				minSojournTime = sojournTime;
				nextState = s;
				nextStateFound = true;
			}
		}

		if(nextStateFound == false)
			continue;

		if( time + minSojournTime > missiontime)
			break;


		currentState = nextState;


		if(currentState == _targetState && bReliability == true )
		{
			break;
		}

		time += minSojournTime;
	}

	int operatingAtEndOfMission = 1;

	if(currentState == _targetState)
		operatingAtEndOfMission = 0;

	return operatingAtEndOfMission;
}