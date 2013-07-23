#ifndef DEBUGTOOLS
#define DEBUGTOOLS

#include<iostream>
#include<iomanip>


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;


void DisplayMatrix(std::string message, matrix<std::vector<double>> m);
void DisplayMatrix(std::string message, matrix<double> m);
void DisplayMatrix(std::string message, matrix<int> m);
void DisplayMatrix(std::string message, vector<double> m);
void DisplayMatrix(std::string message, std::vector<double> m);

#define TESTMODEL(x,m)	DisplayMatrix(x,m); 

#endif

