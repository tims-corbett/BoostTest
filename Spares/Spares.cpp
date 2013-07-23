#include "../Distributions.h"
#include "Spares.h"
#include "../TestResults.h"

Spares::Spares()
{
	failureMean = 1/0.03;
	failureCOV = 0.4;
	repairMean = 1/4.0;			
}


void Spares::TestPolyWeibull()
{
	int nTransformers = 12;

	Distribution *dist = 
				DistributionFactory::GetDistribution(DistributionFactory::PolyWeibull, failureMean, failureCOV, nTransformers);

	double t= 10;
	double polyWblCdf = dist->cdf(t);
	double expCdf = 1 - exp(-nTransformers * (1/failureMean ) * t);	// 1 - exp(-n * \lambda * t)

	double polyWblPdf = dist->pdf(t);
	double expPdf =  nTransformers * (1/failureMean ) * exp(-nTransformers * (1/failureMean ) * t);	// n*\lambda*exp(-n * \lambda * t)


	std::cout << "PolyWbl cdf = " << polyWblCdf << std::endl;
	std::cout << "Exp cdf = " << expCdf << std::endl << std::endl;

	std::cout << "PolyWbl pdf = " << polyWblPdf << std::endl;
	std::cout << "Exp pdf = " << expPdf << std::endl;


	return;

}


void Spares::RunModel(int nTransformers, int nSpares)
{
		std::ostringstream fpath;
		
		fpath << "Spares\\Spares_" << nTransformers << "_" << nSpares << "_" << failureCOV << ".dat";

		int nStates = nSpares + 2;
		int spareNo = nSpares;

		JaggedMatrix *jmWbl = new JaggedMatrix(nStates);

		for(int i = 1; i < nStates; i++)
		{
			Distribution *dist = 
				DistributionFactory::GetDistribution(DistributionFactory::PolyWeibull, failureMean, failureCOV, nTransformers);

			jmWbl->AddDistribution(i, i-1, dist);

			if( i != 1 && nSpares > 0)
			{
				Distribution *distRep = DistributionFactory::GetDistribution(DistributionFactory::PolyWeibull, repairMean, 1.0, spareNo);
				jmWbl->AddDistribution(i-1, i, distRep);
				spareNo--;

			}
		}
			
		jmWbl->Display();
		//jmWbl->DisplayInputs();
		//jmWbl->GetMarkovTransitionMatrix();
		//jmWbl->Serialize("E:\\Dumps\\SparesViz.dot");

		SemiMarkovModel smpWbl(jmWbl);
		smpWbl.SetModelInput(40, 20000);
		smpWbl.SetupMatrices();

		//smpWbl.IntegralSystemEquations("E:\\Civil-7\\Research\\multi-state\\Proposal\\Chapter 5 - SMP\\VCProjects\\BoostTest\\BoostTest\\Spares\\Spares.Eqn.txt");

		smpWbl.ComputeStateProbabilities();

		vector<double> vWblS = smpWbl.GetStateProbability(nStates-1, 0);


		vector<double> time = smpWbl.GetTimeVector();

		TestResults sysresults(fpath.str());
		
		sysresults.AddResult(time);
		sysresults.AddResult(vWblS);

		sysresults.Serialize( );
}



void Spares::RunModel()
{
	//TestPolyWeibull();


	int nTransformers = 12;
	int nSpares = 4;

	for(int i = 0; i<= nSpares; i++)
		RunModel(nTransformers, i);
}


void main()
{
	Spares model;
	model.RunModel();
}