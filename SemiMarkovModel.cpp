
#include "SemiMarkovModel.h"
#include "ExpandableMatrix.h"
#include "../Quadrature/OouraQuadrature.h"


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/banded.hpp> 

#include <boost/numeric/ublas/lu.hpp>

#include <fstream>


#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>


SemiMarkovModel::~SemiMarkovModel()
{
}

void SemiMarkovModel::WaitingTimeMatrix(double t, matrix<double>& C, vector<double>& w)
{
	if(_waitingHandler != NULL)
	{
		w = _waitingHandler(t);	//TESTMODEL("W[i]: ", W[i])
		return;
	}

	w = vector<double>(StateCount);    
	std::fill(w.begin(), w.end(), 0.0);

    for (size_t c = 0; c < StateCount; c++)
	{
		matrix_column< matrix<double>> col(C, c);
		w += col;
	}
}

double SemiMarkovModel::StateWaitingTime(int t, int state)
{
	double w = 0.0;

	vector<double> krow = KernelRow(t,state);

    for (size_t c = 0; c < StateCount; c++)
		w += krow[c];

	return w;
}

void  SemiMarkovModel::NoWaitingPolyWbl(double t, 	matrix<double>& W)
{
	W = identity_matrix<double>(StateCount);
	for(size_t i=0; i<StateCount; i++)
		W(i,i) = _model->RowReliabilityProduct(t,i);
	//TESTMODEL("W", W)
}

vector<double> SemiMarkovModel::Trapz2Points(int t, const vector<vector<double>>& f)
{
	return  0.5 * dt * (f[t] +  f[t-1]);
}

void SemiMarkovModel:: NoWaitingTedious(int t, 	vector<matrix<double>>& W, const vector<vector<double>>& w )
{
	vector<double> v = Trapz2Points(t, w);

	if(t==1)
	{
		vector<double>n1 = ones - v;
		W[t] = diagonal_matrix<double>(n1.size(), n1.data());
	}
	else
	{
		diagonal_matrix<double> secondInt;
		secondInt = diagonal_matrix<double>(v.size(), v.data());
		W[t] = W[t-1] - secondInt;
	}
	//Diagnostics
	//TESTMODEL("w[t-1]: ", w[t-1]);
	//TESTMODEL("w[t]: ", w[t]);
	//TESTMODEL("2ndIntegral: ", secondIntegral);
}

/// <summary>
/// The kernel or core of the semi-markov process model
/// </summary>
/// <param name="t"></param>
/// <returns>the kernel at time t</returns>
void SemiMarkovModel::KernelMatrix(double t, matrix<double>& C)
{
	if( _kernelHandler != NULL)		
	{
		C = _kernelHandler(t);	//TESTMODEL("C[i]: ", C[i])
		return;
	}
	
	C =  matrix<double>(StateCount, StateCount);	
    matrix<double>  PdfMat = _model->pdf(t);


    if (StateCount <= 2)
	{
		C.assign( PdfMat );     
		return;
	}

    matrix<double>  OneMinusCdfMat = _model->Reliability(t);	//TESTMODEL("1-CDF", OneMinusCdfMat)
    vector<double>  prodOneMinusCdf(StateCount);

	for(size_t col = 0; col < StateCount; col++)
	{
		std::fill(	prodOneMinusCdf.begin(),
			prodOneMinusCdf.end(),
			1.0 );

		for (size_t c = 0; c < StateCount; c++)
		{
			if( col != c)
			{
				matrix_column< matrix<double>> RelCol(OneMinusCdfMat, c);
				prodOneMinusCdf.assign( element_prod(prodOneMinusCdf, RelCol ));
			}
		}
		matrix_column< matrix<double>> pdfcol(PdfMat, col);
		column(C, col).assign(element_prod(pdfcol, prodOneMinusCdf));
	}
	//TESTMODEL("IntegralTest ", C)
}


