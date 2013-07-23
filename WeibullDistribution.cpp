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

	int index = ceil((cov - covStart) / covDiff);

	return COVShapeMap[index][1];
}


void WeibullDistribution::SetParameters(double mean, double cov)
{
 double vShape = GetShape(cov);
 double vScale = mean / boost::math::tgamma(1 + 1 / vShape) ;

 _wblObject = weibull_distribution<>(vShape, vScale);

 _name = "Wbl";
}



void WeibullDistribution::DisplayParameters()
{
	std::cout << "[" << _wblObject.scale() << "," << _wblObject.shape() << "],";
}

double WeibullDistribution::pdf(const double& t)
{
	if(t==0 && _wblObject.shape()  == 1)
		return (_wblObject.shape() / _wblObject.scale());

	return boost::math::pdf(_wblObject, t);
}