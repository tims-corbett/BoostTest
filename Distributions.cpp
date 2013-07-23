#include "Distributions.h"

#include <boost/math/special_functions/gamma.hpp>


double WeibullDistribution::GetShape(double cov)
{
	double COVShapeMap[][2] = {
		{ 0.2,        5.79771499997173},
		{ 0.25,        4.54241199979626},
		{ 0.3,        3.71390899968045},
		{ 0.35,        3.12889399959868},
		{ 0.4,        2.69569599953813},
		{ 0.45,        2.36338299949168},
		{ 0.5,        2.10139499945506},
		{ 0.55,        1.89028999944991},
		{ 0.6,        1.71711399946416},
		{ 0.65,        1.57290899947602},
		{ 0.7,        1.45128599948603},
		{ 0.75,        1.34756999949456},
		{ 0.8,        1.25826499950191},
		{ 0.85,        1.18070999950829},
		{ 0.9,        1.11283999951387},
		{ 0.95,        1.05303599951879},
		{ 1,			1.00000000000},
		{ 1.05,        0.952723999521795},
		{ 1.1,        0.910336999520577},
		{ 1.15,        0.872158999519479},
		{ 1.2,        0.837617999518485},
		{ 1.25,        0.806238999517583},
		{ 1.3,        0.77762399951676},
		{ 1.35,        0.751435999516007},
		{ 1.4,        0.727388999515316},
		{ 1.45,        0.705238999514679}
	};

	double covStart = COVShapeMap[0][0];
	double covDiff =  0.05;

	double i = (cov - covStart) / covDiff;

	int index = (int)i;
	if( i-int(i) > 0.025 )
		index++;
	
	//int index = ceil(i);

	return COVShapeMap[index][1];
}

//WeibullDistribution::WeibullDistribution(double mean, double cov, double x1, double x2):
//	  Distribution(x1, x2),
//	  _wblObject(weibull_distribution<>(1.0, 1.0))
//{
// SetParameters(mean, cov);
//}

WeibullDistribution::WeibullDistribution(double mean, double cov, int dummy):
			_wblObject( weibull_distribution<>(1.0, 1.0))
{
 SetParameters(mean, cov);
}


void WeibullDistribution::SetParameters(double mean, double cov)
{
 Distribution::SetParameters(mean, cov);

 double vShape = GetShape(cov);
 double vScale = mean / boost::math::tgamma(1 + 1 / vShape) ;

 _wblObject = weibull_distribution<>(vShape, vScale);

 _name = "Wbl";
}

double WeibullDistribution::GetScale()
{
	return _wblObject.scale();
}

double WeibullDistribution::GetShape()
{
	return _wblObject.shape();
}


void WeibullDistribution::DisplayParameters()
{
	std::cout << "[" << _wblObject.scale() << "," << _wblObject.shape() << "],";
}

double WeibullDistribution::pdf(const double& t)
{
	double originalPdf = 0.0;

	if(t==0 && _wblObject.shape()  == 1)
		originalPdf = (1.0 / _wblObject.scale());
	else
		originalPdf = boost::math::pdf(_wblObject, t);

		return originalPdf;

}


double WeibullDistribution::cdf(const double& t)
{
	double originalCdf = 0.0;
	originalCdf = boost::math::cdf(_wblObject,t);

		return originalCdf;

	

}

void PolyWeibullDistribution::DisplayParameters() 
{ 
	std::cout<< _count << "[" << _lambda << "," << _gamma << "],";
}

PolyWeibullDistribution::PolyWeibullDistribution(double scale, double shape, int n)
{
	_wblObject = WeibullDistribution(scale, shape);
	_count = n;

	_lambda = 1/_wblObject.GetScale();
	_gamma = _wblObject.GetShape();

}

PolyWeibullDistribution::PolyWeibullDistribution(double mean, double cov, int n, int dummy)
{
	_wblObject = WeibullDistribution(mean, cov, dummy);
	_count = n;

	_lambda = 1/_wblObject.GetScale();
	_gamma = _wblObject.GetShape();


}

double PolyWeibullDistribution::pdf(const double& t)
{

	double lambda_t = _lambda * t;
	double suffix = exp(-_count * pow( lambda_t, _gamma));

	double prefix = _count * (_lambda*_gamma) * pow(lambda_t, _gamma-1);

	return prefix * suffix;
}

double PolyWeibullDistribution::cdf(const double& t)
{
	double base = pow( _lambda * t, _gamma);
	return 1-exp(-_count * base);
}

//https://projects.coin-or.org/svn/LEMON/trunk/lemon/random.h

void LogNormalDistribution::SetParameters(double mean, double cov) 
{ 
  double lg = log(1+cov*cov);
  double new_mean = log(mean)-lg/2;
  double new_sd = sqrt(lg);
  _lognmlObject = lognormal(new_mean ,  new_sd );
   // _lognmlObject = lognormal( log(1/0.438), 0.315 );
  _name = "Lognormal";
}


void UniformDistribution::SetParameters(double mean, double cov)
{ 
	double sd = mean *cov;
	double b = sd*sqrt(3.0) + mean;
	double a = 2*mean - b;
	_uniformObject = uniform_distribution<>(a, b);
	_name="Uniform";
}


UniformDistribution::UniformDistribution(double mean, double cov, int dummy)
{
 SetParameters(mean, cov);
}
