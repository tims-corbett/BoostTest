
#include "JaggedMatrix.h"
#include "RBDTools.h"

#include<cstdlib>

#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>


#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_traits.hpp> 

#include <algorithm> //transform
#include <numeric>
#include <functional>   // for multiplies

namespace ublas = boost::numeric::ublas;

//typedef boost::
	//GraphvizDigraph Graph;
//typedef boost::graph_traits<boost::GraphvizDigraph>::edge_descriptor EdgeDescriptor;



using namespace boost::numeric::ublas;

JaggedMatrix::JaggedMatrix()
{
	_size = 0;
	_edgeCount = 0;
}


JaggedMatrix::JaggedMatrix(int n)
{
	_jaggedMatrix = matrix<boost::ptr_vector<Distribution>>(n,n);
	_meanMatrix = matrix<std::vector<double>>(n,n);
	_covMatrix = matrix<std::vector<double>>(n,n);
	_size = n;
	_edgeCount= 0;
    _adjacencyMatrix = matrix<int>(n,n);
}


JaggedMatrix::~JaggedMatrix()
{
	/*for(int i=0;i<_size;i++)
		for(int j=0;j<_size;j++)
		{
			if( _jaggedMatrix(i,j).size() == 0)
				continue;
			for(int c=0; c < _jaggedMatrix(i,j).size();c++)
				boost::ptr_vector<Distribution>::auto_type x= _jaggedMatrix(i,j).release(_jaggedMatrix(i,j).begin());
		}*/
}

void JaggedMatrix::AddDistribution(size_t r, size_t c, Distribution* distribution)
{
	if(_size == 0)
		throw std::exception("Null jagged matrix");

	_edgeCount++;
	_adjacencyMatrix(r,c) = 1;


	_jaggedMatrix(r,c).push_back(distribution);
	//_jaggedMatrix(r,c).push_back(    std::auto_ptr<Distribution>(distribution)     );
}

void JaggedMatrix::AddDistribution(const TransitionInfo& info)
{
	AddDistribution(info._from, info._to, 
		DistributionFactory::GetDistribution(info._type, info._meantime, info._cov));
}

void JaggedMatrix::AddDistribution(size_t r, size_t c, 
								   double mean, double cov, 
								   DistributionFactory::DistributionType type)
{
	if(_size == 0)
		throw std::exception("Null jagged matrix");

	Distribution* distribution = 
		DistributionFactory::GetDistribution(type, mean, cov);

	AddDistribution(r,c,distribution);

	_meanMatrix(r,c).push_back(mean);
	_covMatrix(r,c).push_back(cov);


}



void JaggedMatrix::AddDistribution(size_t r, size_t c, 
								   double meantime, double cov, 
								   double repairtime, double repaircov,
								   DistributionFactory::DistributionType failuretype,
								   DistributionFactory::DistributionType repairtype
								   )
{
	if(_size == 0)
		throw std::exception("Null jagged matrix");

	AddDistribution(r,c, meantime, cov, failuretype);
	AddDistribution(r,c, repairtime, repaircov, repairtype);

}

void JaggedMatrix::DisplayInputs()
{
	for(size_t i=0;i<_size;i++)
	{
		for(size_t j=0;j<_size;j++)
		{
			if( _jaggedMatrix(i,j).size() == 0)
				std::cout << "0\t";
			else
			{
				//std::cout << "{";
				for(size_t c=0; c<_jaggedMatrix(i,j).size();c++)
					std::cout <<_jaggedMatrix(i,j)[c].MarkovTransitionRate() << "\t";
				//std::cout << "}";
			}

		}

		std::cout << std::endl;
	}
}



// individual pdf function of each distribution
vector<double> JaggedMatrix::Cellpdf(double t, size_t r, size_t c)
{
	vector<double> result;

	if( _jaggedMatrix(r,c).empty())
		return result;

	size_t len = _jaggedMatrix(r, c).size();

	result = vector<double>(len);

	for (unsigned i = 0; i < _jaggedMatrix(r, c).size(); i++)
		result[i] = (_jaggedMatrix(r,c))[i].pdf(t);

	return result;
}


