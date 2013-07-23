#include "../Distributions.h"
#include "FireModelLogNormal.h"
#include "../TestResults.h"
#include "../SemiMarkovModel.h"
#include "../JaggedMatrix.h"

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

//http://www.statsoft.com/textbook/distribution-fitting/#log-normal
//http://www.johndcook.com/distribution_chart.html
//http://www.stat.math.ethz.ch/~stahel/lognormal/bioscience.pdf

FireModelLogNormal::FireModelLogNormal()
{

}


const double m21 = 2.0;		//Sustained to non-fire	-	exponential dist
const double c21 = 5.0/m21;	//lognormal (-0.29735355387334639, 1.4074805394273071)

const double m23 = 8.45;	//Sustained to vigorous	-	log-normal dist
const double c23 = 0.78 / m23;

const double m32 = 1.0;		//Vigorous to sustained	-	exponential
const double c32 = 2.0 / m32;

const double m34 = 5.55;	//Vigorous to interactive -	normal
const double c34 = 3.22 / m34;

const double m43 = 1.5;		//interactive to vigorous - exponential
const double c43 = 9.0 / m43;

const double m45 = 0.5;		//interactive to remote	-	exponential
const double c45 = 3.5 / m45;

const double m54 = 0.6;		//remote to interactive	-	exponential
const double c54 = 6.0 / m54;

const double m56 = 5.18;	//remote to fullroom	-	lognormal
const double c56 = 4.18 / m56;

//const double m61 = 5.1;		//full-room to non-fire	-	exponential

void FireModelLogNormal::TestDistributions()
{
		Distribution *d21 = 
			DistributionFactory::GetDistribution(DistributionFactory::Exponential, m34,1);

		d21->DisplayParameters();

}

void FireModelLogNormal::RunModel()
{
	//RunSMP();
	//RunBerlinSMP();
	//RunBerlinExponentialSMP();
	//RunMarkov();


}


void FireModelLogNormal::RunSMP()
{
		std::ostringstream fpath;
		
		fpath << "FireModel_LogNormal.dat";

		int nStates = 6;

		JaggedMatrix *jmWbl = new JaggedMatrix(nStates);

		Distribution *d21 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m21,c21);
		jmWbl->AddDistribution(1,0,d21);


		Distribution *d23 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m23,c23);
		jmWbl->AddDistribution(1,2,d23);

		
		Distribution *d32 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m32,c32);
		jmWbl->AddDistribution(2, 1, d32);


		Distribution *d34 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m34,c34);
		jmWbl->AddDistribution(2,3,d34);


		Distribution *d43 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m43,c43);
		jmWbl->AddDistribution(3,2,d43);


		Distribution *d45 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m45,c45);
		jmWbl->AddDistribution(3,4,d45);


		Distribution *d54 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m54,c54);
		jmWbl->AddDistribution(4,3,d54);


		Distribution *d56 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m56,c56);
		jmWbl->AddDistribution(4,5,d56);


		jmWbl->Display();
		//jmWbl->DisplayInputs();

		SemiMarkovModel smpWbl(jmWbl);
		smpWbl.SetModelInput(45, 20000);
		smpWbl.SetupMatrices();

		smpWbl.ComputeStateProbabilities();

		vector<double> time = smpWbl.GetTimeVector();
		vector<double> p21 = smpWbl.GetStateProbability(1,0);
		vector<double> p22 = smpWbl.GetStateProbability(1,1);
		vector<double> p23 = smpWbl.GetStateProbability(1,2);
		vector<double> p24 = smpWbl.GetStateProbability(1,3);
		vector<double> p25 = smpWbl.GetStateProbability(1,4);
		vector<double> p26 = smpWbl.GetStateProbability(1,5);


		TestResults sysresults(fpath.str());
		
		sysresults.AddResult(time);
		sysresults.AddResult(p21);
		sysresults.AddResult(p22);
		sysresults.AddResult(p23);
		sysresults.AddResult(p24);
		sysresults.AddResult(p25);
		sysresults.AddResult(p26);

		sysresults.Serialize( );

}


