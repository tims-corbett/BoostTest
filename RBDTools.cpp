
#include "RBDTools.h"

#include <boost/foreach.hpp>
#include<numeric>
#include<functional>
#include<vector>
#include<math.h>





void RBDTools::Combinations(int ComponentCount,std::vector<std::vector<double>>& Combinations)
{
	unsigned int stateCount = (int) pow(2.0, (int)ComponentCount);

	std::vector<double> v = std::vector<double>(ComponentCount);

	std::fill(v.begin(), v.end(), 0.0f);

	int idx = 0;
	const int max = 1;
	bool iterationRequired = true;

	////outermost loop is idx = 0;
	while( idx >= 0)
	{
	
		Combinations.push_back(v);

		idx = ComponentCount - 1;
		do{
			v[idx]++;
			iterationRequired = false;

			if(v[idx] > max)
			{
				v[idx] = 0;
				idx--;
				iterationRequired = true;
			}
		}while(idx >= 0 && iterationRequired);
	}

}


void RBDTools::Combinations(int ComponentCount,std::vector<std::vector<double>>& Combinations, 
											   std::map<double, std::vector<double>>& CutSets,
											   AvailabilityHandler  Available)

{

	unsigned int stateCount = (int) pow(2.0, (int)ComponentCount);

	std::vector<double> v = std::vector<double>(ComponentCount);

	std::fill(v.begin(), v.end(), 0.0f);

	int idx = 0;
	const int max = 1;
	bool iterationRequired = true;
	int i_combs=0, j_cutsets=0;

	////outermost loop is idx = 0;
	while( idx >= 0)
	{
	
		Combinations.push_back(v);


		if( Available(v) == 0)
		{CutSets.insert(make_pair(i_combs, v));	}

		i_combs++;

		idx = ComponentCount - 1;
		do{
			v[idx]++;
			iterationRequired = false;

			if(v[idx] > max)
			{
				v[idx] = 0;
				idx--;
				iterationRequired = true;
			}
		}while(idx >= 0 && iterationRequired);
	}

}

std::vector<std::vector<double> > RBDTools::MinimalCutsets( std::map<double, std::vector<double>>& cutsets)
{
	std::vector<std::vector<double> > minimals;

	if(cutsets.size()==1)
		minimals.push_back(cutsets[0]);

	std::vector<double> v, temp(cutsets[0].size());

    std::map<double, std::vector<double>>::const_iterator it = cutsets.end(); 
	it--;
	while(it != cutsets.begin())
	{
		v = it->second;
		minimals.push_back(v);	
		it--;

		while(it != cutsets.begin())
		{
			temp = it->second;
			std::transform(v.begin(), v.end(), temp.begin(), temp.begin() , std::multiplies<double>());
			double sum = std::accumulate(temp.begin(), temp.end(), 0.0);
		if(sum == 0)
			break;
		it--;

		}
	}
	return minimals;
}


void RBDTools::displayStates(std:: vector<std::vector<double> > v)
{
	BOOST_FOREACH(std::vector<double> vec,  v )
	{
		BOOST_FOREACH(double d,  vec )
			std::cout << d << " ";

		std::cout << std::endl;
	}

}


double RBDTools::AtleastOneWorks(double p1, double p2)
{
	return (1.0 - (1.0 - p1) * (1.0 - p2));
}

double RBDTools::Availability2ParallelComponents(std::vector<double> v)
{
	return (RBDTools::AtleastOneWorks(v[0], v[1]));
}

double RBDTools::Availability4ParallelComponents(std::vector<double> v)
{
    return (RBDTools::AtleastOneWorks(v[0], v[1]) * RBDTools::AtleastOneWorks(v[2], v[3]));
}

double RBDTools::Availability2SeriesComponents(std::vector<double> v)
{
	return (v[0]*v[1]);
}


bool RBDTools::IsMultipleTransitions(std::vector<double>& v1, std::vector<double>& v2)
{
	double sum=0;
    for (int i = 0; i < (int)v1.size(); i++)
        sum += abs(v1[i] - v2[i]);

	return(sum != 1);	
}