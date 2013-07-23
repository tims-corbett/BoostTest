#include "../Distributions.h"
#include "DigitalSystems.h"
#include "../TestResults.h"


//All transition rate per hour

const double m12 = 1/25.5e-8;		// Correct -> Low
	const double c12 = 1;

const double m13 = 1/4.2e-5;		//Correct -> Prev
	const double c13 = 0.6;

const double m14 = 1/5.5e-8;		// Correct -> High
	const double c14 = 1;

const double m15 = 1/5.5e-8;		// Correct -> Arb.
	const double c15 = 1;

const double m23 = 1/4.2e-5;		// Low -> Prev
const double m43 = 1/4.2e-5;		// High -> Prev
const double m53 = 1/4.2e-5;		// Arb -> Prev


FeedWaterController::FeedWaterController()
{

}


void FeedWaterController::TestDistributions()
{
	Distribution *dist = 
		DistributionFactory::GetDistribution(DistributionFactory::Weibull, m12, c12);

	double t= 5;
	double wblCdf = dist->cdf(t);
	double wblPdf = dist->pdf(t);

	// t = 10, mean = 2, sd=5
	std::cout << "Wbl cdf = " <<  wblCdf << std::endl;	// 0.9452
	std::cout << "Wbl pdf = " << wblPdf << std::endl;	// 0.0222

	return;

}



//State 1 - Correct output
//State 2 - Low Output
//State 3 - Previous Output
//State 4 - High Output
//State 5 - Arbitrary Output



void FeedWaterController::RunModel()
{
	//TestDistributions();
	RunSMP();
}


void FeedWaterController::RunSMP()
{
		std::ostringstream fpath;
		
		fpath << "DigitalSystems\\FeedWaterController_0.6_Prev.dat";

		int nStates = 5;

		JaggedMatrix *jmWbl = new JaggedMatrix(nStates);

		Distribution *d12 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m12,c12);
		jmWbl->AddDistribution(0,1,d12);


		Distribution *d13 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m13,c13);
		jmWbl->AddDistribution(0,2,d13);

		
		Distribution *d14 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m14,c14);
		jmWbl->AddDistribution(0, 3, d14);


		Distribution *d15 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m15,c15);
		jmWbl->AddDistribution(0,4,d15);


		Distribution *d23 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m23,1);
		jmWbl->AddDistribution(1,2,d23);


		Distribution *d43 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m43,1);
		jmWbl->AddDistribution(3,2,d43);


		Distribution *d53 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m53,1);
		jmWbl->AddDistribution(4,2,d53);


//------------------------------------------------------------------------------------------


		jmWbl->Display();
		//jmWbl->DisplayInputs();

		SemiMarkovModel smpWbl(jmWbl);
		smpWbl.SetModelInput(24, 15000);
		smpWbl.SetupMatrices();

		smpWbl.ComputeStateProbabilities();

		vector<double> time = smpWbl.GetTimeVector();
		vector<double> p12 = smpWbl.GetStateProbability(0,1);
		vector<double> p13 = smpWbl.GetStateProbability(0,2);
		vector<double> p14 = smpWbl.GetStateProbability(0,3);
		vector<double> p15 = smpWbl.GetStateProbability(0,4);


		TestResults sysresults(fpath.str());
		
		sysresults.AddResult(time);
		sysresults.AddResult(p12);
		sysresults.AddResult(p13);
		sysresults.AddResult(p14);
		sysresults.AddResult(p15);
		
		sysresults.Serialize( );

}

void main()
{
		FeedWaterController model;
		model.RunModel();
}