// individual pdf function of a row
vector<double> JaggedMatrix::pdf(double t, size_t r)
{
	vector<double> result(_size);
	std::fill(result.begin(), result.end(), 0.0);

	vector<double> cellpdf;

	for (size_t i = 0; i < _size; i++)
	{
		cellpdf = Cellpdf(t, r, i);
		
		if(cellpdf.empty())
			continue;

		result[i] = cellpdf[0];
	}

	return result;
}


/// <summary>
/// individual Reliability function of each distribution
/// in cell [i,j]
/// </summary>
/// <param name="t">time step at which reliability is needed</param>
/// <param name="r">row of the jagged matrix</param>
/// <param name="c">row of the jagged matrix</param>
/// <returns>Reliability vector</returns>
vector<double> JaggedMatrix::CellReliability(double t, size_t r, size_t c)
{
	vector<double> result  = unit_vector<double>(1);

	if (_jaggedMatrix(r, c).empty())
		return result;

	size_t len = _jaggedMatrix(r, c).size();

	result.resize(len); //result = vector<double>(len);


	for( size_t i=0; i<len; i++)
		result[i] = 1 - _jaggedMatrix(r, c)[i].cdf(t);


	return result;
}

double JaggedMatrix::RowReliabilityProduct(double t, size_t r)
{
	vector<double> cellrel;
	double prod=1;
	for(size_t c=0; c<_size; c++)
	{
		cellrel = CellReliability(t,r,c);
		prod = std::accumulate(cellrel.begin(), cellrel.end(), prod, std::multiplies<double>());
	}
	return prod;
}

double JaggedMatrix::cdf(double t, size_t i, size_t j)
{
	if (_jaggedMatrix(i, j).empty())
		return 0;

	size_t len = _jaggedMatrix(i, j).size();

	if (len == 1)
		return _jaggedMatrix(i, j)[0].cdf(t);
}

/// <summary>
/// combined pdf function of a cell [r,c]
/// </summary>
/// <param name="t">time step at which pdf is needed</param>
/// <param name="r">row of the jagged matrix</param>
/// <param name="c">row of the jagged matrix</param>
/// <returns>pdf</returns>
double JaggedMatrix::pdf(double t, size_t i, size_t j)
{
	if (_jaggedMatrix(i, j).empty())
		return 0;

	size_t len = _jaggedMatrix(i, j).size();

	if (len == 1)
		return _jaggedMatrix(i, j)[0].pdf(t);

	vector<double> v1, v2;

	if (len == 2)
	{
		v1 = CellReliability(t, i, j);
		v2 = Cellpdf(t, i, j);

		//return v1[0]*v2[0] + v1[1]*v2[1];

		return v1[0]*v2[1] + v1[1]*v2[0];
	}

	if (len == 3)
	{
		v1 = CellReliability(t, i, j);
		v2 = Cellpdf(t, i, j);

		return v1[0] * v1[1] * v2[2] +
			v1[0] * v2[1] * v1[2] +
			v2[0] * v1[1] * v1[2];
	}

	throw std::exception("More than two compositions not supported for state reduction");
}


/// <summary>
/// pdf matrix keeping state reduction in mind.
/// a maximum of 3 distributions can be composed for
/// a single transition as of now
/// </summary>
/// <param name="t">timestep at which pdf is needed</param>
/// <returns>matrix of pdfs for each transition</returns>
matrix<double> JaggedMatrix::pdf(double t)
{
	matrix<double> pdfm = matrix<double>(_size, _size);
	double pdfval;

	for (size_t i = 0; i < _size; i++)
		for (size_t j = 0; j < _size; j++)
		{
			pdfval = pdf(t, i, j);
			pdfm(i, j) = pdfval;
		}
	return pdfm;
}


/// <summary>
/// Combined Reliability function of a single cell [i,j] array
/// </summary>
/// <param name="t">time step at which reliability is needed</param>
/// <param name="r">row of the jagged matrix</param>
/// <param name="c">row of the jagged matrix</param>
/// <returns>Reliability</returns>
double JaggedMatrix:: Reliability(double t, size_t r, size_t c)
{
	if (_jaggedMatrix(r, c).empty())
		return 1.0;

	size_t len = _jaggedMatrix(r, c).size();
	double result = 1.0;

	for (size_t i = 0; i < len; i++)
		result *= (1 - _jaggedMatrix(r, c)[i].cdf(t));

	return result;
}

