/*
 * movement.h
 *
 *  Created on: Nov 7, 2023
 *      Author: ethgru
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

//constants that represented the error proportions
double distance_coef;
double angle_coef;

typedef struct drivingStruct{
    int exitCode;
    int distanceTraveled;
}drive_t;

/*
 * Will move a certain distance forwards (+) or backwards (-) in centimeters with using an error coefficient value
 * program will return immediately if an obstacle was detected
 *
 * @return
 * distanceTraveled is the distance traveled
 * exitCode is 0 if drive was completed, 1 if bump right, 2 if bump left, 3 if hole detected
 *
 * Can use OI functions to constantly detect if bumpRight bumpLeft and hole
 */
drive_t drive(oi_t *sensor, int centimeters);

void move_init();
/*
 * Will turn the cyBot a certain degree with using an error coefficient value
 * Clockwise is a positive value, Counterclockwise is a negative value
 */
void turn(oi_t *sensor, int degrees);

void move_forward(oi_t *sensor, int centimeters);
void move_dynamic(oi_t *sensor, int centimeters);

void turn_clockwise(oi_t *sensor, int degrees);

void turn_counterclockwise(oi_t *sensor, int degrees);

void move_backward(oi_t *sensor, int centimeters);

void turn(oi_t *sensor, int degrees);

void move(oi_t *sensor, int centimeters);
#endif /* MOVEMENT_H_ */
