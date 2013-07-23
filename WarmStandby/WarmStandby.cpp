#include<string>
#include<iostream>

#include "WarmStandby.h"
#include "../JaggedMatrix.h"
#include "../TestResults.h"
#include "../DebugTools.h"

#include <boost/bind.hpp>

WarmStandby::WarmStandby()
{
	 mttf = 30;
	 cov = 0.7;
	 ttrCov = 1.0;
	 mttr = 1;
	 mission = 120;
}

void WarmStandby::BuildOldModel()
{
		//double mttf = 100, cov = 1.0, mttr = 2.4;
		//double ttrCov = 1; //sqrt(0.6)/mttr;

		_oldModel = new JaggedMatrix(3);

		_oldModel->AddDistribution(0,1, mttf, cov, DistributionFactory::Weibull);
		_oldModel->AddDistribution(0,1, mttf, cov, DistributionFactory::Weibull);
		_oldModel->AddDistribution(1,2, mttf, cov, DistributionFactory::Weibull);

		_oldModel->AddDistribution(2,1, mttr, ttrCov, DistributionFactory::Weibull);
		_oldModel->AddDistribution(1,0, mttr, ttrCov, DistributionFactory::Weibull);

		_oldModel->Display();
		std::cout << std::endl;
}


void WarmStandby::RunModel()
{
	RunSystem();
	for(cov=1.0;cov >= 0.5; cov-=0.1)
	RunComponent();
}

void WarmStandby::RunSystem()
{
	//double mttf = 100, cov = 1.0, mttr = 2.4;
	//double ttrCov = 1; //sqrt(0.6)/mttr;

	BuildOldModel();

	JaggedMatrix* jm = new JaggedMatrix(2); 
	jm->AddDistribution(0,1,mttf, cov, DistributionFactory::Weibull);
	jm->AddDistribution(0,1,mttf, cov, DistributionFactory::Weibull);
	jm->AddDistribution(1,0,mttr, ttrCov, DistributionFactory::Weibull);
	jm->AddDistribution(1,1,mttf, cov, DistributionFactory::Weibull);

	jm->Display();

	_smp = new SemiMarkovModel(jm);
	_smp->SetModelInput(mission, 30000);

	_smp->RegisterHandlers(
		boost::bind(&WarmStandby::KernelHandler, this, _1),
		boost::bind(&WarmStandby::WaitingHandler, this, _1));

	_smp->SetupMatrices();	
	
	_smp->ComputeStateProbabilities();
	_time = _smp->GetTimeVector();

	vector<double> ones(_time.size());
	std::fill(ones.begin(), ones.end(), 1.0);

	_phi =  ones -(_smp->GetStateProbability(0, 0) + _smp->GetStateProbability(0, 1));

	TestResults result("WarmStandby/ExpExpStandbySystem.dat");
	result.AddResult(_time);
	result.AddResult(_phi);
	result.Serialize();

	delete _oldModel;
}

void WarmStandby::RunComponent()
{
	//double mttf = 100, cov = 1.0, mttr = 2.4;
	//double ttrCov = 1; //sqrt(0.6)/mttr;

	JaggedMatrix* jm = new JaggedMatrix(2); 
	jm->AddDistribution(0,1,mttf, cov, DistributionFactory::Weibull);
	jm->AddDistribution(1,0,mttr, ttrCov, DistributionFactory::Weibull);

	jm->Display();

	SemiMarkovModel smp(jm);
	smp.SetModelInput(mission, 20000);
	smp.SetupMatrices();	
	
	smp.ComputeStateProbabilities();
	vector<double> clock = smp.GetTimeVector();
	vector<double> p = smp.GetStateProbability(0, 1);	//Unavailable

	std::stringstream fpath;
	fpath << "WarmStandby\\ExpExpStandbyComponent" << cov << ".dat";

	TestResults result(fpath.str());
	result.AddResult(clock);
	result.AddResult(p);
	result.AddResult(p);
	result.Serialize();
}


matrix<double> WarmStandby::KernelHandler(double t)
{
	matrix<double> kernel(_smp->StateCount, _smp->StateCount);

	//One repair man problem
	kernel(0,0) = 0;
	kernel(0,1) = _smp->_model->pdf(t,0,1);
	kernel(1,0) = _smp->_model->pdf(t,1,0) * _oldModel->Reliability(t,1,2);
	kernel(1,1) = _oldModel->cdf(t,1,2) * _oldModel->pdf(t,2,1);
	
	//TESTMODEL("Kernel", kernel)
	return kernel;
}

vector<double> WarmStandby::WaitingHandler(double t)
{
	vector<double> waiting(_smp->StateCount);

	waiting(0) = _smp->_model->pdf(t,0,1);
	waiting(1) = _smp->_model->pdf(t,1,0) * _oldModel->Reliability(t,1,2) +
		_oldModel->pdf(t,1,2) * _smp->_model->Reliability(t,1,0);

	//	TESTMODEL("waiting", waiting)
	return waiting;

}