#include "MonteCarloTest.h"

#include "System.h"
#include "Component.h"
#include "../TestResults.h"

void MonteCarloTest::RunTests()
{
	//SingleNonRepairableComponentReliabilityTest();
	//SingleRepairableComponentAvailabilityTest();
	//HenleySystemTest();
	//SteadyStateTest();
	//HenleySubsystemTest(1);
	//HenleySubsystemTest(2);

	NCCWTest();
}

void MonteCarloTest::SingleNonRepairableComponentReliabilityTest()
{
	//TESTCASE
	//RBD with failuremean = mu, cov = 1
	//Reliability:	R = exp(-(1/mu) * t)
	//MATLAB:	R = 1-expcdf(t,mu)
	//Test:		mu=10; t=5; 
	//Result:	R =   0.6065, failure = 0.3935
	//Monte-C:	TESTED


	Component* c1 = new Component(0, "C1", 10, 1);		//2hrs, fig.3.2 of thesis

	Cutset* mincutset = new Cutset(0);
	mincutset->AddComponent(c1);

	System s;
	s.AddComponent(c1);
	s.AddMinimalCutset(mincutset);

	s.SimulateReliability(5, 1000, 30);

}

MonteCarloTest::~MonteCarloTest()
{
	std::cout << "~MonteCarloTest" << std::endl;
}


void MonteCarloTest::SingleRepairableComponentAvailabilityTest()
{
	//TESTCASE
	//at t=90
	//failuremean = 40, cov = 1, mu = 0.5, Unavail: 0.0123	TESTED
	//failuremean = 30, cov = 0.3, mu = 1, Unavail: 0.0323


	Component* c = new Component(0, "C1", 40, 1, 0.5);		//0.0123
	//Component* c = new Component(0, "C2", 30, 0.3, 1);		//0.0323
	//Component* c = new Component(0, "C3", 30, 1, 0.5);		//0.0164
	//Component* c = new Component(0, "C4", 20, 0.25, 1);	//0.0476	

	Cutset* mincutset = new Cutset(0);
	mincutset->AddComponent(c);

	System s;
	s.AddComponent(c);
	s.AddMinimalCutset(mincutset);

	double init=90;
	double step=1;
	double mission=90;

	TestResults result("MonteCarlo\\plots\\SteadyC2.dat");
	
	
	s.SimulateReliability(init, step, mission, 1000000, 1, result);
	
	result.Serialize();

}

void MonteCarloTest::SteadyStateTest()
{
	//TESTCASE
	//at t=90
	//failuremean = 40, cov = 1, mu = 0.5, Unavail: 0.0123	TESTED
	//failuremean = 30, cov = 0.3, mu = 1, Unavail: 0.0323


	//Component* c = new Component(0, "C1", 40, 1, 0.5);		//0.0123
	Component* c = new Component(0, "C2", 30, 0.3, 1);		//0.0323
	//Component* c = new Component(0, "C3", 30, 1, 0.5);		//0.0164
	//Component* c = new Component(0, "C4", 20, 0.25, 1);	//0.0476	

	Cutset* mincutset = new Cutset(0);
	mincutset->AddComponent(c);

	System s;
	s.AddComponent(c);
	s.AddMinimalCutset(mincutset);

	double init=250;
	double step=1;
	double mission=250;
	int N = 10000;

	TestResults result("MonteCarlo\\plots\\test.dat");
	TestResults resultC2("MonteCarlo\\plots\\SteadyC2.dat");


	vector<double> key(N);
	vector<double> val(N);

	for(int n=0; n<N; n+=500	)	
	{
		s.SimulateReliability(init, step, mission, n, 1, result);
		key[n]=n;
		//val[n]=result._results[2][];

	}
	resultC2.AddResult(key);
	resultC2.AddResult(val);
	resultC2.Serialize();
	
}

void MonteCarloTest::HenleySystemTest()
{
	//TESTCASE
	//at t=90
	//system unavail:	3.9729e-004

	Component* c1 = new Component(0, "C1", 40, 1, 0.5);		//0.0123
	Component* c2 = new Component(0, "C2", 30, 0.3, 1);		//0.0323
	Component* c3 = new Component(0, "C3", 30, 1, 0.5);		//0.0164
	Component* c4 = new Component(0, "C4", 20, 0.25, 1);	//0.0476	

	Cutset* cutset0 = new Cutset(0);
	cutset0->AddComponent(c1);
	cutset0->AddComponent(c2);

	Cutset* cutset1 = new Cutset(1);
	cutset1->AddComponent(c3);
	cutset1->AddComponent(c4);

	System s;
	s.AddComponent(c1);
	s.AddComponent(c2);
	s.AddComponent(c3);
	s.AddComponent(c4);
	s.AddMinimalCutset(cutset0);
	s.AddMinimalCutset(cutset1);

	double init=90;
	double step=1;
	double mission=90;

	TestResults result("MonteCarlo\\plots\\System.dat");
	s.SimulateReliability(init, step, mission, 1000000, 1, result);
	result.Serialize();
}


void MonteCarloTest::NCCWTest()
{
	double lambda_1 = 7.7243e-6;		//pump train
	double lambda_H = 3.4243e-6;		//heat exhanger
	double mu = 1/19.4;					//pump train repair time


	Component* c1 = new Component(0, "Pump Train 1", 1/lambda_1, 1, 1/mu);		
	Component* c2 = new Component(0, "Pump Train 2", 1/lambda_1, 1, 1/mu);		
	Component* c3 = new Component(0, "Heat Exchanger", 1/lambda_H, 1.1);		

	Cutset* cutset0 = new Cutset(0);
	cutset0->AddComponent(c1);
	cutset0->AddComponent(c2);

	Cutset* cutset1 = new Cutset(1);
	cutset1->AddComponent(c3);

	System s;
	s.AddComponent(c1);
	s.AddComponent(c2);
	s.AddComponent(c3);
	s.AddMinimalCutset(cutset0);
	s.AddMinimalCutset(cutset1);

	double init=1000;
	double step=1;
	double mission=1000;

	TestResults result("MonteCarlo\\plots\\NCCWSystemPT0.3.dat");
	s.SimulateReliability(init, step, mission, 10000000, 1, result);
	result.Serialize();
}

void MonteCarloTest::HenleySubsystemTest(int subsys)
{

	Component* c1;
	Component* c2; 
	//Component* c1 = new Component(0, "C3", 30, 1, 0.5);		//0.0164
	//Component* c2 = new Component(0, "C4", 20, 0.25, 1);	//0.0476	

	if(subsys==1)
	{
	 c1 = new Component(0, "C1", 40, 1, 0.5);		//0.0123
	 c2 = new Component(0, "C2", 30, 0.3, 1);		//0.0323
	}
	else
	{
		c1 = new Component(0, "C3", 30, 1, 0.5);		//0.0164
		c2 = new Component(0, "C4", 20, 0.25, 1);	//0.0476	
	}

	Cutset* cutset0 = new Cutset(0);
	cutset0->AddComponent(c1);
	cutset0->AddComponent(c2);

	System s;
	s.AddComponent(c1);
	s.AddComponent(c2);
	s.AddMinimalCutset(cutset0);

	double init=0;
	double step=1;
	double mission=90;

	std::ostringstream file;
	file << "MonteCarlo\\plots\\SubSystem" << subsys << ".dat";
	TestResults result(file.str());


	s.SimulateReliability(init, step, mission, 800000, 1, result);
	result.Serialize();
	
}


void main()
{
	MonteCarloTest mctest;
	mctest.RunTests();
}