void FireModelLogNormal::RunBerlinSMP()
{
		std::ostringstream fpath;
		
		fpath << "FireModel_Berlin.dat";

		int nStates = 6;

		JaggedMatrix *jmWbl = new JaggedMatrix(nStates);

		Distribution *d21 = 
			DistributionFactory::GetDistribution(DistributionFactory::Uniform, m21,c21);
		jmWbl->AddDistribution(1,0,d21);


		Distribution *d23 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m23,c23);
		jmWbl->AddDistribution(1,2,d23);

		
		Distribution *d32 = 
			DistributionFactory::GetDistribution(DistributionFactory::Uniform, m32,c32);
		jmWbl->AddDistribution(2, 1, d32);


		Distribution *d34 = 
			DistributionFactory::GetDistribution(DistributionFactory::Normal, m34,c34);
		jmWbl->AddDistribution(2,3,d34);


		Distribution *d43 = 
			DistributionFactory::GetDistribution(DistributionFactory::Uniform, m43,c43);
		jmWbl->AddDistribution(3,2,d43);


		Distribution *d45 = 
			DistributionFactory::GetDistribution(DistributionFactory::Uniform, m45,c45);
		jmWbl->AddDistribution(3,4,d45);


		Distribution *d54 = 
			DistributionFactory::GetDistribution(DistributionFactory::Uniform, m54,c54);
		jmWbl->AddDistribution(4,3,d54);


		Distribution *d56 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m56,c56);
		jmWbl->AddDistribution(4,5,d56);


		jmWbl->Display();
		//jmWbl->DisplayInputs();

		SemiMarkovModel smpWbl(jmWbl);
		smpWbl.SetModelInput(45, 20000);
		smpWbl.SetupMatrices();

		smpWbl.ComputeStateProbabilities();

		vector<double> time = smpWbl.GetTimeVector();
		vector<double> p21 = smpWbl.GetStateProbability(1,0);
		vector<double> p22 = smpWbl.GetStateProbability(1,1);
		vector<double> p23 = smpWbl.GetStateProbability(1,2);
		vector<double> p24 = smpWbl.GetStateProbability(1,3);
		vector<double> p25 = smpWbl.GetStateProbability(1,4);
		vector<double> p26 = smpWbl.GetStateProbability(1,5);


		TestResults sysresults(fpath.str());
		
		sysresults.AddResult(time);
		sysresults.AddResult(p21);
		sysresults.AddResult(p22);
		sysresults.AddResult(p23);
		sysresults.AddResult(p24);
		sysresults.AddResult(p25);
		sysresults.AddResult(p26);

		sysresults.Serialize( );

}


//All backwards are exponential
void FireModelLogNormal::RunBerlinExponentialSMP()
{
		std::ostringstream fpath;
		
		fpath << "FireModel_BerlinExp.dat";

		int nStates = 6;

		JaggedMatrix *jmWbl = new JaggedMatrix(nStates);

		Distribution *d21 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m21,1.0);
		jmWbl->AddDistribution(1,0,d21);


		Distribution *d23 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m23,c23);
		jmWbl->AddDistribution(1,2,d23);

		
		Distribution *d32 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m32,1.0);
		jmWbl->AddDistribution(2, 1, d32);


		Distribution *d34 = 
			DistributionFactory::GetDistribution(DistributionFactory::Normal, m34,c34);
		jmWbl->AddDistribution(2,3,d34);


		Distribution *d43 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m43,1.0);
		jmWbl->AddDistribution(3,2,d43);


		Distribution *d45 = 
			DistributionFactory::GetDistribution(DistributionFactory::Uniform, m45,c45);
		jmWbl->AddDistribution(3,4,d45);


		Distribution *d54 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m54,1.0);
		jmWbl->AddDistribution(4,3,d54);


		Distribution *d56 = 
			DistributionFactory::GetDistribution(DistributionFactory::LogNormal, m56,c56);
		jmWbl->AddDistribution(4,5,d56);


		jmWbl->Display();
		//jmWbl->DisplayInputs();

		SemiMarkovModel smpWbl(jmWbl);
		smpWbl.SetModelInput(45, 1000);
		smpWbl.SetupMatrices();

		smpWbl.ComputeStateProbabilities(boost::bind(&FireModelLogNormal::DisplayTimeStep, this, _1));

		vector<double> time = smpWbl.GetTimeVector();
		vector<double> p21 = smpWbl.GetStateProbability(1,0);
		vector<double> p22 = smpWbl.GetStateProbability(1,1);
		vector<double> p23 = smpWbl.GetStateProbability(1,2);
		vector<double> p24 = smpWbl.GetStateProbability(1,3);
		vector<double> p25 = smpWbl.GetStateProbability(1,4);
		vector<double> p26 = smpWbl.GetStateProbability(1,5);


		TestResults sysresults(fpath.str());
		
		sysresults.AddResult(time);
		sysresults.AddResult(p21);
		sysresults.AddResult(p22);
		sysresults.AddResult(p23);
		sysresults.AddResult(p24);
		sysresults.AddResult(p25);
		sysresults.AddResult(p26);

		sysresults.Serialize( );

}

