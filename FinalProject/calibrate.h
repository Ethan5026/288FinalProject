/*
 * calibrate.h
 *
 *  Created on: Nov 7, 2023
 *      Author: ethgru
 */

#ifndef CALIBRATE_H_
#define CALIBRATE_H_

//Calibrate the distance driven and give the distance error coefficient
//ex. if the Cybot is told to move 100cm and moves 80 cm then the error coefficient is 1.25
double calibrate_driveDistance(oi_t *sensor);

//Calibrate the angle driven and give the angle error coefficient
//ex. if the Cybot is told to turn 100 degrees and moves 80 degrees then the error coefficient is 1.25
double calibrate_driveDistance(oi_t *sensor);

//Calculate the coefficient that needs to be multiplied by the timer values to get an accurate distance
double calibrate_ping();


#endif /* CALIBRATE_H_ */
