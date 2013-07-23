#ifndef COMPUTER_INTERACTION_H
#define COMPUTER_INTERACTION_H

#include "../JaggedMatrix.h"
#include "../SemiMarkovModel.h"

//Probabilistic risk assessment modeling of digital instrumentation and control
//systems using two dynamic methodologies

//T. Aldemir , S.Guarro b, D.Mandelli , J.Kirschenbaum , L.A.Mangan , P.Bucci , M.Yau , E.Ekici ,
//D.W. Miller , X.Sun , S.A.Arndt 


class FeedWaterController
{
public:
	FeedWaterController();
	void TestDistributions();
	void RunModel();
	void RunSMP();
};


#endif