#include "FlemingPipingMonteCarlo.h"
#include "../SMPMonteCarlo/SMPMonteCarlo.h"
#include "../TestResults.h"


FlemingPipingMonteCarlo::FlemingPipingMonteCarlo()
{
	phi = 1/4.35e-4;
	omega = 1/2.1e-2;
	lambdaF = 1/1.79e-4;
	rhoF = 1/9.53e-6;
	mu = 1/7.92e-1;
	rhoL = 1/1.97e-2;

	R = 3;
	L = 2;
	F = 1;
	S = 0;
}

void FlemingPipingMonteCarlo::RunModel()
{
		std::ostringstream fpath;
		
		//FlemingPipingMC_Wbl.dat: Col 1 - time; Col 2..cov 1.3,1.0,0.7
		fpath << "FlemingPipingMonteCarlo\\FlemingPipingMC_Wbl.dat";
		TestResults sysresults(fpath.str());

		for(double cov = 1.3; cov >= 0.7; cov -= 0.3)
		{
			std::cout << std::endl << "COV: " << cov << std::endl;
			RunModel(cov, sysresults);
		}

		sysresults.Serialize();
}


void FlemingPipingMonteCarlo::RunModel(double cov, TestResults& results)
{
		double missiontime = 60;

		JaggedMatrix *jmWbl = new JaggedMatrix(4);		//SFLR

		jmWbl->AddDistribution(S, F, phi, cov,	DistributionFactory::Weibull);
			

		jmWbl->AddDistribution(F, S, omega, 1);
		jmWbl->AddDistribution(F, L, lambdaF, 1);
		jmWbl->AddDistribution(L, R, rhoL, 1);
		
		jmWbl->AddDistribution(F, R, rhoF, 1);
		jmWbl->AddDistribution(L, S, mu, 1);

		jmWbl->Display();
		jmWbl->DisplayInputs();

		SemiMarkovModel smpWbl(jmWbl);
		smpWbl.SetModelInput(missiontime, 20000);

		SMPMonteCarlo MCTest(&smpWbl, S, R);
		MCTest.RunModel(10000000, missiontime);
		
		if(results._results.empty())
		{
			results.AddResult(MCTest._time);
		}

		results.AddResult(MCTest._phi);
}