/// <summary>
/// The kernel or core of the semi-markov process model
/// </summary>
/// <param name="t"></param>
/// <returns>the kernel at time t</returns>
vector<double> SemiMarkovModel::KernelRow(double t, int r)
{
	vector<double> C(StateCount);

    vector<double>  PdfVector = _model->pdf(t,r);

    if (StateCount <= 2)
	{
		C.assign( PdfVector );     
		return C;
	}

    vector<double>  OneMinusCdf = _model->Reliability(t,r);	//TESTMODEL("1-CDF", OneMinusCdfMat)
    vector<double>  prodOneMinusCdf(StateCount);



	for(size_t col = 0; col < StateCount; col++)
	{
		prodOneMinusCdf[col] = 1.0;

		for (size_t c = 0; c < StateCount; c++)
		{
			if( col != c)
				prodOneMinusCdf[col] *= OneMinusCdf[c];
		}
		C[col] =  PdfVector[col] * prodOneMinusCdf[col];

	}
	//TESTMODEL("IntegralTest ", C)
	return C;
}

double SemiMarkovModel::KernelElement(double t, int r, int col)
{
	double k = 0;

    double dpdf   = _model->pdf(t,r,col);

    if (StateCount <= 2)
	{
		return dpdf;
	}

    vector<double>  OneMinusCdf = _model->Reliability(t,r);	//TESTMODEL("1-CDF", OneMinusCdfMat)

	double  prodOneMinusCdf;

	prodOneMinusCdf = 1.0;

	for (size_t c = 0; c < StateCount; c++)
	{
		if( col != c)
			prodOneMinusCdf *= OneMinusCdf[c];
	}
	k =  dpdf * prodOneMinusCdf;

	return k;
}

SemiMarkovModel::SemiMarkovModel(JaggedMatrix* model)
{
	bContinue = true;
    _model = model;
	StateCount = _model->_size;
	_kernelHandler = NULL;
}

void SemiMarkovModel::SetModelInput(double missionTime, int NSteps)
{	
	N = NSteps;
	MissionTime = missionTime;
    dt = MissionTime / N;
	StateCount = _model->_size;

	ones = vector<double>(StateCount);
	std::fill(ones.begin(), ones.end(), 1.0);

}

void SemiMarkovModel::SetupMatrices()
{
    double dtHalf = dt / 2;


		//State Probabilities
	phi.resize(N+1); //phi = vector<matrix<double>>(N+1);
	C.resize(N+1); //C = vector<matrix<double>>(N+1);
	W.resize(N+1); //W = vector<matrix<double>>(N+1);
	helper.resize(N+1); //helper = vector<matrix<double>>(N+1);
	w.resize(N+1); //w = vector<vector<double>>(N+1);


    identity_matrix<double> identity(StateCount);

	KernelMatrix(0, C[0]);			//TESTMODEL("C[0]: ", C[0])
	WaitingTimeMatrix(0, C[0], w[0] );		//TESTMODEL("w[i]: ", w[i])

    W[0] = identity;
	helper[0] = W[0] - dtHalf * C[0];		//used inside the MRE


    for (int i = 1; i < N + 1; i++)
	{
		KernelMatrix(i * dt, C[i]);	//TESTMODEL("C[i]: ", C[i])
		WaitingTimeMatrix(i*dt, C[i], w[i] );		//TESTMODEL("w[i]: ", w[i])
		
       // NoWaitingTedious(i, W, w);		
		NoWaitingPolyWbl(i*dt, 	W[i]); //TESTMODEL("W[i]: ", W[i])

		helper[i] = W[i] - dtHalf * C[i] ;
		phi[i].resize(StateCount, StateCount, false);
	}

	phi[0] =  identity;
}


