
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


std::pair<float,Point> HybridAnomalyDetector::updateCircle(std::vector<Point*> PointVec){
	Circle c = findMinCircle(PointVec.data(), PointVec.size());
	return 	std::make_pair(c.radius, c.center);
}


float HybridAnomalyDetector::checkDistance(Point point, Point cent) {
	 return dist(point, cent);
}
