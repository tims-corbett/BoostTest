#include "Distributions.h"

Distribution* DistributionFactory::GetDistribution(DistributionFactory::DistributionType type,  double mean, double cov, int weight)
{
	if( mean == 0)
		return NULL;

	Distribution* dist = NULL;

		
	switch(type)
	{
	case PolyWeibull:
		dist = new PolyWeibullDistribution(mean, cov, weight,0);
	}

	return dist;

}


//Distribution* DistributionFactory::GetDistribution(
//									 DistributionType type,  
//									 double mean, 
//									 double cov,
//									 double truncX1,
//									 double truncX2)
//{
//
//	if( mean == 0)
//		return NULL;
//
//	Distribution* dist = NULL;
//
//	switch(type)
//	{
//	case DistributionType::Weibull:
//		dist =  new WeibullDistribution(mean, cov, truncX1, truncX2);
//		 break;
//	}
//
//	return dist;
//
//
//}


Distribution* DistributionFactory::GetDistribution(DistributionFactory::DistributionType type,  double mean, double cov)
{
	if( mean == 0)
		return NULL;

	Distribution* dist = NULL;

	switch(type)
	{
	case Weibull:
		dist =  new WeibullDistribution(mean, cov, 0);
		 break;

	case Exponential:
			dist =  new ExponentialDistribution(mean);
			break;

	case Normal:
		dist =  new NormalDistribution(mean, cov, 0);
		 break;

	case LogNormal:
		dist =  new LogNormalDistribution(mean, cov, 0);
		 break;

	case Gamma:
		dist =  new GammaDistribution(mean, cov, 0);
		 break;

	case GumbelMax:
		 dist =  new GumbelMaxDistribution(mean, cov, 0);
		 break;

	case Logistic:
		dist =  new LogisticDistribution(mean, cov, 0);
		 break;

	case Uniform:
		dist =  new UniformDistribution(mean, cov, 0);
		 break;
	}

	//dist->SetMean(mean, cov);	

	return dist;
}