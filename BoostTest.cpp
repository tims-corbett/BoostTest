
//#include<vld.h>



#include <stdlib.h>
#include <crtdbg.h>

#include<numeric>

//Library includes


//Test includes
#include "FlemingPiping/FlemingPiping.h"
#include "HenleyReducedState/HenleyReducedState.h"
#include "UGFHenley/UGFHenleyModel.h"
#include "MonteCarlo/MonteCarloTest.h"
#include "HenleyTest/HenleyTest.h"
#include "PreventiveMaintenance/PreventiveMaintenance.h"
#include "SMPMonteCarlo/SMPMonteCarloTest.h"
#include "WarmStandby/WarmStandby.h"
#include "BiroliniHenley/BiroliniHenley.h"
#include "NCCW/NCCW.h"
#include "TailGas/TailGas.h"
#include "NCCWShortLife/NCCWShortLife.h"
#include "TailGasMS/TailGasMS.h"
#include "TailGasMSPM/TailGasMSPM.h"
#include "FlemingPipingMonteCarlo/FlemingPipingMonteCarlo.h"
#include "TailGasMS/SMPMonteCarloTailGas.h"
#include "Spares/Spares.h"
#include "FireGrowth/FireGrowth.h"
#include "DigitalSystems/DigitalSystems.h"

#include "FireModel/FireModel.h"

//Boost includes
#include <boost/numeric/ublas/vector.hpp>
#include <boost/function.hpp>
#include <boost/foreach.hpp>

#include<functional>
#include <algorithm>		///transform, multiplies
#include<vector>
#include<math.h>

//#define DEBUG_MODEL
//#include "DebugTools.h"


using namespace boost::numeric::ublas;


void TestDistributions()
{
	double phi = 30;

	Distribution* exp = DistributionFactory::GetDistribution(
		DistributionFactory::Exponential, phi, 0.4);
	
	Distribution* wbl = DistributionFactory::GetDistribution(
		DistributionFactory::Weibull, phi, 0.3);
	
	Distribution* lognml = DistributionFactory::GetDistribution(
		DistributionFactory::LogNormal, phi, 0.4);
	
	Distribution* gamma = DistributionFactory::GetDistribution(
		DistributionFactory::Gamma, phi, 0.4);

	exp->DisplayParameters();
	wbl->DisplayParameters();
	lognml->DisplayParameters();
	gamma->DisplayParameters();
}

void MonteCarloTestFn()
{
	MonteCarloTest mctest;
	mctest.RunTests();
}

void UGFHenleyTestFn()
{
	UGFHenleyModel ugfTest;
	ugfTest.RunModel();
}

void HenleyReducedTestFn()
{
	HenleyReducedState stateReductionTest;
	stateReductionTest.RunModel();
}

void FlemingTestFn()
{
	FlemingPiping pipingModel;
	pipingModel.RunModel();
}

void CutsetTestFn()
{
	std::vector<double> v;
	std::vector<std::vector<double>>  combos;
	std::map<double, std::vector<double>>  cutsets;

	RBDTools::Combinations(4, combos, cutsets, RBDTools::Availability4ParallelComponents); 
	RBDTools::MinimalCutsets(cutsets);
}

void HenleyTestFn()
{
	std::cout << "Starting HenleyTest..." << std::endl;
	HenleyTest test;
	test.RunModel();
}

void PMModelTest()
{
	PreventiveMaintenanceModel model;
	model.RunModel();
}

void SMPMonteCarloTestFn()
{
	SMPMonteCarloTest test;
	test.RunModel();
}

void WarmStandbyTestFn()
{
	WarmStandby model;
	model.RunModel();
}

void BiroliniHenleyFn()
{
	BiroliniHenley model;
	model.RunModel();
}

void NCCWTestFn()
{
	NCCW model;
	model.RunModel();
}

void NCCWShortLifeTestFn()
{
	NCCWShortLife model;
	model.RunModel();
}

void TailGasTestFn()
{
	TailGas model;
	model.RunModel();
}

void TailGasMultiStateTestFn()
{
	TailGasMS model;
	model.RunModel();
}


void TailGasMultiStatePreventiveMaintenanceTestFn()
{
	TailGasMSPM model;
	model.RunModel();
}


void FlemingPipingMonteCarloTestFn()
{
	FlemingPipingMonteCarlo model;
	model.RunModel();
}


void TailGasMultiStateMonteCarloTestFn()
{
	SMPMonteCarloTailGas model;
	model.RunModel();
}

void SparesTestFn()
{
	Spares model;
	model.RunModel();
}

void FireGrowthTestFn()
{
	FireGrowth model;
	model.RunModel();
}

void FireModelTestFn()
{
	FireModel model;
	model.RunModel();
}

void DigitalSystemsTestFn()
{
	FeedWaterController model;
	model.RunModel();
}


void main()
{
//	TestDistributions();
//	WarmStandbyTestFn();
//	HenleyTestFn();
	//BiroliniHenleyFn();
//	TestDistributions();
//	MonteCarloTestFn();
//	NCCWTestFn();
	//PMModelTest();
//	SMPMonteCarloTestFn();
	//TailGasTestFn();
	//TailGasMultiStateTestFn();
	//TailGasMultiStateMonteCarloTestFn(); 
	 //TailGasMultiStatePreventiveMaintenanceTestFn();
	//FlemingPipingMonteCarloTestFn();

	//FlemingTestFn();
	//SparesTestFn();

	//FireGrowthTestFn();
	FireModelTestFn();

	//DigitalSystemsTestFn();

	//NCCWShortLifeTestFn();
}