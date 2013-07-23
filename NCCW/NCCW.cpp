#include "NCCW.h"

#include "../SemiMarkovModel.h"

NCCW::NCCW()
{
	lambda_1 = 7.7243e-6;
	lambda_H = 3.4243e-6;
	mu = 1/19.4;

	results = TestResults("NCCW\\NCCWPT.dat");

}

void NCCW::BuildModel(double cov)
{
	JaggedMatrix *jm = new JaggedMatrix(3);
	
	jm->AddDistribution(1,0, 1/lambda_1, cov);
	jm->AddDistribution(1,0, 1/lambda_H, 1);

	jm->AddDistribution(1,2, 1/mu, 1.0);
	jm->AddDistribution(2,0, 1/lambda_H, 1);

	jm->AddDistribution(2,1, 1/lambda_1, cov);
	jm->AddDistribution(2,1, 1/lambda_1, cov);

	jm->Display();

	SemiMarkovModel smp (jm);
	smp.SetModelInput( 100, 30000);
	smp.SetupMatrices();

	if(results.IsEmpty())
	{
		time = smp.GetTimeVector();
		results.AddResult(time);
	}

	smp.ComputeStateProbabilities();
	results.AddResult(smp.GetStateProbability(-1,0));
}

void NCCW::RunModel()
{
	for(double cov = 0.3; cov <= 1.3; cov+=0.1)
	{
		BuildModel(cov);
	}
	
	results.Serialize();	
}



void main()
{
	NCCW model;
	model.RunModel();
}
