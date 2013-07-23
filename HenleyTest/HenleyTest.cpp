#include "HenleyTest.h"
#include "../SMPMonteCarlo/SMPMonteCarlo.h"

HenleyTest::HenleyTest(std::string desc, double missiontime, int N)
{
	_missionTime = missiontime;
	_NSteps = N;
	_desc = desc;

	std::cout << desc << "..." << std::endl;
}


void  HenleyTest::SingleComponentTest(double _mean, double _cv, double _muTime)					
{
		double mean[] = { _mean };
		double cv[] =  {  _cv };
		double repcv[] = { 1.0 };
		double muTime[] = { _muTime };
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

		model.IntegralSystemEquations("HenleyTest\\matlab\\TwoStatesEqn.txt");


		model.SetupMatrices();

		//SMPMonteCarlo MCTest(&model, -1, 0);
		//MCTest.RunModel(100000);
		//_phiMC = MCTest._phi;
		//_timeMC = MCTest._time;

		model.ComputeStateProbabilities();
		
		_time = model.GetTimeVector();
		_phi = model.GetStateProbability(-1,0);
	}

void HenleyTest::OneRepairCrew_1o2(double mean[], double cv[], double rep[])
{
	JaggedMatrix *jm = new JaggedMatrix(5);

	jm->AddDistribution(0,1,mean[0], cv[0], DistributionFactory::Weibull);
	jm->AddDistribution(0,2,mean[1], cv[1], DistributionFactory::Weibull);

	jm->AddDistribution(1,0,rep[0],	 1.0, DistributionFactory::Weibull);
	jm->AddDistribution(1,3,mean[1], cv[1], DistributionFactory::Weibull);

	jm->AddDistribution(2,0,rep[1], 1.0, DistributionFactory::Weibull);
	jm->AddDistribution(2,4,mean[0], cv[0], DistributionFactory::Weibull);

	jm->AddDistribution(3,2,rep[0],	 1.0, DistributionFactory::Weibull);
	jm->AddDistribution(4,1,rep[1],  1.0, DistributionFactory::Weibull);
		
	jm->Display();
	
	SemiMarkovModel model(jm);
	model.SetModelInput(_missionTime, _NSteps);


	//jm->Serialize("..\\matlab\\" + _desc + ".dot");
	model.SetupMatrices();		
	model.ComputeStateProbabilities();

	_time = model.GetTimeVector();
	_phi = model.GetStateProbability(0, 3) +
			model.GetStateProbability(0, 4);
}

void HenleyTest::ExplosionModel(int n,double mean[],double cv[], double rep[])
{
		int componentCount = n;

		//Single bloated model
		//JaggedMatrix *jm =
		//		new JaggedMatrix(componentCount,
		//						mean, cv, 
		//						rep, cv,
		//						DistributionFactory::Weibull,
		//						DistributionFactory::Weibull
		//						);

		JaggedMatrix *jm = new JaggedMatrix(4);

		jm->AddDistribution(3,1,mean[0], cv[0], DistributionFactory::Weibull);
		jm->AddDistribution(1,3,rep[0], 1.0, DistributionFactory::Weibull);

		jm->AddDistribution(2,0,mean[0], cv[0], DistributionFactory::Weibull);
		jm->AddDistribution(0,2,rep[0], 1.0, DistributionFactory::Weibull);
		jm->AddDistribution(0,2,rep[0], 1.0, DistributionFactory::Weibull);

		jm->AddDistribution(3,2,mean[1], cv[1], DistributionFactory::Weibull);
		jm->AddDistribution(2,3,rep[1], 1.0, DistributionFactory::Weibull);

		jm->AddDistribution(1,0,mean[1], cv[1], DistributionFactory::Weibull);
		jm->AddDistribution(0,1,rep[1], 1.0, DistributionFactory::Weibull);
		jm->AddDistribution(0,1,rep[1], 1.0, DistributionFactory::Weibull);


		jm->Display();
		
		SemiMarkovModel model(jm);
		model.SetModelInput(_missionTime, _NSteps);

	
		//jm->Serialize("HenleyTest\\matlab\\" + _desc + ".dot");
		model.SetupMatrices();		
		model.ComputeStateProbabilities();

		//SMPMonteCarlo MCTest(&model, -1, 0);
		//MCTest.RunModel(100000);
		//_phiMC = MCTest._phi;
		//_timeMC = MCTest._time;

		_time = model.GetTimeVector();
		_phi = model.GetStateProbability(-1, 0);


}

