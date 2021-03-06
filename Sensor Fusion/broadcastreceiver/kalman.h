#ifndef KALMAN_H
#define KALMAN_H

#include <QQuaternion>
#include <QList>
#include "math.h"
#include "filter.h"

class Kalman : public Filter
{
public:
    Kalman();
    void setAngle(float newAngle);
    float getRate();
    void setQangle(float newQ_angle);
    void setQbias(float newQ_bias);
    void setRmeasure(float newR_measure);
    float getQangle();
    float getQbias();
    float getRmeasure();
    float getAngle(float newAngle, float newRate, float dt);

    //new added functions
    void updateOrientation(float, float, float, float, float, float, float, float, float);
    QQuaternion getRotation();
    float radToDegree(float);
    QList<float> getAngleFromVectorDegree(float, float, float, float, float, float);
    QList<float> getAngles(QList<float>, QList<float>, float);
    void setAngles(QList<float>);
/*
* private internal value for kalman filter
*/
private:
    /* Kalman filter variables */
    float Q_angle; // Process noise variance for the accelerometer
    float Q_bias;  // Process noise variance for the gyro bias
    float R_measure; // Measurement noise variance - this is actually the variance of the measurement noise

    float angle; // The angle calculated by the Kalman filter - part of the 2x1 state vector
    float bias; // The gyro bias calculated by the Kalman filter - part of the 2x1 state vector
    float rate; // Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate

    float P[2][2]; // Error covariance matrix - This is a 2x2 matrix
    float K[2]; // Kalman gain - This is a 2x1 vector
    float y; // Angle difference
    float S; // Estimate error

    // added new attributes
    QQuaternion orientation;
    float ex, ey, ez;
    float angleX, angleY, angleZ, rateX, rateY, rateZ;
    float yX, yY, yZ;
};

#endif // KALMAN_H
