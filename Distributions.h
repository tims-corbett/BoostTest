#ifndef DISTRIBUTIONS
#define DISTRIBUTIONS

#include <boost/math/distributions/weibull.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/lognormal.hpp>
#include <boost/math/distributions/gamma.hpp>
#include <boost/math/distributions/extreme_value.hpp>
#include <boost/math/distributions/logistic.hpp>
#include <boost/math/distributions/exponential.hpp>
#include <boost/math/distributions/uniform.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include<math.h>
#include<string>
#include<iostream>

#include "SMPUtilities.h"

using namespace boost::numeric::ublas;
using namespace boost::math;

class Distribution
{
protected:	
	double _mean, _cov;
	std::string _name;
  
public:
	Distribution() {  }
	

	virtual double	pdf(const double& time) { return 0;}
	virtual double	cdf(const double& time) { return 0;}

	virtual void SetParameters(double mean, double cov) { _mean=mean; _cov=cov;}

	void SetMean(double mean, double cov) { _mean=mean; _cov=cov; }
	
	virtual std::string Notation()  
	{
		std::ostringstream out;
		out << "(" << _mean << ", " << _cov << ")";
		return out.str();
	}

	virtual std::string MarkovTransitionRate()  
	{
		std::ostringstream out;
		
		if(_mean == 0)
			out << "0";
		else
			out << 1/_mean;
		return out.str();
	}


	virtual double InverseTransform() { return 0; }
	virtual double InverseTransform(double p) { return 0;}



	virtual ~Distribution() {  }

	//Diagnostics
	virtual void DisplayParameters() { }
};

/*************
Matlab code for truncated pdf and cdf

F = @(x) wblcdf(x, l, ga);
f = @(x) wblpdf(x, l, ga);
g = @(x) ((x>=a) & (x<=b)).*f(x);

%Truncated
tf = @(x) g(x)./(F(b)-F(a));
tF = @(x) (x<a).*0 + ((x>=a) & (x<=b)).*(F(x)-F(a))./(F(b)-F(a)) + (x>b).*1;
**************/

class WeibullDistribution : public Distribution
{
protected:
	weibull_distribution<> _wblObject;
	
public:

	WeibullDistribution(): 
	  _wblObject( weibull_distribution<>(1.0, 1.0)) {}

	WeibullDistribution(double scale, double shape):
	  _wblObject(weibull_distribution<>(shape, scale)) { }

	/*WeibullDistribution(double mean, double cov, double x1, double x2);*/

	WeibullDistribution(double mean, double cov, int dummy);


	static double GetShape(double cov);

		double GetShape();
		double GetScale();

	  virtual double  pdf(const double& t);
	  virtual  double cdf(const double& t);

	  virtual void SetParameters(double mean, double cov);

	  virtual double InverseTransform() { return quantile(_wblObject,SMPUtilities::rand01()); }
	  virtual double InverseTransform(double p) { return quantile(_wblObject, p); }

	  virtual ~WeibullDistribution() {  }

	  //Diagnostics
	  virtual void DisplayParameters();
};


class PolyWeibullDistribution : public Distribution
{
protected:
	WeibullDistribution _wblObject;
	int _count;
	double _lambda, _gamma;

public:

	PolyWeibullDistribution(double scale, double shape, int n);
	PolyWeibullDistribution(double mean, double cov, int n, int dummy);

	virtual double pdf(const double& t);
	virtual double cdf(const double& t);

	virtual void DisplayParameters();

	virtual ~PolyWeibullDistribution() { }

};

class ExponentialDistribution : public Distribution
{
	exponential _expObject;

public:

	ExponentialDistribution(): 
	  _expObject(1.0) {}

	ExponentialDistribution(double lambda):
	  _expObject(lambda) { }


	  virtual double  pdf(const double& t) { return boost::math::pdf(_expObject,t); }
	  virtual  double cdf(const double& t) { return boost::math::cdf(_expObject,t); }

	  virtual void SetParameters(double mean, double cov) 
	  { 
		  _expObject = exponential(mean);
		  _name="Exp";
	  }


