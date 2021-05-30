#include <fstream>
#include <iostream>
#include "timeseries.h"
#include <utility>
#include <sstream>

/**
 * @brief method for printing a vector from any kind.
 * 
 * @tparam T 
 * @param vec 
 */
template <typename T>
void printVec(std::vector<T> vec) {
    for(T var: vec) {
        std::cout << var << " ,";
    }
    std::cout << std::endl;
}

/**
 *  Constructor.
 */
TimeSeries::TimeSeries(const char* CSVfileName) : filename(CSVfileName) {
    col_vec = setColNames();
    table =  this->inputFromCsv();
}

//getter for the table.
std::map<std::string, std::vector<float>> TimeSeries::getTable() {
    return table;
}

//getting the column names into a vector.
std::vector<std::string> TimeSeries::setColNames() {
    //opening the file.
    std::ifstream file(filename);
    std::vector<std::string> vec;
    std::string line,column;
    //checks if the file is open.
    if(!file.is_open()) {
        throw std::runtime_error("unable to open the file");
    }
    if(file.good()) {
        //reading the first line.
        std::getline(file, line);
        //using string stream.
        std::stringstream ss(line);

        //adding every columan as a key to the map.
        while(std::getline(ss, column, ',')) {
            vec.push_back(column);
        }
    }
    file.close();
    return vec;
}

//getter for col vec.
std::vector<std::string> TimeSeries::getColNames() {
    return col_vec;
} 





//setting up the table.
std::map<std::string, std::vector<float>> TimeSeries::inputFromCsv() {
    //intilizing map of string and vector(float).
    std::map<std::string, std::vector<float>> table;

    //opening the file.
    std::ifstream file(filename);

    //checks if the file is open.
    if(!file.is_open()) {
        throw std::runtime_error("unable to open the file");
    }

    //variabls for containing the csv data.
    std::string line,colunm;
    float num;
    std::vector<float> vec;

    std::vector<std::string> colVec = getColNames();
    for(string s : colVec) {
        table.insert(std::pair<std::string, std::vector<float>>(s, vec));
    }

    while(!file.eof()) {
       std::getline(file, line);
       std::stringstream ss(line);
       
       int c_i = 0;
        while(ss >> num) {
            table[col_vec[c_i]].push_back(num);
            if(ss.peek() == ',') {
                ss.ignore();
            }
           c_i++;
        }
    }
    return table;
}