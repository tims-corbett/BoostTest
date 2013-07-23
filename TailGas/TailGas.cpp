#include "TailGas.h"

#include <boost/numeric/ublas/vector.hpp>
#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"
#include "../TestResults.h"

using namespace boost::numeric::ublas;

//                       #    Sym   Col#     Life
//Booster fan            1    A      2       5
//Quench pumps           2    B,C    3       10
//Feedwater pump         1    D      4       10
//Circulation pumps      2    E,F    5       10
//Filter                 1    G      6       5


void TailGas::RunModel()
{
	_missionTime = 20;
	_NSteps = 15000;
	TestResults results("TailGas\\TailGas.dat");
	const double days = 365.242199;

	SingleComponentTest(5, 0.6,  7.0/days, results, "Booster fan", true);	//Booster fan
	SingleComponentTest(10, 0.4,  15.0/days,results,"Quench pumps");		//Quench pumps
	SingleComponentTest(10, 0.3,  15.0/days,results, "Feedwater pump");		//Feedwater pump
	SingleComponentTest(10, 0.6,  15.0/days,results, "Circulation pump");	//Circulation pump
	SingleComponentTest(5, 0.4,  7.0/days,results, "Filter");				//Filter

	results.Serialize();
}

void  TailGas::SingleComponentTest(double _mean, double _cv, double _muTime, TestResults& result, std::string name, bool bAddTime)					
{
	std::cout << name << std::endl;

		double mean[] = { _mean };
		double cv[] =  {  _cv };
		double repcv[] = { 1.0 };
		double muTime[] = { _muTime };
		vector<double> _phi;
		vector<double> _time;

		JaggedMatrix *jm =
				new JaggedMatrix(1, 
				mean,cv,
				muTime, repcv, 
				DistributionFactory::Weibull,
				DistributionFactory::Weibull
				);

		jm->Display(); 

		SemiMarkovModel model(jm);
		model.SetModelInput(_missionTime, _NSteps);

		model.SetupMatrices();
		model.ComputeStateProbabilities();
		
		if(bAddTime)
		{
			_time = model.GetTimeVector();
			result.AddResult(_time);
		}

		_phi = model.GetStateProbability(-1,0);
		result.AddResult(_phi);
	}