/// <summary>
/// Combined Reliability function of a single row
/// </summary>
/// <param name="t">time step at which reliability is needed</param>
/// <param name="r">row of the jagged matrix</param>
/// <param name="c">row of the jagged matrix</param>
/// <returns>Reliability</returns>
vector<double> JaggedMatrix:: Reliability(double t, size_t r)
{
	vector<double> result(_size);

	for (size_t i = 0; i < _size; i++)
		result[i] = Reliability(t,r,i);

	return result;
}

/// <summary>
/// Reliability function of minimum of n random variables is
/// product of reliability functions of the n r.v.s
/// </summary>
/// <param name="t">time step at which reliability is needed</param>
/// <returns>Reliability matrix keeping state reduction in view</returns>
matrix<double> JaggedMatrix:: Reliability(double t)
{
	matrix<double> result = matrix<double>(_size, _size,1.0);
	double rel = 0.0;

	for (size_t i = 0; i < _size; i++)
		for (size_t j = 0; j < _size; j++)
		{
			if (_jaggedMatrix(i, j).empty())
			{
				result(i, j) = 1.0;
				continue;
			}

			rel = 1.0;
			BOOST_FOREACH(Distribution& dist, _jaggedMatrix(i,j))
				rel *= (1 - dist.cdf(t));
			result(i, j) = rel;
		}

		return result;
}

/// <summary>
/// CDF of minimum of n random variables is
/// CDF = 1 - RELIABILITY
/// </summary>
/// <param name="t">timestep at which cdf is needed</param>
/// <returns>cdf matrix ( = 1 - reliability)</returns>
matrix<double> JaggedMatrix::cdf(double t)
{
	matrix<double> rel = Reliability(t);
	matrix<double> result = matrix<double>(_size, _size);

	for(size_t i=0;i<_size;i++)
		for(size_t j=0;j<_size;j++)
			result(i,j) = 1 - rel(i,j);
	return result;
}


/// <summary>
/// Hazard or failure rate of all the distributions at time t
/// Simply finds pdf / rel
/// </summary>
/// <param name="t">Time t at which hazard is needed</param>
/// <returns>hazard at time t</returns>
matrix<double> JaggedMatrix:: Hazard(double t)
{
	matrix<double> pdfmatrix = pdf(t);
	matrix<double> rel = Reliability(t);

	matrix<double> hazard = element_div(pdfmatrix, rel);

	return hazard;
}

//same distribution for all the transitions
void JaggedMatrix::BuildModel(matrix<std::vector<double>> mean, 
										  matrix<std::vector<double>> cov,
										  DistributionFactory::DistributionType distributionType
										  )                                  
{
    size_t r, c, j;
	size_t len;
    double vShape = 0.0;

	_jaggedMatrix = matrix<boost::ptr_vector<Distribution>>(_size, _size);
    _adjacencyMatrix = matrix<int>(_size, _size);


    for ( r=0; r< _size; r++)
        for(c=0; c<_size; c++)
        {
            if (mean(r, c).empty())
			{
				_adjacencyMatrix(r, c) = 0;
                continue;
			}

            len = mean(r, c).size();

            for (j = 0; j < len; j++)
            {
				AddDistribution(r,c, 
					 mean(r,c)[j], cov(r,c)[j], distributionType );
            }
        }
}