/// <summary>
/// Evaluates the Markov RENEWAL EQUATION
/// and dumps the state probabilities in SemiMarkovModel.phi[]
/// </summary>
/// <param name="MissionTime">Evaluates state probabilities up to MissionTime</param>
/// <param name="N">Number of points on the timeline for discrete summation</param>
void SemiMarkovModel::ComputeStateProbabilities(boost::function<void (SemiMarkovModel*)> TimeStepCallBack)
{
    double dtHalf = dt / 2;

    identity_matrix<double> identity(StateCount);
	matrix<double> offset = identity - dtHalf * C[0];		//TESTMODEL("offset ", offset)
	
	permutation_matrix<> pm(offset.size1());
	matrix<double> copiedMatrix = matrix<double>(offset);
	lu_factorize(copiedMatrix,pm);
	matrix<double> inverseMatrix(identity_matrix<double>(copiedMatrix.size1()));
	lu_substitute(copiedMatrix,pm,inverseMatrix);

	offset = inverseMatrix;			//TESTMODEL("offsetinv ", offset)

	ExpandableMatrixPair matrixPair( N+1, StateCount, StateCount, StateCount, dt);


	matrix<double> dtHalfPhi0 =  dtHalf * phi[0];	//TESTMODEL("dtHalfPhi0 ", dtHalfPhi0)		
	matrix<double> temp(StateCount, StateCount);

	boost::shared_ptr<boost::thread> m_thread;
	
	if(TimeStepCallBack != NULL)
		_timeStepCallBack = TimeStepCallBack;

//	m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&TimeStepCallBack, timeStep)));
		
	m_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&SemiMarkovModel::DisplayTimeStep, this)));
	bWasForcefullyTerminated = false;

	for (timeStep = 1; (timeStep < N + 1) && bContinue; timeStep++)
    {
		matrixPair.Add(C[timeStep], phi[timeStep-1], temp);
		phi[timeStep].assign( prod(offset, (helper[timeStep] + temp  )));		
    }

	bContinue = false;

	if( timeStep < N+1)
		bWasForcefullyTerminated=true;

	m_thread->join();
}

void SemiMarkovModel::RegisterHandlers(boost::function<matrix<double> (double)>  kernelHandler, 
									   boost::function<vector<double> (double)> waitingHandler)
{
	_kernelHandler = kernelHandler;
	_waitingHandler = waitingHandler;
}

void SemiMarkovModel::DisplayTimeStep()
{

	_timeStepCallBack(this);

	//boost::xtime xt;
	//try
	//{
	//	while(timeStep > 0 )
	//	{
	//		std::cout << timeStep << "\t";
	//		boost::xtime_get(&xt, boost::TIME_UTC);
	//		xt.sec += 1; // change xt to next second
	//		boost::thread::sleep(xt); 
	//
	//	}
	//}catch(...)
	//{
	//}

}

vector<double> SemiMarkovModel::GetStateProbability(
			int initState, 
			int targetState
			)
{
	size_t phiSize = phi.size();

	if(initState == -1)	
		initState = (int)StateCount - 1;

	vector<double> result(phiSize);
	
	for( size_t i=0; i<phiSize; i++)
	{
			result(i) = phi[i](initState, targetState);
	}
	return result;
}

vector<vector<double>> SemiMarkovModel::GetStateProbabilities(int initState	)
{
	size_t phiSize = phi.size();

	if(initState == -1)	
		initState = (int)StateCount - 1;

	vector<vector<double>> result(StateCount);
	
	for(size_t j=0; j < StateCount; j++)
	{
		vector<double> stateprob(phiSize);
		for( size_t i=0; i<phiSize; i++)
		{
				stateprob(i) = phi[i](initState, j);
		}
		result(j) = stateprob;
	}
	return result;
}

bool SemiMarkovModel::PathExists(int fromState, int toState)
{
	return _model->PathExists(fromState, toState);
}

double SemiMarkovModel::SampleTimeSpent(int inState, int toState, double p)
{
	if(inState == toState)
	{
		return 0;
	}

	return _model->SampleTimeSpent(inState, toState,p);
}

vector<double> SemiMarkovModel::GetTimeVector()
{
	vector<double> timesteps(N + 1 );

	for(int i=0; i < N+1; i++)
		timesteps(i) = i*dt;

	return timesteps;
}

