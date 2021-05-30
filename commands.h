

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <sstream>
#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

class standartIO: public DefaultIO {
	ifstream in;
	ofstream out;
public:
	standartIO(string inputFile,string outputFile){
		in.open(inputFile);
		out.open(outputFile);
	}
	virtual string read(){
		string s;
		in>>s;
		return s;
	}
	virtual void write(string text){
		out<<text;
	}

	virtual void write(float f){
		out<<f;
	}

	virtual void read(float* f){
		in>>*f;
	}
    ~standartIO(){};
};

// you may add here helper classes
class commandsSharedInfo{
public:
    HybridAnomalyDetector ad;
    TimeSeries* ts1;
    TimeSeries* ts2;
	int numOfLines;
	std::vector<AnomalyReport> vecAR;
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
public:
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute()=0;
	virtual ~Command(){}
};

class uploadCsv: public Command {
    commandsSharedInfo* csi;
public:
    uploadCsv(DefaultIO* dio,commandsSharedInfo* csi) : Command(dio){
        this->csi = csi;
    }
    void execute(){
		ofstream anomalyTrain;
		ofstream anomalyTest;
		anomalyTrain.open("anomalyTrain.csv");
		anomalyTest.open("anomalyTest.csv");


        dio->write("Please upload your local train CSV file.\n");

        std::string str;
        while(str != "done") {
            str = dio->read();
			anomalyTrain<<str;
			anomalyTrain<<"\n";
        }
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
		str = dio->read();
		int numOfLines = 0;
        while(str != "done") {
			numOfLines++;
			anomalyTest<<str;
			anomalyTest<<"\n";
            str = dio->read();      
        }
        dio->write("Upload complete.\n");
		csi->ts1 = new TimeSeries("anomalyTrain.csv");
		csi->ts2 = new TimeSeries("anomalyTest.csv");
		csi->numOfLines = numOfLines;
		anomalyTrain.close();
		anomalyTest.close();
    }
	~uploadCsv(){}

};

class algorithmSetting : public Command {
    commandsSharedInfo* csi;
public:
    algorithmSetting(DefaultIO* dio,commandsSharedInfo* csi) : Command(dio){
        this->csi = csi;
    }

	void execute() {
		HybridAnomalyDetector ad;
		dio->write("The current correlation threshold is 0.9\nType a new threshold\n");
		std::string str = dio->read();
		float userThreshold = stof(str);
		if(0 <= userThreshold && userThreshold <=1) {
			ad.setThreshold(userThreshold);
			csi->ad = ad;
		} else {
			dio->write("please choose a value between 0 and 1.\n");
			execute();
		}
		return;
	}

};
// implement here your command classes
class detectAnomalies : public Command {
    commandsSharedInfo* csi;
public:
    detectAnomalies(DefaultIO* dio,commandsSharedInfo* csi) : Command(dio){
        this->csi = csi;
    }

	void execute() {
		csi->ad.learnNormal(*(csi->ts1));
		csi->vecAR = csi->ad.detect(*(csi->ts2));
		dio->write("anomaly detection complete.\n");
	}
};

class displayResults : public Command {
    commandsSharedInfo* csi;
public:
    displayResults(DefaultIO* dio,commandsSharedInfo* csi) : Command(dio){
        this->csi = csi;
    }
	void execute() {
		int size = csi->vecAR.size();
		std::string str,description;
		int time;
		for(int i = 0; i < size; i++) {
			time = csi->vecAR[i].timeStep;
			description = csi->vecAR[i].description;
			str = std::to_string(time) + "\t " + description + "\n";
			dio->write(str);
		}
		dio->write("Done.\n");
	}
};


class uploadAnomalies : public Command {
    commandsSharedInfo* csi;
public:
    uploadAnomalies(DefaultIO* dio,commandsSharedInfo* csi) : Command(dio){
        this->csi = csi;
    }

	void execute() {
		dio->write("Please upload your local anomalies file.\nUpload complete.\n");
		std::vector<int> dontOverdo;
		int result, differentGroups = 1;
		int N, P = 0, n,from, to;
		float TP = 0, FP,trueRate, falseRate, groupNum;
		//counts the different groups.
		for(int i = 1; i < csi->vecAR.size(); i++) {
			result = csi->vecAR[i].timeStep - csi->vecAR[i-1].timeStep;
			if(result != 1 || csi->vecAR[i].description != csi->vecAR[i-1].description) {
				differentGroups++;
			}
		}
		for(int i = 0; i < differentGroups; i++) {
			dontOverdo.push_back(0);
		}
		//n is the number of lines.
		//N is n - all the anomalies.
		N = n = (csi->numOfLines - 1);

		std::vector<int> vecRange;
		std::string str = dio->read();
		while(str != "done") {
			groupNum = 0;
			P++;
			std::stringstream ss(str);
			for (int k; ss >> k;) {
        		vecRange.push_back(k);    
        		if (ss.peek() == ',')
            		ss.ignore();
    		}

			from = vecRange[0];
			to = vecRange[1] + 1;
			N -= (to - from);
			int flag = 1;
			for(int i = 0; i < csi->vecAR.size(); i++) {
			if(i > 0) {
				if(result != 1 || csi->vecAR[i].description != csi->vecAR[i-1].description) {
					groupNum++;
				}
			}
				for(int j = from; j < to; j++){
					if(csi->vecAR[i].timeStep == j) {
						flag *= 0;
					}
				}
				if(flag == 0) {
					dontOverdo[groupNum] = 1;
					flag = 1;
				}
			}
			str = dio->read();
			vecRange.clear();
		}
		for(auto num : dontOverdo) {
			if(num == 1)
				TP++;
		}
		FP = differentGroups - TP;

		trueRate = TP / P;
		falseRate = FP / N;

		trueRate = floor(trueRate * 1000) / 1000;
		falseRate = floor(falseRate * 1000) / 1000;
		dio->write("True Positive Rate: ");
		dio->write(trueRate);
		dio->write("\n");

		dio->write("False Positive Rate: ");
		dio->write(falseRate);
		dio->write("\n");

	}
};

class Exit : public Command {
public:
    Exit(DefaultIO* dio) : Command(dio) {}
	void execute(){
		return;
	}
};

#endif /* COMMANDS_H_ */
