#include <boost/bind.hpp>

#include "BiroliniHenley.h"
#include "../SemiMarkovModel.h"
#include "../TestResults.h"

BiroliniHenley::BiroliniHenley()
{
	_mission = 60;
	_nsteps = 25000;

	mttf = 30;
	ttfCov = 0.9;
	mttr = 0.5;
	ttrCov = 1;
}

void BiroliniHenley::RunModel()
{
	BuildOldModel();
	BuildNewModel();
	RunSystem();
	RunComponent();
}

void BiroliniHenley::RunSystem()
{
	_smp->ComputeStateProbabilities();
	vector<double> clock = _smp->GetTimeVector();

	vector<double> ones(clock.size());
	std::fill(ones.begin(), ones.end(), 1.0);

	vector<double> p =	ones - (
						_smp->GetStateProbability(0, 0) +
						_smp->GetStateProbability(0, 1) +
						_smp->GetStateProbability(0, 2) );		//UnAvavailability

	std::stringstream fpath;
	fpath << "BiroliniHenley\\WblExpRedundantSystem"  << ".dat";

	TestResults result(fpath.str());
	result.AddResult(clock);
	result.AddResult(p);
	result.Serialize();	
}

void BiroliniHenley::BuildOldModel()
{
	_oldModel = new JaggedMatrix(4);
	_oldModel->AddDistribution(0,1,mttf, ttfCov); 
	_oldModel->AddDistribution(1,0,mttr, ttrCov);
	_oldModel->AddDistribution(0,2,mttf, ttfCov); 
	_oldModel->AddDistribution(2,0,mttr, ttrCov);
	_oldModel->AddDistribution(1,3,mttf, ttfCov);
	_oldModel->AddDistribution(3,1,mttr, ttrCov);
	_oldModel->AddDistribution(2,3,mttf, ttfCov);
	_oldModel->AddDistribution(3,2,mttr, ttrCov);
	std::cout << "Old Model" << std::endl;
	_oldModel->Display();

}

void BiroliniHenley::BuildNewModel()
{
	_newModel = new JaggedMatrix(3);
	_newModel->AddDistribution(0,1,mttf, ttfCov); 
	_newModel->AddDistribution(1,0,mttr, ttrCov);
	_newModel->AddDistribution(0,2,mttf, ttfCov); 
	_newModel->AddDistribution(2,0,mttr, ttrCov);
	_newModel->AddDistribution(1,1,mttf, ttfCov);
	_newModel->AddDistribution(2,2,mttf, ttfCov);
	std::cout << "New Model" << std::endl;
	_newModel->Display();

	_smp = new SemiMarkovModel(_newModel);
	_smp->SetModelInput(_mission, _nsteps);

	_smp->RegisterHandlers(
		boost::bind(&BiroliniHenley::KernelHandler, this, _1),
		boost::bind(&BiroliniHenley::WaitingHandler, this, _1));

	_smp->SetupMatrices();	

}

void BiroliniHenley::RunComponent()
{
	JaggedMatrix* jm = new JaggedMatrix(2); 
	jm->AddDistribution(0,1,mttf, ttfCov, DistributionFactory::Weibull);
	jm->AddDistribution(1,0,mttr, ttrCov, DistributionFactory::Weibull);

	jm->Display();

	SemiMarkovModel smp(jm);
	smp.SetModelInput(_mission, _nsteps);
	smp.SetupMatrices();	
	
	smp.ComputeStateProbabilities();
	vector<double> clock = smp.GetTimeVector();
	vector<double> p = smp.GetStateProbability(0, 1);	//Unavailable

	std::stringstream fpath;
	fpath << "BiroliniHenley\\WblExpRedundantComponent" << ".dat";

	TestResults result(fpath.str());
	result.AddResult(clock);
	result.AddResult(p);
	result.AddResult(p);
	result.Serialize();
}



BiroliniHenley::~BiroliniHenley()
{
	delete _smp;
	delete _oldModel;
}

matrix<double> BiroliniHenley::KernelHandler(double t)
{
	matrix<double> kernel(_smp->StateCount, _smp->StateCount);

	//One repair man problem
	kernel(0,0) = 0;
	kernel(1,2) = 0;
	kernel(2,1) = 0;

	kernel(0,1) = _newModel->pdf(t,0,1)* _newModel->Reliability(t,0,2);
	kernel(0,2) = _newModel->pdf(t,0,2)* _newModel->Reliability(t,0,1);

	kernel(1,0) = _newModel->pdf(t,1,0) * _oldModel->Reliability(t,1,3);
	kernel(2,0) = _newModel->pdf(t,2,0) * _oldModel->Reliability(t,2,3);

	kernel(1,1) = _oldModel->cdf(t,1,3) * _newModel->pdf(t,1,0);
	kernel(2,2) = _oldModel->cdf(t,2,3) * _newModel->pdf(t,2,0);
	

	//TESTMODEL("Kernel", kernel)
	return kernel;
}

vector<double> BiroliniHenley::WaitingHandler(double t)
{
	vector<double> waiting(_smp->StateCount);

	waiting(0) = _newModel->pdf(t,0,1) * _newModel->Reliability(t,0,2) +
				_newModel->pdf(t,0,2) * _newModel->Reliability(t,0,1);

	waiting(1) = _newModel->pdf(t,1,0) * _oldModel->Reliability(t,1,3) +
		_oldModel->pdf(t,1,3) * _newModel->Reliability(t,1,0);

	waiting(2) = _newModel->pdf(t,2,0) * _oldModel->Reliability(t,2,3) +
		_oldModel->pdf(t,2,3) * _newModel->Reliability(t,2,0);


	//	TESTMODEL("waiting", waiting)
	return waiting;

}