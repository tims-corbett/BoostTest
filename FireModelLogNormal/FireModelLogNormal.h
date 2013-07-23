#ifndef FireModelLogNormal_H
#define FireModelLogNormal_H

//#include "../JaggedMatrix.h"

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

class SemiMarkovModel;

class FireModelLogNormal
{

	boost::mutex m_timeStepMutex;

public:
	FireModelLogNormal();
	void RunModel();
	void RunSMP();
	void RunBerlinSMP();
	void RunBerlinExponentialSMP();

	void RunMarkov();

	void DisplayTimeStep(SemiMarkovModel*);
	void TestDistributions();


};


#endif