  	  virtual double InverseTransform() { return quantile(_expObject, SMPUtilities::rand01()); }
	  virtual double InverseTransform(double p) { return quantile(_expObject, p); }

	  virtual ~ExponentialDistribution() {  }



	  //Diagnostics
	  virtual void DisplayParameters() { std::cout << "[" << _expObject.lambda() <<  "],"; }
};

class UniformDistribution : public Distribution
{
	uniform_distribution<> _uniformObject;

public:

	UniformDistribution(): 
	  _uniformObject() {}

	UniformDistribution(double a, double b):
	  _uniformObject(a,b) { }

  	UniformDistribution(double mean, double cov, int dummy);


	  virtual double  pdf(const double& t) { return boost::math::pdf(_uniformObject,t); }
	  virtual  double cdf(const double& t) { return boost::math::cdf(_uniformObject,t); }

	  virtual void SetParameters(double mean, double cov); 

  	  virtual double InverseTransform() { return quantile(_uniformObject, SMPUtilities::rand01()); }
	  virtual double InverseTransform(double p) { return quantile(_uniformObject, p); }

	  virtual ~UniformDistribution() {  }



	  //Diagnostics
	  virtual void DisplayParameters() { std::cout << "[" << _uniformObject.lower() << ", " <<_uniformObject.upper() << "],"; }

};


class NormalDistribution : public Distribution
{
	normal _nmlObject;
	
public:

		NormalDistribution(): 
		  _nmlObject( normal(1.0, 1.0)) {}

		NormalDistribution(double mean, double sd):
		  _nmlObject(normal(mean, sd)) { }

		  NormalDistribution(double mean, double cov, int dummy)
					{ SetParameters(mean,cov);  }

	  virtual double  pdf(const double& t) { return boost::math::pdf(_nmlObject,t); }
	  virtual double  cdf(const double& t) { return boost::math::cdf(_nmlObject,t); }

	  virtual void SetParameters(double mean, double cov) { _nmlObject = normal(mean, mean*cov); _name="Normal";  }

  	  virtual double InverseTransform() { return quantile(_nmlObject, SMPUtilities::rand01()); }
	  virtual double InverseTransform(double p) { return quantile(_nmlObject, p); }


	  virtual ~NormalDistribution() {  }

	  //Diagnostics
	  virtual void DisplayParameters() {std::cout << "[" << _nmlObject.mean() << "," << _nmlObject.standard_deviation() << "],";}
};


class LogNormalDistribution : public Distribution
{
	lognormal _lognmlObject;
	
public:

		LogNormalDistribution(): 
		  _lognmlObject( lognormal(0.0, 1.0)) {}

		LogNormalDistribution(double mean, double sd):
		  _lognmlObject(lognormal(mean, sd)) { }

		  LogNormalDistribution(double mean, double cov, int dummy)
							   { SetParameters(mean,cov); }

	  virtual double  pdf(const double& t) { return boost::math::pdf(_lognmlObject,t); }
	  virtual double  cdf(const double& t) { return boost::math::cdf(_lognmlObject,t); }
	  virtual void SetParameters(double mean, double cov) ;
  	  virtual double InverseTransform() { return quantile(_lognmlObject, SMPUtilities::rand01()); }
	  virtual double InverseTransform(double p) { return quantile(_lognmlObject, p); }

	  virtual ~LogNormalDistribution() {  }

	  //Diagnostics
	  virtual void DisplayParameters() {std::cout << _lognmlObject.location() << "," << _lognmlObject.scale() << std::endl;}
};


class GammaDistribution : public Distribution
{
	gamma_distribution<> _gammaObject;
	
public:

		GammaDistribution(): 
		  _gammaObject( gamma_distribution<>(1.0, 1.0)) {}

		GammaDistribution(double scale, double shape):
		  _gammaObject(gamma_distribution<>(shape, scale)) { }

		  GammaDistribution(double mean, double cov, int dummy):
				_gammaObject( gamma_distribution<>(1.0, 1.0))
							   { SetParameters(mean,cov); }

