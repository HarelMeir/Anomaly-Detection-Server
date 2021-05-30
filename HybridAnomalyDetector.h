

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
	std::pair<float, Point> updateCircle(vector<Point*> PointVec);
	float checkDistance(Point x, Point y);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