void FireModelLogNormal::RunMarkov()
{
		std::ostringstream fpath;
		
		fpath << "FireModel_Markov.dat";

		int nStates = 6;

		JaggedMatrix *jmWbl = new JaggedMatrix(nStates);

		Distribution *d21 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m21,1);
		jmWbl->AddDistribution(1,0,d21);


		Distribution *d23 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m23,1);
		jmWbl->AddDistribution(1,2,d23);

		
		Distribution *d32 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m32,1);
		jmWbl->AddDistribution(2, 1, d32);


		Distribution *d34 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m34,1);
		jmWbl->AddDistribution(2,3,d34);


		Distribution *d43 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m43,1);
		jmWbl->AddDistribution(3,2,d43);


		Distribution *d45 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m45,1);
		jmWbl->AddDistribution(3,4,d45);


		Distribution *d54 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m54,1);
		jmWbl->AddDistribution(4,3,d54);


		Distribution *d56 = 
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, m56,1);
		jmWbl->AddDistribution(4,5,d56);


		jmWbl->Display();
		//jmWbl->DisplayInputs();

		SemiMarkovModel smpWbl(jmWbl);
		smpWbl.SetModelInput(45, 20000);
		smpWbl.SetupMatrices();

		smpWbl.ComputeStateProbabilities();

		vector<double> time = smpWbl.GetTimeVector();
		vector<double> p21 = smpWbl.GetStateProbability(1,0);
		vector<double> p22 = smpWbl.GetStateProbability(1,1);
		vector<double> p23 = smpWbl.GetStateProbability(1,2);
		vector<double> p24 = smpWbl.GetStateProbability(1,3);
		vector<double> p25 = smpWbl.GetStateProbability(1,4);
		vector<double> p26 = smpWbl.GetStateProbability(1,5);


		TestResults sysresults(fpath.str());
		
		sysresults.AddResult(time);
		sysresults.AddResult(p21);
		sysresults.AddResult(p22);
		sysresults.AddResult(p23);
		sysresults.AddResult(p24);
		sysresults.AddResult(p25);
		sysresults.AddResult(p26);

		sysresults.Serialize( );

}


void main()
{
	FireModelLogNormal model;
	//model.RunModel();
	//model.RunBerlinExponentialSMP();

	model.TestDistributions();
}


void FireModelLogNormal::DisplayTimeStep(SemiMarkovModel* pModel)
{
	boost::xtime xt;
	try
	{
		while(pModel->bContinue )
		{
			std::cout <<  pModel->timeStep << "\t";
		
			if( pModel->timeStep > 100)
			{
				boost::mutex::scoped_lock(m_timeStepMutex);
				pModel->bContinue=false;
			}


			boost::xtime_get(&xt, boost::TIME_UTC);
			xt.sec += 1; // change xt to next second
			boost::thread::sleep(xt); 
	
		}
	}catch(...)
	{
	}

}