	  virtual double  pdf(const double& t) { return boost::math::pdf(_gammaObject,t); }
	  virtual double  cdf(const double& t) { return boost::math::cdf(_gammaObject,t); }

	  virtual void SetParameters(double mean, double cov) 
	  { 
		  double k = 1/(cov*cov);
		  _gammaObject = gamma_distribution<>(k, mean/k );
		  _name = "Gamma";
	  }

  	  virtual double InverseTransform() { return quantile(_gammaObject, SMPUtilities::rand01()); }
	  virtual double InverseTransform(double p) { return quantile(_gammaObject, p); }


	  virtual ~GammaDistribution() {  }

	  //Diagnostics
	  virtual void DisplayParameters() {std::cout << "[" << _gammaObject.scale() << "," << _gammaObject.shape() << "],";}
};


class GumbelMaxDistribution : public Distribution
{
	extreme_value _gumbelObject;

public:

		GumbelMaxDistribution(): 
		  _gumbelObject( extreme_value(0.0, 1.0)) {}

		GumbelMaxDistribution(double location, double scale):
		  _gumbelObject(extreme_value(location, scale)) { }

		  GumbelMaxDistribution(double mean, double cov, int dummy):
				_gumbelObject( extreme_value(0.0, 1.0))
							   { SetParameters(mean,cov); }

	  virtual double  pdf(const double& t) { return boost::math::pdf(_gumbelObject,t); }
	  virtual double  cdf(const double& t) { return boost::math::cdf(_gumbelObject,t); }


	  virtual void SetParameters(double mean, double cov) 
	  { 
		 //EulerMascheroniConstant
		double EMConstant = 0.5772156649015328606;
		double PI = 3.1415926535897932384626433;

		double beta = mean * cov * sqrt(6.0) /  PI ;
		double mu = mean - beta * EMConstant;

		 _gumbelObject = extreme_value(mu, beta);
		 _name = "Gumbel";
	  }

	  virtual double InverseTransform() { return quantile(_gumbelObject, SMPUtilities::rand01()); }
	  virtual double InverseTransform(double p) { return quantile(_gumbelObject, p); }


	  virtual ~GumbelMaxDistribution() {  }

	  //Diagnostics
	  virtual void DisplayParameters() {std::cout << "[" << _gumbelObject.location() << "," << _gumbelObject.scale() << "],";}
};


class LogisticDistribution : public Distribution
{
	logistic _logisticObject;

public:

		LogisticDistribution(): 
		  _logisticObject( logistic(0.0, 1.0)) {}

		LogisticDistribution(double location, double scale):
		  _logisticObject(logistic(location, scale)) { }

		  LogisticDistribution(double mean, double cov, int dummy):
				_logisticObject( logistic(0.0, 1.0))
							   { SetParameters(mean,cov); }

	  virtual double  pdf(const double& t) { return boost::math::pdf(_logisticObject,t); }
	  virtual double  cdf(const double& t) { return boost::math::cdf(_logisticObject,t); }


	  virtual void SetParameters(double mean, double cov) 
	  { 
		double PI = 3.1415926535897932384626433;
		 _logisticObject = logistic(mean, (cov*mean*sqrt(3.0)/PI));
		 _name = "Logistic";
	  }

  	  virtual double InverseTransform() { return quantile(_logisticObject, SMPUtilities::rand01()); }
	  virtual double InverseTransform(double p) { return quantile(_logisticObject, p); }



	  virtual ~LogisticDistribution() {  }

	  //Diagnostics
	  virtual void DisplayParameters() {std::cout << "[" << _logisticObject.location() << "," << _logisticObject.scale() << "],";}
};



class DistributionFactory
{
public:
static enum DistributionType{Uniform, Exponential, Gamma, GumbelMax, Logistic, LogNormal, Normal, Weibull, PolyWeibull } ;

static Distribution* GetDistribution(
									 DistributionType type,  
									 double mean, 
									 double cov);


static Distribution* GetDistribution(DistributionType type,  
									 double mean, 
									 double cov, 
									 int weight);
};

#endif