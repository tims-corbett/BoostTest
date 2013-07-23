#include "SMPMonteCarloTailGas.h"
#include "../SMPMonteCarlo/SMPMonteCarlo.h"
#include "../TestResults.h"

void SMPMonteCarloTailGas::RunModel()
{
	TestResults results("TailGasMS\\TailGasMSMC.dat");

	_missionTime = 20;
	_NSteps = 15000;
	const double days = 365.242199;

	DegradedComponentTest(4, 0.6, 1, 0.9,  7.0/days, results, "Booster fan");	//Booster fan
	DegradedComponentTest(8, 0.4, 2, 0.8, 15.0/days,results,"Quench pumps");		//Quench pumps
	DegradedComponentTest(8, 0.3, 2, 0.8, 15.0/days,results, "Feedwater pump");		//Feedwater pump
	DegradedComponentTest(8, 0.6, 2, 0.8, 15.0/days,results, "Circulation pump");	//Circulation pump
	DegradedComponentTest(4, 0.4, 1, 0.9, 7.0/days,results, "Filter");				//Filter

	results.Serialize();
}

void  SMPMonteCarloTailGas::DegradedComponentTest(double _mean21, double _cv21, double _mean10, double _cv10, double _muTime, TestResults& results, std::string name)
{
		std::cout << name << std::endl;

		vector<double> _phi;
		vector<double> _time;


		JaggedMatrix *jm = 
			new JaggedMatrix(3);


		jm->AddDistribution(2,1, _mean21, _cv21, DistributionFactory::Weibull);
		jm->AddDistribution(1,0, _mean10, _cv10, DistributionFactory::Weibull);
		jm->AddDistribution(1,2, _muTime, 1.0, DistributionFactory::Weibull);

		jm->Display();
		
		SemiMarkovModel smp(jm);
		smp.SetModelInput(_missionTime, _NSteps);


	SMPMonteCarlo MCTest(&smp, -1, 0);
	MCTest.RunModel(1000000,_missionTime);
	
	if(results.IsEmpty())
	{
		results.AddResult(MCTest._time);
	}

	results.AddResult(MCTest._phi);

}