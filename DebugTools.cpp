
#include<iostream>
#include<iomanip>


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;


void DisplayMatrix(std::string message, matrix<std::vector<double>> m)
{
	std::cout << message <<  std::endl;

	double val = 0;
	for(size_t i=0;i<m.size1();i++)
	{
		for(size_t j=0;j<m.size2();j++)
		{
			val = (m(i,j).size()==0 ) ? 0 : m(i,j)[0];
			std::cout << std::setprecision(14) << val << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void DisplayMatrix(std::string message, matrix<double> m)
{
	std::cout << message <<  std::endl;
	for(size_t i=0;i<m.size1();i++)
	{
		for(size_t j=0;j<m.size2();j++)
			std::cout << std::setprecision(14) << m(i,j) << "\t";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void DisplayMatrix(std::string message, matrix<int> m)
{
	std::cout << message <<  std::endl;
	for(size_t i=0;i<m.size1();i++)
	{
		for(size_t j=0;j<m.size2();j++)
			std::cout << std::setprecision(14) << m(i,j) << "\t";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void DisplayMatrix(std::string message, vector<double> m)
{
	std::cout << message << " " << m << std::endl;
}

void DisplayMatrix(std::string message, std::vector<double> m)
{
	std::cout << message << " [";
	for(std::vector<double>::const_iterator it = m.begin(); it != m.end(); it++)
		std::cout << std::setprecision(14) << *it << " ";

	std::cout << " ]" << std::endl;
}

