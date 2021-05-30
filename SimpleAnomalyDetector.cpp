
#include "SimpleAnomalyDetector.h"
#include <math.h>
#include <string.h>


SimpleAnomalyDetector::SimpleAnomalyDetector() {
	threshold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

//measuring the max dev, and return it as the max threshold.
float thresholdHelper(std::vector<float> vec1, std::vector<float> vec2, Line l) {
	float threshold, maxDev = 0;
	for(int i = 0; i < vec1.size(); i++) {
		Point p(vec1[i], vec2[i]);
		threshold = dev(p, l);

		if(threshold > maxDev) {
			maxDev = threshold;
		}
	}
	return maxDev;
}





std::pair<float, std::string> SimpleAnomalyDetector::mostCorelative(const std::string s, std::vector<std::string> vec, const TimeSeries& ts) {
	//initilizing help- parameters.
	std::string maxStr;
	float cor, maxCore = 0;
	
	//time series we can work with.
	TimeSeries ts2 = ts;
	//convert the vector of "s" to c-type.
	std::vector<float> vec1 = ts2.getTable()[s];
	float* str = vec1.data();

	//the size of the float arrays.
	int size = ts2.getTable()[s].size();


	for(int i = 0; i < vec.size(); i++) {
		if(s == vec[i]) {
			continue;
		}

		std::vector<float> vec2 = ts2.getTable()[vec[i]];
		float* str2 = vec2.data();

		cor = pearson(str, str2, size);
		if(cor > maxCore) {
			maxCore = cor;
			maxStr = vec[i];		
		}
		
	}
	return make_pair(maxCore, maxStr);
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){

	//creating a struct instance.
	struct correlatedFeatures cfs;

	//creating ts we can use.
	TimeSeries ts2 = ts;

	//getting the col names into a vector.
	std::vector<std::string> vec= ts2.getColNames();

	//the size of the vectors inside the table.
	int size = ts2.getTable()[vec[0]].size();
	std::vector<Point*> pointsVec;
	std::vector<std::string> dontOverdo;

	int overdoFlag = 0;
	//going over all the columns and checking who is most coralative with them.
	for(std::string s: vec) {
		for(int i = 0; i < dontOverdo.size(); i++) {
			if(s == dontOverdo[i]) {
				overdoFlag = 1;
			}
		}
		//inisilzing a pair of the most corelative column with s and their corrlation.
		std::pair<float, std::string> pair = mostCorelative(s, vec, ts);
		if(overdoFlag == 1) {
			overdoFlag = 0;
			continue;
		}
		overdoFlag = 0;
		
		//if the corlation of the columns are above the threshold add them to the vector as correlative.
		//threshold = 0.9
		if(abs(pair.first) >= threshold) {

			for(int i = 0; i < size ; i++) {
				Point* p = new Point(ts2.getTable()[s][i], ts2.getTable()[pair.second][i]);
				pointsVec.push_back(p);		
			}
			Line l;
			l = linear_reg(pointsVec.data(), size);

			cfs.feature1 = s;
			//feature2 = second.
			cfs.feature2 = pair.second;
			//corrlation = first.
			cfs.corrlation = pair.first;
			cfs.lin_reg = l;
			cfs.threshold = thresholdHelper(ts2.getTable()[s], ts2.getTable()[pair.second], l);
			cfs.flag = 0;
			cf.push_back(cfs);
			dontOverdo.push_back(pair.second);
			pointsVec.clear();
		}
		else if(abs(pair.first) >= 0.5) {
			cfs.feature1 = s;
			cfs.feature2 = pair.second;
			cfs.corrlation = pair.first;
			cfs.threshold = 0.5;
			for(int i = 0; i < size ; i++) {
				Point* p = new Point(ts2.getTable()[s][i], ts2.getTable()[pair.second][i]);
				pointsVec.push_back(p);		
			}
			std::pair<float, Point> minCircle = updateCircle(pointsVec);
			cfs.minRadius = minCircle.first * 1.1;
			cfs.centerX = minCircle.second.x;
			cfs.centerY = minCircle.second.y;
			cfs.flag = 1;
			cf.push_back(cfs);
			dontOverdo.push_back(pair.second);
		}
		
	}
}

/**
 * We will use learnormal for learning.
 * after that, we will go over the map of the test flight 1 line by 1.
 * for each line we will make points of the corelative couples, and will check if their dev from the lin_reg of the learnormal
 * if its bigger then the max allowed threshold.
 * If it is bigger, we will add this to the vector of the anomaly reports.
 * 
 */
std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
	//timeseries we can use.
	TimeSeries ts2 = ts;
	
	//getting the number of lines.
	std::vector<correlatedFeatures> corVec = getNormalModel();
	long size = ts2.getTable()[ts2.getColNames()[0]].size();

	//initlizing our Anomalyreports vector.
	std::vector<AnomalyReport> arVec;

	//float type variable for saving the dev of the current point from the reg line.
	float testPointDev;
	float dist;

;
	//going over all the line one by one, and checking for anomaly-situations for the colreative values.
	for(long i = 0; i < cf.size(); i++) {
		for(long j = 0; j < size; j++) {

			//creating a point
			Point p(ts2.getTable()[cf[i].feature1][j], ts2.getTable()[cf[i].feature2][j]);
				
			if(cf[i].flag == 0) {
				//checking its distance from the reg line.
				testPointDev = dev(p, cf[i].lin_reg);
				
				//if its bigger then allowed - sending report about it.
				if(testPointDev > (1.18 * cf[i].threshold)) {
					std::string s = cf[i].feature1 + "-" + cf[i].feature2;
					arVec.push_back(AnomalyReport(s, j+1));
				}
			}
			else {
				Point cen(cf[i].centerX, cf[i].centerY);
				dist = checkDistance(p, cen);

				if(dist > cf[i].minRadius) {
					std::string s = cf[i].feature1 + "-" + cf[i].feature2;
					arVec.push_back(AnomalyReport(s, j+1));
				}
			}

		}
	}	
	return arVec;
}

