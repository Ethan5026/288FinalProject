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

//Initializes the ping, ir, and pwm
void icecreamScan_init();
/*
 * Updates an array of objects with the objects and its information found from within the scan boundaries
 */
void icecreamScan_scan(obj_t objects[], int startAngle, int endAngle);

#endif /* ICECREAMSCAN_H_ */
