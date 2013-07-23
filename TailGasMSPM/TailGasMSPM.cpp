#include "TailGasMSPM.h"

#include <boost/numeric/ublas/vector.hpp>
#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"
#include "../TestResults.h"

using namespace boost::numeric::ublas;

//                       #    Sym   Col#     Life	2->1	1->0	1->2
//Booster fan            1    A      2       5		4(0.6)	1(0.9)	7
//Quench pumps           2    B,C    3       10		8(0.4)	2(0.8)	15
//Feedwater pump         1    D      4       10		8(0.3)	2(0.8)	15
//Circulation pumps      2    E,F    5       10		8(0.6)	2(0.8)	15
//Filter                 1    G      6       5		4(0.4)	1(0.9)	7

//State 3: Perfect
//State 2: Degraded
//State 1: Repair
//State 0: Dead


void TailGasMSPM::RunModel()
{
	_missionTime = 40;
	_NSteps = 15000;
	const double days = 365.242199;

	for(double pmtime = 1; pmtime <= 15; pmtime+=1)	//pm once in pmtime years
	{
		std::ostringstream out;
		out << "TailGasMSPM\\TailGasMSPM40_" << pmtime << ".dat";
		TestResults results(out.str());

		DegradedComponentTest(7,    0.4,  8,    0.6, 3/days, 5/days, pmtime, results, "Booster fan");	//Booster fan, A
		DegradedComponentTest(10,   0.5,  10,   0.4, 5/days, 7/days, pmtime, results, "Quench pumps");		//Quench pumps, BC
		DegradedComponentTest(9,    0.6,  6,    0.4, 5/days, 7/days, pmtime, results, "Feedwater pump");		//Feedwater pump, D
		DegradedComponentTest(8,    0.4,  12,   0.5, 5/days, 7/days, pmtime, results, "Circulation pumps");	//Circulation pumps, EF
		DegradedComponentTest(8,    0.4,  7,    0.3, 3/days, 5/days, pmtime, results, "Filter");				//Filter, G

		results.Serialize();
	}
} 

void  TailGasMSPM::DegradedComponentTest(double _mean32, double _cv32, double _mean20, double _cv20, double _muTime13, double _muTime03, double _pmTime, TestResults& result, std::string name)					
{
		std::cout << name << std::endl;

		vector<double> _phi;
		vector<double> _time;


		JaggedMatrix *jm = 
			new JaggedMatrix(4);

		jm->AddDistribution(3, 2, _mean32, _cv32, DistributionFactory::Weibull);
		jm->AddDistribution(2, 0, _mean20, _cv20, DistributionFactory::Weibull);
		jm->AddDistribution(1, 3, _muTime13, 1.0, DistributionFactory::Weibull);
		jm->AddDistribution(2, 1, _pmTime, 1.0, DistributionFactory::Weibull);
		jm->AddDistribution(0, 3, _muTime03, 1.0, DistributionFactory::Weibull);

		jm->Display();
		
		SemiMarkovModel smp(jm);
		smp.SetModelInput(_missionTime, _NSteps);

		smp.RegisterHandlers(NULL, NULL);
		smp.SetupMatrices();
		smp.ComputeStateProbabilities();

		if(result.IsEmpty())
		{
			_time = smp.GetTimeVector();
			result.AddResult(_time);
		}

		_phi = smp.GetStateProbability(-1,0);
		result.AddResult(_phi);
	}
