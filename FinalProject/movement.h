/*
 * movement.h
 *
 *  Created on: Nov 7, 2023
 *      Author: ethgru
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "open_interface.h"

//constants that represented the error proportions
double distance_coef;
double angle_coef;

typedef struct drivingStruct{
    int exitCode;
    int distanceTraveled;
}drive_t;

void move_init();

/*
 * Will move a certain distance forwards (+) or backwards (-) in centimeters with using an error coefficient value
 * program will return immediately if an obstacle was detected
 * Safe drive is true if you want it to detect obstacles and give corresponding error codes
 *
 * @return
 * distanceTraveled is the distance traveled
 * exitCode is 0 if drive was completed, 1 if bump right, 2 if bump left, 3 if hole detected
 *
 * Can use OI functions to constantly detect if bumpRight bumpLeft and hole
 *
 */
drive_t drive(oi_t *sensor, int centimeters, int safeDrive);

/*
 * Will turn the cyBot a certain degree with using an error coefficient value
 * Clockwise is a positive value, Counterclockwise is a negative value
 */
void turn(oi_t *sensor, int degrees);

#endif /* MOVEMENT_H_ */