matrix<double> SemiMarkovModel::MeanWaitingTime()
{
	double tiny, aw[100000], ans, err;
	int lenaw;

	matrix<double> result(StateCount,StateCount);

    lenaw = 100000;
    tiny = 1.0e-307;

	IntegralHelper ihelper(this);

	for(size_t r=0; r<StateCount; r++)
	{
		ihelper.row = r;
		for(size_t c=0; c<StateCount; c++)
		{
			ihelper.column = c;
			intdeiini(lenaw, tiny, 1.0e-15, aw);
			intdei(&SemiMarkovModel::IntegralCallback, 0.0, aw, &ans, &err, &ihelper);
			result(r,c) = ans;
		}
	}

	return result;
}


double SemiMarkovModel::IntegralCallback(double x, void* cargo)
{
	IntegralHelper* iHelper = (IntegralHelper*)cargo;
	return x * iHelper->model->KernelElement(x, iHelper->row, iHelper->column);
}

double SemiMarkovModel::TestValidity(double t, int init)
{
	int N = (int) (MissionTime / dt);

	if(init == -1)
		init = (int)StateCount - 1;
	
	double sum = 0;

	for(size_t i=0; i<StateCount; i++)
		sum += phi[N](init, i);
	return sum;
}

void SemiMarkovModel::IntegralSystemEquations(std::string filepath)
{
	std::string result = "";
	std::ostringstream temp;
	std::string prev;

	TESTMODEL("AdjMat",_model->_adjacencyMatrix)


	temp << "\\begin{align*}" << std::endl;

	for(size_t i=0; i < StateCount; i++)
	{
		for(size_t j=0; j < StateCount; j++)
		{
			temp << "\\phi_{" << i << j << "}(t) = &";

			if(i==j)
			{
				temp << "W_" << i << "(t) + "; 
			}

			prev = "";
			for(size_t k=0; k<StateCount; k++)
			{
				if(_model->_adjacencyMatrix(i,k) == 1)
				{
					if( k != 0 && prev != "")
						temp << "+";
					temp << "\\int_0^t{ C_{" << i << k << "}(\\tau)\\phi_{" << k << j << "}(t-\\tau) }d\\tau ";
					prev = "+";
				}
			}

			temp << "\\\\" << std::endl;
			std::cout << temp.str();
		}

	}

	temp << "\\end{align*}" ;
	std::ofstream ofs(filepath.c_str());
	ofs << temp.str();
	ofs.close();

}

//
//matrix<double> SemiMarkovModel::GetMarkovTransitionProbabilityMatrix()
//{
//	matrix<double> mtp(StateCount, StateCount);
//
//	//***************** zero out all diagonal entries
//	matrix<double> A = model->GetMarkovTransitionMatrix();
//
//	double rowsum;
//
//	for(int i = 0; i < StateCount; i++)
//	{
//		rowsum = std::accumulate(A.begin2(), A.end2(), 0);
//
//		for( int j=0; j<StateCount; j++)
//		{
//			mtp(i,j) = A(i,j) / rowsum;
//		}
//	}	
//	return mtp;
//}
//
//vector<double> SemiMarkovModel::ImbddedChainStationaryDistribution()
//{
//		matrix<double> A = model->GetMarkovTransitionMatrix();
//		matrix<double> p_ij = model.GetMarkovTransitionProbabilityMatrix();
//
//		matrix_identity<double> eye(StateCount);
//
//		p_ij.assign(p_ij - eye);
//
//		for(int i=0; i<StateCount; i++)
//			p_ij(i, StateCount-1) = 1;
//
//		
//		z = zeros(len, 1);
//		z(end) = 1;
//
//		imChain = linsolve(p_ij, z,opts)';
//
//		Ainv = identity_matrix<float>(A.size1());
//		permutation_matrix<size_t> pm(A.size1());
//		lu_factorize(A,pm)
//		lu_substitute(A, pm, Ainv);
//
//
//}




/*
boost::thread thr1( boost::bind( &X::run, &x ) );
boost::thread thr2( boost::bind( &X::complexOperation, &x, 30 ) );
boost::thread thr3( boost::bind( &X::complexOperation, &x, 100 ) );
*/