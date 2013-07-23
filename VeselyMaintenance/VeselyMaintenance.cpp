#include "../Distributions.h"
#include "VeselyMaintenance.h"
#include "../TestResults.h"
#include<iostream>
#include <boost/thread/thread.hpp>

//#include<vld.h>


boost::mutex m_mutex;

VeselyMaintenance::VeselyMaintenance()
{

}


void VeselyMaintenance::TestDistributions()
{

}



//State 1 - Operational
//State 2 - Degraded
//State 3 - Maintenance
//State 4 - Failure

/************** TRANSITIONS *****************

od	
om
of

dm
df

mo
md
mf

fo
fd

***********************************************

T_m = Average time between maintenances

***********************************************/

void VeselyMaintenance::RunModel(double r_od, double gamma, double T_m, int TestCase)
{
		std::ostringstream fpath;
		
		//Naming: _[gamma]_[T_m]_[R_d]
		fpath << "Analysis\\VeselyMaintenance_" << gamma << "_" << T_m << "_" << r_od << ".dat" ;

		int nStates = 4;

		JaggedMatrix jmWbl(nStates);

		Distribution *d12 =		//od
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_od(r_od), gamma_od(gamma) );
		jmWbl.AddDistribution(0,1,d12);


		Distribution *d13 =		//om
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_om(T_m, r_od), 1.0 );
		jmWbl.AddDistribution(0,2,d13);


		Distribution *d14 =		//of
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_of(), 1.0 );
		jmWbl.AddDistribution(0,3,d14);


		

		Distribution *d23 =		//dm
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_dm(T_m, r_od), 1.0 );
		jmWbl.AddDistribution(1,2,d23);


		Distribution *d24 =		//df
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_df(r_od), 1.0 );
		jmWbl.AddDistribution(1,3,d24);



		Distribution *d31 =		//mo
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_mo(), 1.0 );
		jmWbl.AddDistribution(2,0,d31);


		Distribution *d32 =		//md
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_md(), 1.0 );
		jmWbl.AddDistribution(2,1,d32);


		Distribution *d34 =		//mf
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_mf(), 1.0 );
		jmWbl.AddDistribution(2,3,d34);

		double df = d_f(T_m);

		Distribution *d41 =		//fo
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_fo(T_m), 1.0 );
		jmWbl.AddDistribution(3,0,d41);


		Distribution *d42 =		//fd
			DistributionFactory::GetDistribution(DistributionFactory::Weibull, mu_fd(T_m), 1.0 );
		jmWbl.AddDistribution(3,1,d42);
		

		
		//------------------------------------------------------------------------------------------
		
		jmWbl.Display();
		jmWbl.DisplayInputs();

		SemiMarkovModel smpWbl(&jmWbl);
		smpWbl.SetModelInput(365 * 24 * 8.4, 50000);

		smpWbl.SetupMatrices();

		std::cout << "Matrix setup complete for test case# " << TestCase << std::endl;
		
		smpWbl.ComputeStateProbabilities();

		vector<double> time = smpWbl.GetTimeVector();
		vector<double> p12 = smpWbl.GetStateProbability(0,1);	//od
		vector<double> p13 = smpWbl.GetStateProbability(0,2);	//om
		vector<double> p14 = smpWbl.GetStateProbability(0,3);	//of
		

		boost::mutex::scoped_lock lock_it( m_mutex );
		TestResults sysresults(fpath.str());
		
		sysresults.AddResult(time);
		sysresults.AddResult(p12);
		sysresults.AddResult(p13);
		sysresults.AddResult(p14);
		
		sysresults.Serialize( );

		std::cout << std::endl << "[" << TestCase <<  "] Analysis\\VeselyMaintenance_" << gamma << "_" << T_m << "_" << r_od << ".dat" << std::endl;


}



void main()
{
		VeselyMaintenance model;
		//model.RunModel(3.0, 1.0, 168, 1);
		//model.RunModel(3.0, 1.0, 73000, 1);
		//return;

		double degRatio[] = {3.0, 10.0};
		double gamma[] = {1.0, 0.4, 0.6};
		double T_m[] = { 168, 336, 730, 2190, 4380, 8760, 21900, 43800, 73000};
		int cnt=1;
		int i,j,k;
		boost::thread *m_thread;
		boost::thread_group group;
	
		
		for(k=0; k<2; k++)
		{
			for(j=0; j<3; j++) 
			{
				for(i=0; i<9; i++)
				{
					m_thread = new boost::thread(boost::bind(&VeselyMaintenance::RunModel, &model, degRatio[k], gamma[j], T_m[i], cnt));
					group.add_thread(m_thread);					
					cnt++;
				}
			}
		}
		group.join_all();
	

}


//void main()
//{
//		VeselyMaintenance model;
//
//		double degRatio[] = {3.0, 10.0};
//		double gamma[] = {0.4, 0.6, 1.0};
//		double T_m[] = { 168, 336, 730, 2190, 4380, 8760, 21900, 43800, 73000};
//		int cnt=1;
//		int i,j,k;
//		
//		for(k=0; k<2; k++)
//		{
//			for(j=0; j<3; j++) 
//				for(i=0; i<9; i++)
//				{
//					std::cout << std::endl << std::endl << cnt << "\t";
//					cnt++;
//					model.RunModel(degRatio[k], gamma[j], T_m[i]);
//				}
//		}
//}



/*********************************
		Conv. Factor (hrs)
1	week	168
2	weeks	336
1	month	730
3	months	2190
6	months	4380
1	year	8760
2.5	years	21900 
5	years	109500
**********************************/