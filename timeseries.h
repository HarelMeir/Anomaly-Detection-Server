#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include "anomaly_detection_util.h"
#include <iostream>
#include <map>
#include <vector>
using namespace std;

/**
 * @brief An object that stroes the csv data.
 * 
 */
class TimeSeries{
	//private
	const char* filename;
	std::map<std::string, std::vector<float>> table;
	std::vector<std::string> col_vec;
	
public:
	//constructor.
	TimeSeries(const char* CSVfileName);
	//read from csv file.
	std::map<std::string, std::vector<float>> getTable();
	
	//setter for the colNames vec.
	std::vector<std::string> setColNames();

	//a getter to it.
	std::vector<std::string> getColNames();

	//m map getting all the values to the csv.
	std::map<std::string, std::vector<float>> inputFromCsv();

	//adding a line to the table.
	void addLine();

	const char* getFilename() {
		return this->filename;
	}
};



#endif /* TIMESERIES_H_ */
