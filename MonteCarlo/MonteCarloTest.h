#ifndef MC_TEST
#define MC_TEST

class Component;
class Cutset;

class MonteCarloTest
{
public:
	void RunTests();
	void SingleNonRepairableComponentReliabilityTest();
	~MonteCarloTest();
	void SingleRepairableComponentAvailabilityTest();
	void HenleySystemTest();
	void HenleySubsystemTest(int subsys);
	void SteadyStateTest();
	void NCCWTest();
};

#endif