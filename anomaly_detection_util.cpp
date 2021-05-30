#include <iostream>
#include "anomaly_detection_util.h"
#include <math.h>

float exp(float* x, int size) {
    float sum = 0;
    for(int i = 0; i < size; i++) {
        sum += x[i];
    }
    return (sum * (1.0 / size));
}

float var(float* x, int size) {
    float p = 1.0 / size;
    float sum = 0;
    for(int i = 0; i < size; i++) {
        sum += pow(x[i] , 2);
    }
    return (p * sum) - pow(exp(x, size), 2);
}

float cov(float* x, float* y, int size) {
    float sum = 0;
    float eX, eY, eXY;
    float z[size];
    eX = exp(x, size);
    eY = exp(y, size);
    for(int i = 0; i < size; i++) {
        z[i]= y[i] * x[i];
    }
    eXY = exp(z, size);
    return eXY - (eX * eY);
}

float pearson(float* x, float* y, int size) {
    float covv = cov(x, y, size);
    float sX = sqrt(var(x, size));
    float sY = sqrt(var(y, size));
    
    return covv / (sX * sY);
}

Line linear_reg(Point** points, int size) {
    float x[size];
    float y[size];
   
    for(int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x,y,size) / var(x, size);
    float b1 = exp(y, size);
    float b2 = a * exp(x, size);
    float b = b1 - b2;
    if(fabs(b) < pow(10,-5)) {
       b = 0;  
    }
    return Line(a,b);
}

float dev(Point p,Point** points, int size) {
    return dev(p, linear_reg(points, size));
}
    
float dev(Point p,Line l) {
    return fabs(p.y - l.f(p.x));
}