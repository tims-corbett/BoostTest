#ifndef TRIAL_RESULTS
#define TRIAL_RESULTS

struct TrialResults
{
	double uptime;
	double downtime;
	double missiontime;
	double firstfailure;
	bool systemFailed;
	int operatingAtEndOfMission;
	int failureCount;

};

#endif