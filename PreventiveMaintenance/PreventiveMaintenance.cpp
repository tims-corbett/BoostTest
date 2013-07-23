#include "PreventiveMaintenance.h"


void PreventiveMaintenanceModel::RunModel()
{
	int k =3;
	double lambdam_inv;
	TestResults results("PreventiveMaintenance\\plots\\pm0.7.dat");

	for(lambdam_inv = 10; lambdam_inv <= 100; lambdam_inv += 10)
		RunModel(k, lambdam_inv, results);

	results.Serialize();

}

double PreventiveMaintenanceModel::_C(double val)
{
	return val/10;
}

void PreventiveMaintenanceModel::RunModel(int k, double lambdam_inv, TestResults& results)
{
	//-----------------------------All units in days
	double mu1_inv = _C(14);
	double mu0_inv	= _C(7);
	double lambda1_inv = _C(1000);
	double lambda0_inv = _C(500);
	double mum_inv = _C(0.5);
	double klambda1_inv = lambda1_inv / k;
	
	//---------------------------Simulation parameters
	double cov = 0.7;
	double _missiontime = _C(1000);
	int _NSteps = 20000;

	//  M1=2, M2=3, ..., Mk = k+1
	//	D1=Mk+1=k+2, D2=k+3, ..., Dk = k+(k+1) = 2k+1

	int nStates = 2*k + 2;
	int F0 = 0;
	int M1 = F0 + 1;
	int Mk = k;
	int D1 = k + 1;
	int Dk = 2*k;
	int F1 = Dk + 1;
	int prevState = D1;
		
	JaggedMatrix *jm = new JaggedMatrix(nStates);

	for(int i=0; i<k; i++)
	{
		jm->AddDistribution(D1+i, F0, lambda0_inv, 1, DistributionFactory::Weibull);
		jm->AddDistribution(D1+i, M1+i, lambdam_inv, 1, DistributionFactory::Weibull);
		jm->AddDistribution(D1+i, D1+i+1, klambda1_inv, cov, DistributionFactory::Weibull);
		jm->AddDistribution(M1+i, prevState, mum_inv, 1, DistributionFactory::Weibull);
		prevState = D1+i;
	}

	jm->AddDistribution(F0, D1, mu0_inv, 1, DistributionFactory::Weibull);
	jm->AddDistribution(F1, D1, mu1_inv, 1, DistributionFactory::Weibull);

	//jm->Serialize("PreventiveMaintenance\\StateSpace.dot");
	jm->Display();

	SemiMarkovModel model(jm);
	model.SetModelInput(_missiontime, _NSteps);

	
	model.SetupMatrices();

	model.ComputeStateProbabilities();
	
	vector<double> _time = model.GetTimeVector();
	vector<double> _phi =	model.GetStateProbability(D1,D1+0) +
							model.GetStateProbability(D1,D1+1) +
							model.GetStateProbability(D1,D1+2);


	if(lambdam_inv==10)
		results.AddResult(_time);

	results.AddResult(_phi);
}

