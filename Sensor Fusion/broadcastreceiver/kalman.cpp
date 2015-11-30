/*
 Copyright (C) 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.
 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").
 Contact information
 -------------------
 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
*/


#include "kalman.h"

    Kalman::Kalman()
    {
        /* We will set the variables like so, these can also be tuned by the user */
        Q_angle   = 0.001f;
        Q_bias    = 0.003f;
        R_measure = 0.03f;

        angle = 0.0f; // Reset the angle
        bias  = 0.0f; // Reset bias

        P[0][0] = 0.0f; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
        P[0][1] = 0.0f;
        P[1][0] = 0.0f;
        P[1][1] = 0.0f;
    }

    /*
    * The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
    */
    float Kalman::getAngle(float newAngle, float newRate, float dt)
    {
        // KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
        // Modified by Kristian Lauszus
        // See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

        // Discrete Kalman filter time update equations - Time Update ("Predict")
        // Update xhat - Project the state ahead
        /* Step 1 */
        rate = newRate - bias;
        angle += dt * rate;

        // Update estimation error covariance - Project the error covariance ahead
        /* Step 2 */
        P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
        P[0][1] -= dt * P[1][1];
        P[1][0] -= dt * P[1][1];
        P[1][1] += Q_bias * dt;

        // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
        // Calculate Kalman gain - Compute the Kalman gain
        /* Step 4 */
        S = P[0][0] + R_measure;
        /* Step 5 */
        K[0] = P[0][0] / S;
        K[1] = P[1][0] / S;

        // Calculate angle and bias - Update estimate with measurement zk (newAngle)
        /* Step 3 */
        y = newAngle - angle;
        /* Step 6 */
        angle += K[0] * y;
        bias += K[1] * y;

        // Calculate estimation error covariance - Update the error covariance
        /* Step 7 */
        P[0][0] -= K[0] * P[0][0];
        P[0][1] -= K[0] * P[0][1];
        P[1][0] -= K[1] * P[0][0];
        P[1][1] -= K[1] * P[0][1];

        return (float)angle;
    }

    /*
    * set start angle
    * Used to set angle, this should be set as the starting angle
    */
    void Kalman::setAngle(float newAngle)
    {
      angle = newAngle;
    }

    /*
    * Return the unbiased rate
    */
    float Kalman::getRate()
    {
      return rate;
    }

    /*
    * These are used to tune the Kalman filter
    */
    void Kalman::setQangle(float newQ_angle)
    {
      Q_angle = newQ_angle;
    }

    /*
    * set q bias to kalman filter
    */
    void Kalman::setQbias(float newQ_bias)
    {
      Q_bias = newQ_bias;
    }

    /*
    * set r mesure to kalman filter
    */
    void Kalman::setRmeasure(float newR_measure)
    {
      R_measure = newR_measure;
    }

    /*
    * get q angle
    */
    float Kalman::getQangle()
    {
      return Q_angle;
    }

    /*
    * get q bias from kalman filter internal
    */
    float Kalman::getQbias()
    {
      return Q_bias;
    }

    /*
    * get r mesure from kalman filter
    */
    float Kalman::getRmeasure()
    {
      return R_measure;
    }