/// <summary>
/// Builds the SMP model with 
/// componentfailure and repair times, 
/// failure time covs are also supplied
/// </summary>
/// <param name="failureTime">mttf of each component</param>
/// <param name="covs">cov for each time to failure</param>
/// <param name="repairTime">repair time of each component</param>
JaggedMatrix::JaggedMatrix(
						   int componentCount,
						   double failureTime[], 
						   double failurecov[], 
						   double repairTime[],
						   double repaircov[],
						   DistributionFactory::DistributionType failuretype,
						   DistributionFactory::DistributionType repairtype
						   ) 
{

	std::vector<std::vector<double>> combos;

	RBDTools::Combinations(componentCount, combos);
	_size = combos.size();
	_jaggedMatrix = matrix<boost::ptr_vector<Distribution>>(_size, _size);

	_meanMatrix = matrix<std::vector<double>>(_size,_size);
	_covMatrix = matrix<std::vector<double>>(_size,_size);
    _adjacencyMatrix = matrix<int>(_size, _size);
	_edgeCount = 0;


    size_t i = 0, j = 0;
    int nonZeroIdx = 0;
	std::vector<double> target;
	double repcov=0;


    for (i = 0; i < _size; i++)
        for (j = 0; j < _size; j++)
        {
			if(RBDTools::IsMultipleTransitions(combos[i],combos[j]) || i==j)
	                continue;

            nonZeroIdx = 0;
            while (combos[i][nonZeroIdx] == combos[j][nonZeroIdx])
                nonZeroIdx++;


            //from 1 ->0 means a failure
            target = combos[j];
			
			if(j < i  )	//failure encountered
			{
				AddDistribution(i, j, 
								failureTime[nonZeroIdx], failurecov[nonZeroIdx], 
								   failuretype);
			}
			else 		//repair encountered
			{
				if(repairTime == NULL)
					continue;

				repcov = (repaircov == NULL) ? 1.0 : repaircov[nonZeroIdx];

				AddDistribution(i, j, 
					repairTime[nonZeroIdx], repcov, 
					repairtype);
			}

        }

}


JaggedMatrix::JaggedMatrix(matrix<std::vector<double>> mean,
						   matrix<std::vector<double>> cov,
						   DistributionFactory::DistributionType type) 
{
	_size = mean.size1();


	_meanMatrix = matrix<std::vector<double>>(_size,_size);
	_covMatrix = matrix<std::vector<double>>(_size,_size);


	BuildModel(mean, cov, type);
}

bool JaggedMatrix::PathExists(int fromState, int toState)
{
	if(_adjacencyMatrix(fromState, toState) == 1)
		return true;
	return false;
}

//time spent in inState on condition that next state is toState
double JaggedMatrix::SampleTimeSpent(int inState, int toState, double p)
{
	if(PathExists(inState, toState)==false)
		return 0;

	//inverting composed distributions is way too complex, just take 0th.
	return _jaggedMatrix(inState, toState)[0].InverseTransform();
}

void JaggedMatrix::Display()
{
	for(size_t i=0;i<_size;i++)
	{
		for(size_t j=0;j<_size;j++)
		{
			if( _jaggedMatrix(i,j).size() == 0)
				std::cout << "[0,0]\t";
			else
			{
				std::cout << "{";
				for(size_t c=0; c<_jaggedMatrix(i,j).size();c++)
					_jaggedMatrix(i,j)[c].DisplayParameters();
				std::cout << "}";
			}

		}

		std::cout << std::endl;
	}
}



enum files_e { dax_h, yow_h, boz_h, zow_h, foo_cpp,
               foo_o, bar_cpp, bar_o, libfoobar_a,
               zig_cpp, zig_o, zag_cpp, zag_o,
                 libzigzag_a, killerapp, N };

const std::vector<std::string> GetNames()
{
  std::vector<std::string> v;
  v.push_back("dax.h");
  v.push_back("yow.h");
  v.push_back("boz.h");
  v.push_back("zow.h");
  v.push_back("foo.cpp");
  v.push_back("foo.o");
  v.push_back("bar.cpp");
  v.push_back("bar.o");
  v.push_back("libfoobar.a");
  v.push_back("zig.cpp");
  v.push_back("zig.o");
  v.push_back("zag.cpp");
  v.push_back("zag.o");
  v.push_back("libzigzag.a");
  v.push_back("killerapp");
  return v;
}

