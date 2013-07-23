
#include "UGFHenleyModel.h"
#include "..\SemiMarkovModel.h"
#include "..\TestResults.h"

vector<vector<double>> UGFHenleyModel::RunSubsystem(double mean[], double cov[], double rep[])
{
		double _missionTime = 80;
		int _NSteps = 10000;

		JaggedMatrix *jm = 
				new JaggedMatrix(3);

		jm->AddDistribution( 2,1, mean[0],cov[0], DistributionFactory::Weibull);
		jm->AddDistribution( 1,0, mean[1],cov[1], DistributionFactory::Weibull);
		jm->AddDistribution( 1,2, rep[0], 1.0, DistributionFactory::Weibull);
		jm->AddDistribution( 0,1, rep[1], 1.0, DistributionFactory::Weibull);


		SemiMarkovModel model(jm);
		jm->Display();
		model.SetModelInput(_missionTime, _NSteps);
		model.SetupMatrices();
		model.ComputeStateProbabilities();

		vector<vector<double>> F0 = model.GetStateProbabilities(2);

		if( time.empty())
			time = model.GetTimeVector();

		return F0;

}


void UGFHenleyModel::RunModel()
{
	double meanS1[] = {20, 10};
	double meanS2[] = {35, 5};

	double covS[] = {0.3, 0.6};
	double repS[] = {0.5, 0.8};

	vector<vector<double>> F0;
	
	F0 = RunSubsystem(meanS1, covS, repS);
	u1 = UFunction(F0);		
	u2 = UFunction(F0);

	F0 = RunSubsystem(meanS2, covS, repS);
	u3 = UFunction(F0);		
	u4 = UFunction(F0);

	UFunction uS1 = UFunction::UProduct(u1, u2, UFunction::MaxRule);
	UFunction uS2 = UFunction::UProduct(u3, u4, UFunction::MaxRule);

	UFunction S = UFunction::UProduct(uS1, uS2, UFunction::MinRule);

	TestResults results("UGFHenley\\results.dat");
	results.AddResult(time);		// 1
	results.AddResult(u1.p[0]);		// 2
	results.AddResult(u3.p[0]);		// 3	
	results.AddResult(uS1.p[0]);	// 4
	results.AddResult(uS2.p[0]);	// 5
	results.AddResult(S.p[0]);		// 6
	results.Serialize();
}
