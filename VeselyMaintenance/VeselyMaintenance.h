#ifndef VESELY_MAINTENANCE_H
#define VESELY_MAINTENANCE_H


#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"

//Vesely, W.E., 1993. Quantifying maintenance eects on unavailability and
//risk using markov modeling. Reliability Engineering and System Safety
//41, 177 - 187.


class VeselyMaintenance
{
	
public:
	
	VeselyMaintenance();

	void TestDistributions();
	void RunModel(double degRatio, double gamma, double T_m, int TestCase);

	inline double lambda()		{ return 1.0E-6;}
	inline double mu()			{ return 1/lambda(); }
	
	inline double f_of()		{ return 0.1; }
	inline double lambda_of()	{ return f_of() * lambda(); }
	inline double mu_of()		{ return mu()/f_of();  }
	inline double gamma_of(double gamma)	{ return gamma; }
	
	inline double lambda_od(double r_od)	{ return r_od * lambda(); }	
	inline double mu_od(double r_od)		{ return mu()/r_od; }
	inline double gamma_od(double gamma)	{ return gamma; }
	
	inline double r_df(double r_od)			{ return r_od*(1-f_of())/ (r_od-(1-f_of())); }
	inline double lambda_df(double r_od)	{ return lambda()*r_df(r_od); }
	inline double mu_df(double r_od)		{ return (1.0/r_df(r_od))*mu();   }
	inline double gamma_df(double gamma)	{ return gamma;   }

	//inline double lambda_om(double T_m)	{ return exp(-(1-f_of())*lambda()*T_m)/T_m; }
	inline double lambda_om(double T_m, double r_od)	{ return exp(-(lambda_of()+lambda_od(r_od))*T_m)/T_m; }
	inline double mu_om(double T_m, double r_od)		{ return 1.0/lambda_om(T_m, r_od); }
	
	inline double lambda_dm(double T_m, double r_od)	{ return exp(-lambda_df(r_od) * T_m * 0.5) / (T_m * 0.5); }	
	inline double mu_dm(double T_m, double r_od)		{ return 1.0 / lambda_dm(T_m, r_od); }
	
	inline double p_mo()		{ return 0.9990; }
	inline double p_md()		{ return 0.0009; }
	inline double p_mf()		{ return 0.0001; }

	inline double d_m()			{ return 72.0; }
	
	inline double lambda_mo()	{ return p_mo()/d_m(); }
	inline double mu_mo()		{ return 1.0/lambda_mo(); }
		
	inline double lambda_md()	{ return p_md()/d_m(); }
	inline double mu_md()		{ return 1.0/lambda_md(); }
	
	inline double lambda_mf()	{ return p_mf()/d_m(); }
	inline double mu_mf()		{ return 1.0/lambda_mf(); }

	inline double p_fd()		{ return 0.01; }
	inline double p_fo()		{ return 0.99; }

	inline double r()			{ return 20.0; }

	inline double T()			{ return 730.0; }
	inline double d_f(double T_m)	{ return  (3.0*T_m > T()) ?  0.5*T()* (1.0-T()/(3.0*T_m)) + r(): 0.5*T()* (1.0-T_m/(3.0*T())) + r(); }

	inline double lambda_fo(double T_m)	{ return p_fo() / d_f(T_m); }
	inline double mu_fo(double T_m)		{ return 1.0/lambda_fo(T_m);}

	inline double lambda_fd(double T_m)	{ return p_fd() / d_f(T_m); }
	inline double mu_fd(double T_m)		{ return 1.0/lambda_fd(T_m); }

};

#endif