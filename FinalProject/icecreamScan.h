/*
 * icecreamScan.h
 *
 *  Created on: Nov 7, 2023
 *      Author: ethgru
 */

#ifndef ICECREAMSCAN_H_
#define ICECREAMSCAN_H_

typedef struct objectStruct{

    //distance to the object
    double distance;

    //centered angle of the location from the cyBot
    int angle;

    //calculated width of the object
    double width;

    //1 if this truly is an object found
    int isObject;
}obj_t;

typedef struct drivingStruct{
    int exitCode;
    int distanceTraveled;
}drive_t;

//Initializes the ping, ir, and pwm
void icecreamScan_init();
/*
 * Updates an array of objects with the objects and its information found from within the scan boundaries
 */
void icecreamScan_scan(obj_t objects[], int startAngle, int endAngle);

/*
 * Drives and continously scans items to the right until a tall skinny object is detected , a bumper is hit, a hole is detected, or distance is reached.
 * exitCode is 0 if drive was completed, 1 if bump right, 2 if bump left, 3 if hole detected, 4 if customer was found
 * the input unfinished is if the previous scan ended when an object was in the middle of being scanned and it current measured width is given. 0 if no object was unifinished
 */
drive_t icecreamScan_drive(oi_t *sensor, int centimeters, int unfinishedWidth);

#endif /* ICECREAMSCAN_H_ */
