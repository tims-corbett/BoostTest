#ifndef SEMIMARKOVMODEL
#define SEMIMARKOVMODEL

#define DEBUG_MODEL
#include "DebugTools.h"


#include "JaggedMatrix.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/function.hpp>


using namespace boost::numeric::ublas;



class SemiMarkovModel
{
public:
		friend static double SemiMarkovModel::IntegralCallback(double x, void* cargo);
		friend class SMPMonteCarlo;
public:

		SemiMarkovModel() { _model = NULL; _kernelHandler = NULL;}

        JaggedMatrix* _model;
		size_t StateCount;

        double dt;
		int N;
		double MissionTime;

		vector<matrix<double>> phi;
		vector<matrix<double>> C;
		vector<matrix<double>> W;
		vector<matrix<double>> helper;
		vector<vector<double>> w;


        int timeStep ;		//For thread to display progress
		void DisplayTimeStep();	//thread handler to display updates
				
		boost::function<matrix<double> (double)>  _kernelHandler; 
		boost::function<vector<double> (double)>  _waitingHandler;
		boost::function<void (SemiMarkovModel*)> _timeStepCallBack;
	
		bool bContinue;
		bool bWasForcefullyTerminated;

		vector<double> ones;

		inline vector<double> Trapz2Points(int t, const vector<vector<double>>& f);

public:

	SemiMarkovModel(JaggedMatrix* model);

	~SemiMarkovModel();

	void SetModelInput(double mission, int steps);

	void ComputeStateProbabilities(boost::function<void (SemiMarkovModel*)> TimeStepCallBack = NULL);
	void SetupMatrices();
	
	void KernelMatrix(double t, matrix<double>& C );
	vector<double> KernelRow(double t, int r);
	double KernelElement(double t, int r, int c);
	
	void WaitingTimeMatrix(double t, matrix<double>& C, vector<double>& v);
	double StateWaitingTime(int t, int state);
	matrix<double> MeanWaitingTime();

	void  NoWaitingTedious(int t, 	vector<matrix<double>>& W, const vector<vector<double>>& w );
	void  NoWaitingPolyWbl(double t, 	matrix<double>& W);

	void RegisterHandlers(boost::function<matrix<double> (double)>  kernelHandler, 
		boost::function<vector<double> (double)> waitingHandler);

	//Utilities
	vector<double> GetStateProbability(int initState, int targetState);
	vector<vector<double>> GetStateProbabilities(int initState	);
	bool PathExists(int fromState, int toState);
	double SampleTimeSpent(int inState, int toState, double p);

	vector<double> GetTimeVector();
	static double IntegralCallback(double x, void* cargo);

	

	//Diagnostics
	void IntegralSystemEquations(std::string filepath);
	double TestValidity(double t, int init);
};



class IntegralHelper
{
public:

	SemiMarkovModel* model;
	size_t row;
	size_t column;

	IntegralHelper(SemiMarkovModel* smpmodel) : model(smpmodel) { }
};

#endif