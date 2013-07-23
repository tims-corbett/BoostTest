#ifndef TEST_RESULTS
#define TEST_RESULTS

#include <boost/numeric/ublas/vector.hpp>
#include <fstream>

using namespace boost::numeric::ublas;

class TestResults
{
	std::string _filename;
public:
	std::vector<vector<double>> _results;

	TestResults() {_filename="";}
	TestResults(std::string filename);
	TestResults(TestResults& res) { _filename = res._filename; }

	void ClearResults();
	bool IsEmpty() { return _results.empty(); }
	void AddResult(vector<double> column);
	void Serialize();
	std::vector<double> Load(long& rowCount, int& columnCount);
	std::vector<double> ExtractXY(std::vector<double> inputData,  int columnNumber, long rowCount, int columnCount);
};

#endif