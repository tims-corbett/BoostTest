#include "TailGasMS.h"

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

//State 2: Perfect
//State 1: Degraded
//State 0: Dead


void TailGasMS::RunModel()
{
	_missionTime = 20;
	_NSteps = 15000;
	TestResults results("TailGasMS\\TailGasMS.dat");
	const double days = 365.242199;

	DegradedComponentTest(4, 0.6, 1, 0.9,  7.0/days, results, "Booster fan", true);	//Booster fan
	DegradedComponentTest(8, 0.4, 2, 0.8, 15.0/days,results,"Quench pumps");		//Quench pumps
	DegradedComponentTest(8, 0.3, 2, 0.8, 15.0/days,results, "Feedwater pump");		//Feedwater pump
	DegradedComponentTest(8, 0.6, 2, 0.8, 15.0/days,results, "Circulation pump");	//Circulation pump
	DegradedComponentTest(4, 0.4, 1, 0.9, 7.0/days,results, "Filter");				//Filter

	results.Serialize();
}

void  TailGasMS::DegradedComponentTest(double _mean21, double _cv21, double _mean10, double _cv10, double _muTime, TestResults& result, std::string name, bool bAddTime)					
{
		std::cout << name << std::endl;

		vector<double> _phi;
		vector<double> _time;


		JaggedMatrix *jm = 
			new JaggedMatrix(3);

		jm->AddDistribution(2,1, _mean21, _cv21, DistributionFactory::Weibull);
		jm->AddDistribution(1,0, _mean10, _cv10, DistributionFactory::Weibull);
		jm->AddDistribution(1,2, _muTime, 1.0, DistributionFactory::Weibull);

		jm->Display();
		
		SemiMarkovModel smp(jm);
		smp.SetModelInput(_missionTime, _NSteps);

		smp.RegisterHandlers(NULL, NULL);
		smp.SetupMatrices();
		smp.ComputeStateProbabilities();

		if(bAddTime)
		{
			_time = smp.GetTimeVector();
			result.AddResult(_time);
		}

		_phi = smp.GetStateProbability(-1,0);
		result.AddResult(_phi);
	}
