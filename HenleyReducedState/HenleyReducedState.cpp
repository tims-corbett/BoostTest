#include "HenleyReducedState.h"

#include "../TestResults.h"


HenleyReducedState::HenleyReducedState()
{

}

void HenleyReducedState::RunModel(double cov)
{
	double mean = 30;
	double rep = 0.5;

	std::ostringstream fpath;
	fpath << "HenleyReducedState\\HenleyWbl" << cov << ".dat";

	JaggedMatrix *jmWbl = new JaggedMatrix(3);	

	jmWbl->AddDistribution(0, 1, rep, 1,	DistributionFactory::Weibull);
	jmWbl->AddDistribution(0, 1, rep, 1,	DistributionFactory::Weibull);
	jmWbl->AddDistribution(1, 2, rep, 1,	DistributionFactory::Weibull);

	jmWbl->AddDistribution(1, 0, mean, cov,	DistributionFactory::Weibull);
	jmWbl->AddDistribution(2, 1, mean, cov,	DistributionFactory::Weibull);
	jmWbl->AddDistribution(2, 1, mean, cov,	DistributionFactory::Weibull);

	jmWbl->Display();
	jmWbl->DisplayInputs();

	SemiMarkovModel smpWbl(jmWbl);
	smpWbl.SetModelInput(60, 25000);
	smpWbl.SetupMatrices();
	smpWbl.RegisterHandlers(NULL, NULL);
	smpWbl.ComputeStateProbabilities();

	vector<double> time = smpWbl.GetTimeVector();
	vector<double> vWblS = smpWbl.GetStateProbability(2,0);

	TestResults sysresults(fpath.str());
	
	sysresults.AddResult(time);
	sysresults.AddResult(vWblS);

	sysresults.Serialize( );

}

void HenleyReducedState::RunModel()
{
	for( double cov = 0.3; cov <= 1.3; cov += 0.1)
		RunModel(cov);
}