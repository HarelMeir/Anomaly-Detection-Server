

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	float minRadius;
	float centerX;
	float centerY;
	int flag;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> cf;
	float threshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	virtual std::pair<float, Point> updateCircle(vector<Point*> PointVec)=0;
	virtual float checkDistance(Point x, Point y)=0;
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	std::pair<float, std::string> mostCorelative(std::string s, std::vector<std::string> vec, const TimeSeries& ts);
	void setThreshold(float num) {
		this->threshold = num;
	}
	float getThreshold() {
		return threshold;
	}
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */

