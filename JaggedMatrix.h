#ifndef JAGGED_matrix
#define JAGGED_matrix




#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/ptr_container/ptr_vector.hpp>


#include "Distributions.h"

using namespace boost::numeric::ublas;

class TransitionInfo
{
public:

	int _from,  _to;
	double _meantime, _cov;
	DistributionFactory::DistributionType _type;


	TransitionInfo(int from, int to,
					double meantime, double cov,
					DistributionFactory::DistributionType type):
		_from(from), _to(to), _meantime(meantime), _type(type)
	{
	}
};


class  JaggedMatrix
{
	friend class SemiMarkovModel;

protected:
	matrix< boost::ptr_vector<Distribution>> _jaggedMatrix;

	matrix<std::vector<double>> _meanMatrix;
	matrix<std::vector<double>> _covMatrix;

	size_t _size;
	matrix<int> _adjacencyMatrix;
	int _edgeCount;
		
	enum {COMPONENT_DOWN = 0, REPAIR_COV = 1 };

public:
	JaggedMatrix();
	JaggedMatrix(int n);

	size_t GetSize() { return _size; }

	 void AddDistribution(size_t r, size_t c, Distribution* distribution);
	 void AddDistribution(const TransitionInfo& info);

	 void AddDistribution(size_t r, size_t c, double meantime, double cov,
		 DistributionFactory::DistributionType failuretype=DistributionFactory::Weibull);


void AddDistribution(size_t r, size_t c, 
				   double meantime, double cov, 
				   double repairtime, double repaircov = 1,
				   DistributionFactory::DistributionType failuretype=DistributionFactory::Weibull,
				   DistributionFactory::DistributionType repairtype=DistributionFactory::Weibull

			   );



void BuildModel(matrix<std::vector<double>> mean, 
										  matrix<std::vector<double>> cov,
										  DistributionFactory::DistributionType distributionType
										  );

	 JaggedMatrix(matrix<std::vector<double>> mean, 
		 matrix<std::vector<double>> cov,
		 DistributionFactory::DistributionType type );

JaggedMatrix(
		   int componentCount,
		   double failureTime[], 
		   double failurecovs[], 
		   double repairTime[] = NULL,
		   double repaircovs[] = NULL,
		   DistributionFactory::DistributionType failuretype = DistributionFactory::Weibull,
		   DistributionFactory::DistributionType repairtype = DistributionFactory::Weibull
		   );

	 
	 vector<double> Cellpdf(double t, size_t r, size_t c);
	 vector<double> CellReliability(double t, size_t r, size_t c);
	 double RowReliabilityProduct(double t, size_t r);

	 
	 double pdf(double t, size_t i, size_t j);		//cell of a matrix
	 matrix<double> pdf(double t);				//entire matrix
	 vector<double> pdf(double t, size_t r);		//row of a matrix

	 double Reliability(double t, size_t r, size_t c);
	 matrix<double> Reliability(double t);
	 vector<double> Reliability(double t, size_t r);

	 matrix<double> cdf(double t);
	 double cdf(double t, size_t r, size_t c);

	 matrix<double> Hazard(double t);

	 double SampleTimeSpent(int inState, int toState, double p);
	 bool PathExists(int fromState, int toState);

	 void Serialize(std::string filepath);

	 matrix<double> GetMarkovTransitionMatrix();
	 matrix<std::vector<double>> GetMeanMatrix() { return _meanMatrix; }

	~JaggedMatrix();

	//Diagnostics
	void Display();
	void DisplayInputs();


};
#endif
