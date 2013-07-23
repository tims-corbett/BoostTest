#include "NCCWShortLife.h"

#include "../SemiMarkovModel.h"

NCCWShortLife::NCCWShortLife()
{
	results = TestResults("NCCWShortLife\\Truncated.dat");
}

/*
%lambda, gamma of wbl
l=10; ga = 3.7;     

%to be truncated from a to b
a = 6; b = 10;

[m v]=wblstat(l, ga);
m; %= 9.0245
v; %= 7.3791
s = sqrt(v); % 2.7164
c = s/m; %= 0.3010
*/
//void NCCWShortLife::TestTruncatedDistribution()
//{
//	double t = 8;
//	double m = 9.0245;
//	double cov = 0.3010;
//
//	Distribution *dist = 
//				DistributionFactory::GetDistribution(DistributionFactory::Weibull, m, cov);
//
//	Distribution *distTrunc = 
//				DistributionFactory::GetDistribution(DistributionFactory::Weibull, m, cov, 6, 10);
//
//
//	std::cout << "pdf = " << dist->pdf(t) << std::endl;
//	std::cout << "Truncated pdf = " << distTrunc->pdf(t) << std::endl << std::endl;
//
//	std::cout << "cdf = " << dist->cdf(t) << std::endl;
//	std::cout << "Truncated cdf = " << distTrunc->cdf(t) << std::endl << std::endl;
//
//	return;
//}


void NCCWShortLife::BuildModel()
{
	/*JaggedMatrix *jm = new JaggedMatrix(2);

	double m = 9.0245;
	double cov = 0.3010;

	Distribution *distTrunc = 
				DistributionFactory::GetDistribution(DistributionFactory::Weibull, m, cov, 6, 10);
	jm->AddDistribution(1,0,distTrunc);

	jm->Display();

	SemiMarkovModel smp (jm);
	smp.SetModelInput( 10, 10000);
	smp.SetupMatrices();

	if(results.IsEmpty())
	{
		time = smp.GetTimeVector();
		results.AddResult(time);
	}

	smp.ComputeStateProbabilities();
	results.AddResult(smp.GetStateProbability(-1,0));*/
}

void NCCWShortLife::RunModel()
{
	//TestTruncatedDistribution();
	//return;

	BuildModel();
	
	//results.Serialize();	
}