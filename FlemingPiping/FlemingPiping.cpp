#include "FlemingPiping.h"
#include "../TestResults.h"

FlemingPiping::FlemingPiping()
{
	double T_FI = 10;
	double T_LI = 1;

	phi = 1/4.35e-4;
	omega = 1 / ((0.25) * (0.90)/(T_FI + (200/8760)));		// 1/2.1e-2 for T_FI = 10
	lambdaF = 1/1.79e-4;
	rhoF = 1/9.53e-6;
	mu = 1/((0.90)*(0.90)/(T_LI + (200/8760)));						//1/7.92e-1;
	rhoL = 1/1.97e-2;

	R = 3;
	L = 2;
	F = 1;
	S = 0;
			
}

void FlemingPiping::RunWeibullModel(double cov)
{
		std::ostringstream fpath;
		
		fpath << "FlemingPiping\\FlemingPipingWbl_TLI_" << cov << ".dat";

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
		smpWbl.SetModelInput(100, 20000);
		smpWbl.SetupMatrices();
		smpWbl.ComputeStateProbabilities();

		vWblS = smpWbl.GetStateProbability(S,S);
		vWblF = smpWbl.GetStateProbability(S,F);
		vWblL = smpWbl.GetStateProbability(S,L);
		vWblR = smpWbl.GetStateProbability(S,R);

		time = smpWbl.GetTimeVector();

		TestResults sysresults(fpath.str());
		
		sysresults.AddResult(time);
		sysresults.AddResult(vWblS);
		sysresults.AddResult(vWblF);
		sysresults.AddResult(vWblL);
		sysresults.AddResult(vWblR);

		sysresults.Serialize( );


}



void FlemingPiping::RunModel()
{
	//for(double cov = 1.1; cov <= 1.4; cov += 0.1)
		RunWeibullModel(0.6);
}


void main()
{
	FlemingPiping pipingModel;
	pipingModel.RunModel();

}
