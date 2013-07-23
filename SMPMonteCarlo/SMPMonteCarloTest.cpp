#include "SMPMonteCarloTest.h"
#include "SMPMonteCarlo.h"
#include "../TestResults.h"

void SMPMonteCarloTest::RunModel()
{
	//SingleComponentModel();
	//NCCWTest();
	DegradedComponentTest();
}

//DegradingComponentTestCOV_30.dat
//Contents: Unavailability, 3->0
//varied: cov of 3->2
//DegradedComponentTest(5, cov, 5, 0.8, 3/days, 5/days , 30/days, results, "Plugs");
//cov 0.4, 0.6, 0.8, 1.0
// Format: time 0.4, 0.6, 0.8, 1.0

void  SMPMonteCarloTest::DegradedComponentTest()
{
	const double days = 365.242199;

	std::ostringstream out;
	out << "SMPMonteCarlo\\DegradingComponentTestCOV_" << 30 << ".dat";
	TestResults results(out.str());

	for(double cov=0.4; cov <= 1.0; cov += 0.2)
		DegradedComponentTest(5, cov, 5, 0.8, 3/days, 5/days , 30/days, results, "Plugs");

	results.Serialize();

}

void  SMPMonteCarloTest::DegradedComponentTest(double _mean32, double _cv32, double _mean20, double _cv20, double _muTime13, double _muTime03, double _pmTime, TestResults& result, std::string name)					
{
		std::cout << name << std::endl;

		vector<double> _phi;
		vector<double> _time;

		double _missionTime = 20;
		int _NSteps = 15000;


		JaggedMatrix *jm = 
			new JaggedMatrix(4);

		jm->AddDistribution(3, 2, _mean32, _cv32, DistributionFactory::Weibull);
		jm->AddDistribution(2, 0, _mean20, _cv20, DistributionFactory::Weibull);
		jm->AddDistribution(1, 3, _muTime13, 1.0, DistributionFactory::Weibull);
		jm->AddDistribution(2, 1, _pmTime, 1.0, DistributionFactory::Weibull);
		jm->AddDistribution(0, 3, _muTime03, 1.0, DistributionFactory::Weibull);

		jm->Display();
		
		SemiMarkovModel smp(jm);
		smp.SetModelInput(_missionTime, _NSteps);

		smp.RegisterHandlers(NULL, NULL);
		smp.SetupMatrices();
		smp.ComputeStateProbabilities();

		if(result.IsEmpty())
		{
			_time = smp.GetTimeVector();
			result.AddResult(_time);
		}

		_phi = smp.GetStateProbability(-1,0);
		result.AddResult(_phi);

		//Add all states to the result
		//_phi = smp.GetStateProbability(-1,0);
		//result.AddResult(_phi);
		//_phi = smp.GetStateProbability(-1,1);
		//result.AddResult(_phi);
		//_phi = smp.GetStateProbability(-1,2);
		//result.AddResult(_phi);


		//SMPMonteCarlo MCTest(&smp, -1, 0);
		//MCTest.RunModel(8000000,_missionTime*2, false);

		//std::ostringstream out;
		//out << "SMPMonteCarlo\\DegradingComponentAvailabilityMC_" << 30 << ".dat";
		//TestResults mcresults(out.str());
		//mcresults.AddResult(MCTest._time);
		//mcresults.AddResult(MCTest._phi);
		//mcresults.Serialize();

	}


void SMPMonteCarloTest::SingleComponentModel(double _mean , double _cv , double _muTime, TestResults& results)
{
		double mean[] = { _mean };
		double cv[] =  {  _cv };
		double repcv[] = { 1.0 };
		double muTime[] = { _muTime };
		double _missionTime = 90;
		int _NSteps = 1;
		vector<double> _phiMC;
		vector<double> _timeMC;

		JaggedMatrix *jm =
				new JaggedMatrix(1, 
				mean,cv,
				muTime, repcv, 
				DistributionFactory::Weibull,
				DistributionFactory::Weibull
				);

		jm->Display();

		SemiMarkovModel model(jm);
		model.SetModelInput(_missionTime, _NSteps);

		SMPMonteCarlo MCTest(&model, -1, 0);
		MCTest.RunModel(1000000,(int)_missionTime);
		
		if(results._results.empty())
		{
			results.AddResult(MCTest._time);
		}

		results.AddResult(MCTest._phi);
}



void SMPMonteCarloTest::SingleComponentModel()
{
		double mean[] = {40, 30, 30, 20};
		double cv[] = {1, 0.3, 1, 0.25};
		double rep[] = {0.5,1,0.5,1};

		TestResults results("SMPMonteCarlo\\Component.dat");

		for(int i=0;i<4;i++)
		{
			SingleComponentModel(mean[i] , cv[i] , rep[i], results);
		}
		
		results.Serialize();
}

void SMPMonteCarloTest::NCCWTest()
{
	TestResults results("SMPMonteCarlo\\NCCWPT1.dat");
	NCCWTest(1.0,  results);

	results.Serialize();

}

void SMPMonteCarloTest::NCCWTest(double cov, TestResults& results)
{
	double lambda_1 = 7.7243e-6;
	double lambda_H = 3.4243e-6;
	double mu = 1/19.4;

	JaggedMatrix *jm = new JaggedMatrix(3);
	
	jm->AddDistribution(1,0, 1/lambda_1, cov);
	jm->AddDistribution(1,0, 1/lambda_H, 1);

	jm->AddDistribution(1,2, 1/mu, 1.0);
	jm->AddDistribution(2,0, 1/lambda_H, 1);

	jm->AddDistribution(2,1, 1/lambda_1, cov);
	jm->AddDistribution(2,1, 1/lambda_1, cov);

	jm->Display();

	SemiMarkovModel smp (jm);
	smp.SetModelInput( 100, 30000);


	SMPMonteCarlo MCTest(&smp, -1, 0);
	MCTest.RunModel(1000000,100);
	
	if(results._results.empty())
	{
		results.AddResult(MCTest._time);
	}

	results.AddResult(MCTest._phi);

}