void HenleyTest::TwoIdenticalComponentsThreeStates(double mean, double cov, double muTime)
	{
		JaggedMatrix *jm = 
			new JaggedMatrix(3);

		jm->AddDistribution(0,1, muTime, 1.0, DistributionFactory::Weibull);
		jm->AddDistribution(0,1, muTime, 1.0, DistributionFactory::Weibull);
		jm->AddDistribution(1,0, mean, cov, DistributionFactory::Weibull);

		jm->AddDistribution(1,2, muTime, 1.0, DistributionFactory::Weibull);
		jm->AddDistribution(2,1, mean, cov, DistributionFactory::Weibull);

		jm->Display();
		

		SemiMarkovModel smp(jm);
		smp.SetModelInput(_missionTime, _NSteps);

		smp.IntegralSystemEquations("HenleyTest\\matlab\\ThreeStatesEqn.txt");

		smp.RegisterHandlers(NULL, NULL);
		smp.SetupMatrices();
		smp.ComputeStateProbabilities();

		_phi = smp.GetStateProbability(-1,0);
		_time = smp.GetTimeVector();

		TestResults sysresults("HenleyTest\\matlab\\IdenticalExpExpSystem.dat");
		sysresults.AddResult(_time);
		sysresults.AddResult(_phi);
		sysresults.Serialize();


	}


	vector<double> HenleyTest::ComposeSeries(HenleyTest& subsys1, HenleyTest& subsys2, int type)
	{
		vector<double> result;
		
		if(type == resulttype::type_smp)
		{
			result = vector<double>(subsys1._phi.size());

			//same as 1-(1-Q1)(1-Q2)
			//subsystem unavailability
			result.assign( subsys1._phi + subsys2._phi - 
					element_prod(subsys1._phi, subsys2._phi) );
		}
		else
		{
			result = vector<double>(subsys1._phiMC.size());

			result.assign( subsys1._phiMC + subsys2._phiMC - 
					element_prod(subsys1._phiMC, subsys2._phiMC) );
		}

		return result;
	}



void HenleyTest::HandleComponents(double mean[], double cv[], double rep[], double _missionTime, int _NSteps)
{
	std::string fname;

	if(mean[0]==mean[1] && mean[1]==mean[2])
		fname = "HenleyTest\\matlab\\IdenticalExpExpComponents.dat";
	else
		fname = "HenleyTest\\matlab\\ExpExpComponents.dat";

	TestResults componentres(fname);
	TestResults MCcomponentres("HenleyTest\\matlab\\MCWblExpComponents.dat");

	HenleyTest componentTest[4];
	vector<double> phi;

	int nComponentsToTest = 1;

	for(int i=0; i < nComponentsToTest; i++)
	{
		componentTest[i] = HenleyTest("Component" , _missionTime, _NSteps);
		componentTest[i].SingleComponentTest(mean[i], cv[i], rep[i]);
		
		if(i==0)
		{
			componentres.AddResult(componentTest[i]._time);
			//MCcomponentres.AddResult(componentTest[i]._timeMC);
		}

		componentres.AddResult(componentTest[i]._phi);
		//MCcomponentres.AddResult(componentTest[i]._phiMC);
	}
		componentres.AddResult(componentTest[0]._phi);
		componentres.AddResult(componentTest[0]._phi);
		componentres.AddResult(componentTest[0]._phi);
	componentres.Serialize();
	//MCcomponentres.Serialize();
}

void HenleyTest::HandleSubsystems(double mean[], double cv[], double rep[],  double mission, int nsteps)
{
	double S1mean[] = { mean[0], mean[1]};
	double S1cv[] =  { cv[0],cv[1]};
	double S1rep[] = { rep[0], rep[1] };

	double S2mean[] = { mean[2],mean[3]};
	double S2cv[] =  { cv[2],cv[3]};
	double S2rep[] = { rep[2], rep[3]};

	
	HenleyTest Subsys1("Subsystem 1", mission, nsteps);
	HenleyTest Subsys2("Subsystem 2", mission,nsteps);

	Subsys1.ExplosionModel(2, S1mean, S1cv, S1rep);
	Subsys2.ExplosionModel(2, S2mean, S2cv, S2rep);
	
	TestResults sysresults("HenleyTest\\matlab\\IdenticalExpExpSystem.dat");
	//TestResults mcsysresults("HenleyTest\\matlab\\MCExpExpSystem.dat");

	sysresults.AddResult(Subsys1._time);
	sysresults.AddResult(HenleyTest::ComposeSeries(Subsys1, Subsys2, resulttype::type_smp));
	sysresults.AddResult(Subsys1._phi);
	sysresults.AddResult(Subsys2._phi);
	
	//mcsysresults.AddResult(Subsys1._timeMC);
	//mcsysresults.AddResult(HenleyTest::ComposeSeries(Subsys1, Subsys2, resulttype::type_montecarlo));
	//mcsysresults.AddResult(Subsys1._phiMC);
	//mcsysresults.AddResult(Subsys2._phiMC);

	sysresults.Serialize();
	//mcsysresults.Serialize();
}


void HenleyTest::RunModel()
{
	double mean[] = {40, 30, 30, 20};
	double cv[] = {1, 0.3, 1, 0.25};
	//double cv[] = {1, 1, 1, 1};
	double rep[] = {0.5,1,0.5,1};

	_missionTime = 60;
	_NSteps = 20000;

	//double mean[] = {30, 30, 30, 30};
	//double cv[] = {0.9, 0.9, 0.9, 0.9};
	////double cv[] = {1, 1, 1, 1};
	//double rep[] = {0.5,0.5,0.5, 0.5};


	TwoIdenticalComponentsThreeStates(mean[1], cv[1], rep[1]);
//	HandleSubsystems(mean, cv, rep, _missionTime, _NSteps);
	//HandleComponents(mean, cv, rep, _missionTime, _NSteps);
}
