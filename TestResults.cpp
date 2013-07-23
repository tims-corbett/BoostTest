#include "TestResults.h"

TestResults::TestResults(std::string filename)
	{
		_filename = filename;
	}


void TestResults::ClearResults()
	{
		_results.clear();
	}

void TestResults::AddResult(vector<double> column)
	{
		_results.push_back(column);
	}

void TestResults::Serialize()
	{
		std::ofstream ofs(_filename.c_str());

		int colCount = _results.size();
		int phiSize = _results[0].size();

		for( int i=0; i< phiSize; i++)
		{

			for(int j=0; j< colCount; j++)
				ofs << _results[j][i] << "\t";

			ofs << std::endl;
		}
		ofs.close();
	}

std::vector<double> TestResults::Load(long& rowCount, int& columnCount)
{
		std::string line;
		std::vector<double> inputData;

		{
		std::ifstream ifs(_filename.c_str());
		//count rows
		rowCount = (long)std::count(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), '\n');
		}


		std::ifstream ifs(_filename.c_str());

		//count columns
		getline(ifs, line);
		columnCount = (int)std::count(line.begin(), line.end(), '\t');
	

		do
		{
			std::size_t prev = 0, pos;
			while ((pos = line.find_first_of("\t", prev)) != std::string::npos)
			{
				if (pos > prev)
					inputData.push_back(atof(line.substr(prev, pos-prev).c_str()));
				prev = pos+1;
			}

			if (prev < line.length())
				inputData.push_back(atof(line.substr(prev, std::string::npos).c_str()));

		}while(getline(ifs, line));

		return inputData;
}

//columnNumber is zero based
std::vector<double> TestResults::ExtractXY(std::vector<double> inputData,  int columnNumber, long rowCount, int columnCount)
{
	std::vector<double> result(rowCount * 2);
	
	long j = 0;

	for(long i = 0; i < inputData.size(); i += columnCount)
	{
		result.at(j) = inputData.at(i);
		result.at(j + rowCount) = inputData.at( i + columnNumber );
		j++;
	}

	return result;

}