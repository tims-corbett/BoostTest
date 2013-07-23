#include "UGFLibrary.h"

UFunction::UFunction(vector<vector<double>> psample)
{
	p = psample;
	perf = vector<double>(p.size());

	for(int i=0;i<p.size(); i++)
		perf(i) = i;
}

UFunction::UFunction(vector<vector<double>> psample, vector<double> perfsample)
{
	p = psample;
	perf = perfsample;
}

UFunction::UFunction(int nStates)
{
	p = vector<vector<double>>( nStates );
}

UFunction UFunction::UProduct(UFunction& u1, UFunction u2, UGFHandler handler)
{

	int nStates = u1.p.size();
	int timesteps = u1.p[0].size();
	int perfidx;

	UFunction u(nStates);
	vector<double> utemp(timesteps);

	for(int i = 0; i < nStates; i++)
	{
		u.p[i] = vector<double>(timesteps);
		u.perf = u1.perf;
		std::fill( u.p[i].begin(), u.p[i].end(), 0);
	}


	for(int i = 0; i < nStates; i++)
	{

		for(int j = 0; j < nStates; j++)
		{
			perfidx = handler( u1.perf, u2.perf, i, j );


			std::transform( u1.p[i].begin(), u1.p[i].end(), 
							u2.p[j].begin(),
							utemp.begin(),
							std::multiplies<double>());

			u.p[perfidx] += utemp;

		}
	}

	return u;
}