const std::vector<std::pair<int,int> > GetEdges()
{
  std::vector<std::pair<int,int> > v;
  v.push_back(std::make_pair(dax_h, foo_cpp));
  v.push_back(std::make_pair(dax_h, bar_cpp));
  v.push_back(std::make_pair(dax_h, yow_h));
  v.push_back(std::make_pair(yow_h, bar_cpp));
  v.push_back(std::make_pair(yow_h, zag_cpp));
  v.push_back(std::make_pair(boz_h, bar_cpp));
  v.push_back(std::make_pair(boz_h, zig_cpp));
  v.push_back(std::make_pair(boz_h, zag_cpp));
  v.push_back(std::make_pair(zow_h, foo_cpp));
  v.push_back(std::make_pair(foo_cpp, foo_o));
  v.push_back(std::make_pair(foo_o, libfoobar_a));
  v.push_back(std::make_pair(bar_cpp, bar_o));
  v.push_back(std::make_pair(bar_o, libfoobar_a));
  v.push_back(std::make_pair(libfoobar_a, libzigzag_a));
  v.push_back(std::make_pair(zig_cpp, zig_o));
  v.push_back(std::make_pair(zig_o, libzigzag_a));
  v.push_back(std::make_pair(zag_cpp, zag_o));
  v.push_back(std::make_pair(zag_o, libzigzag_a));
  v.push_back(std::make_pair(libzigzag_a, killerapp));
  return v;
}

void TestGraphViz()
{
  const std::vector<std::string> names = GetNames();
  const std::vector<std::pair<int,int> > used_by = GetEdges();

  const int n_edges = used_by.size();
  std::vector<int> weights(n_edges,1);

  typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::property< boost::vertex_color_t, boost::default_color_type >,
    boost::property< boost::edge_weight_t, int >
    > Graph;

  Graph g(used_by.begin(), used_by.end(), weights.begin(), N); //WORKS

  //Write graph to file
  {
    std::ofstream f("newtest.dot");
    boost::write_graphviz(f, g, boost::make_label_writer(&names[0]));
    f.close();
  }

  //std::system("kgraphvie1wer test.dot");
}

/* upgraded boost from 1.40 to 1.44
*  This piece of code went broken after the upgrade
*  typedef boost::GraphvizDigraph doesn't work anymore
	typedef boost::GraphvizDigraph Graph;
	typedef boost::graph_traits<boost::GraphvizDigraph>::edge_descriptor EdgeDescriptor;
*/
void JaggedMatrix::Serialize(std::string filepath)
{
	typedef std::pair<int,int> Edge;

	std::vector<std::pair<int,int> > used_by;
	int index=0;

	for(unsigned i=0; i < _size; i++)
	{
			for(unsigned j=0; j < _size; j++)
			{
				if( _jaggedMatrix(i,j).size()==0)
					continue;

				used_by.push_back(std::make_pair(i,j));
			}
	}


  std::vector<std::string> names;
  names.push_back("dax.h");
  names.push_back("yow.h");

  const int n_edges = used_by.size();
  std::vector<int> weights(n_edges,1);

  typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::property< boost::vertex_color_t, boost::default_color_type >,
    boost::property< boost::edge_weight_t, int >
    > Graph;

  Graph g(used_by.begin(), used_by.end(), weights.begin(), n_edges-1); //WORKS

  //Write graph to file
  {
    std::ofstream f("newtest.dot");
    boost::write_graphviz(f, g, boost::make_label_writer(&names[0]));
    f.close();
  }


 //int result = std::system("C:\\Civil-7\\Research\\multi-state\\Proposal\\Chapter 5 - SMP\\VCProjects\\BoostTest\\BoostTest\\HenleyTest\\matlab\\plots\\dot2tex-2.8.7\\dot2tex\\dot2tex.py -ftikz newtest.dot > newtestdot.tex");
//"C:\\Program Files\\MiKTeX 2.9\\miktex\\bin\\x64\\pdflatex.exe" -interaction=nonstopmode "%pm"



}


matrix<double> JaggedMatrix::GetMarkovTransitionMatrix()
{
	matrix<double> result(_size, _size);

	for(size_t	r = 0; r < _size; r++)
	{
		for(size_t	c = 0; c < _size; c++)
		{
			result(r,c) = 0;

			if( _meanMatrix(r,c).size() != 0)
			{
				if( _meanMatrix(r,c)[0] != 0)
				{
					result(r, c) = 1 / _meanMatrix(r, c)[0];
				}
			}

			std::cout << result(r,c) << "\t";

		}
		std::cout << std::endl;

	}
	return result;
}