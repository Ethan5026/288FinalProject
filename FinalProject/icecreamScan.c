/*
 * icecreamScan.c
 *
 *  Created on: Nov 13, 2023
 *      Author: idenning
 */

#include "icecreamScan.h"
#include "movement.h"
#include "pwm.h"
#include "IR.h"
#include "ping.h"
#include "Timer.h"
#include "uart.h"
#include<limits.h>
#include<math.h>
#include "open_interface.h"

typedef struct scan_struct{
    double sound_dist;
    int IR_raw_val;
} Scan_t;


typedef struct partial_obj_struct {
    int start_angle;
    int end_angle;
    char isStarted;
    float width;
} partial_obj_t;

void capture(int angle, Scan_t *scanData);
obj_t completeObject(partial_obj_t obj);

void icecreamScan_init() {
    pwm_init();
    ping_init();
    IR_init();
}
double MINIMUM_EDGE_CHANGE = 4;
double MAXIMUM_CUSTOMER_DISTANCE = 35;
double MAX_OBJECT_WIDTH = 100;
double MIN_OBJECT_WIDTH = 50;


//Still needs to work with the IR calibrated values and dist_coef
drive_t icecreamScan_drive(oi_t *sensor, int centimeters, int unfinishedWidth){

    //will be removed after implementation
    double distance_coef = 1;

    // is 1 if an object is in the middle of a scan
    int objectStarted = (unfinishedWidth != 0);

    //total distance traveled
    double distanceTracker = 0;

    //previous IR value
    double irPrevious = 248063.4636 * pow((double)IR_receive(), -1.3058990322);

    //current IR value
    double irCurrent;

    //starting width of the object
    double objectStartDistance;

    //total width of the object
    double objectTotalDistance;

    drive_t returnVal;

    char message[20];

    //can't allow centimeters to be negative, illegal argument
    if(centimeters < 0){
        returnVal.exitCode = 5;
        returnVal.distanceTraveled = 0;
        return returnVal;
    }

    //set the angle to 0
    pwm_setAngle(0);

    //set the wheels to move slowly
    oi_setWheels(200, 200);

    //while the distance traveled is lower than distance given
    while(fabs(distanceTracker) < fabs(centimeters * 10 * distance_coef)){

        //get the current IR value and compare if object has start
        irCurrent = 248063.4636 * pow((double)IR_receive(), -1.3058990322);
        if((fabs(irCurrent - irPrevious) > MINIMUM_EDGE_CHANGE) && irCurrent < MAXIMUM_CUSTOMER_DISTANCE && objectStarted == 0){

            //start the object scanning
            objectStarted = 1;
            objectStartDistance = distanceTracker / distance_coef;
            objectTotalDistance = distanceTracker / distance_coef;
        }

        else if((fabs(irCurrent - irPrevious) < MINIMUM_EDGE_CHANGE) && objectStarted == 1){

            //update the object's scan data
            objectTotalDistance = distanceTracker / distance_coef;
        }

        else if((fabs(irCurrent - irPrevious) > MINIMUM_EDGE_CHANGE) && objectStarted == 1){

            //calculate final data

            //if width is around 5cm, which represents a skinny object, and is close to the curb then it stops
            if(((objectTotalDistance - objectStartDistance + unfinishedWidth) < MAX_OBJECT_WIDTH) &&((objectTotalDistance - objectStartDistance + unfinishedWidth) > MIN_OBJECT_WIDTH)){
                //customer found
                oi_setWheels(0,0);

                returnVal.exitCode = 4;
                returnVal.distanceTraveled = distanceTracker / distance_coef;

                return returnVal;
            }
            else{
                objectStarted = 0;
                objectStartDistance = 0;
                objectTotalDistance = 0;

            }
        }
        sprintf(message, "%.2lf,   %.2lf,  %d \n\r", irCurrent, distanceTracker / distance_coef, objectStarted);
        uart_sendStr(message);
        //check if anything is being hit or holes
        if(sensor->bumpLeft){
            oi_setWheels(0,0);
            returnVal.exitCode = 2;
            returnVal.distanceTraveled = distanceTracker / distance_coef;
            return returnVal;
        }
        else if(sensor->bumpRight){
            oi_setWheels(0,0);
            returnVal.exitCode = 1;
            returnVal.distanceTraveled = distanceTracker / distance_coef;
            return returnVal;
        }
        else if(sensor->cliffLeft || sensor->cliffRight){
            oi_setWheels(0,0);
                        returnVal.exitCode = 3;
                        returnVal.distanceTraveled = distanceTracker / distance_coef;
                        return returnVal;
        }

        //update the distance values
        oi_update(sensor);
        distanceTracker += sensor->distance;
        irPrevious = irCurrent;

    }
    oi_setWheels(0,0);
    returnVal.exitCode = 0;
    returnVal.distanceTraveled = centimeters * 10;
    return returnVal;
}

void icecreamScan_scan(obj_t objects[], int startAngle, int endAngle) {
    Scan_t scanData;
    int objIndex = 0;

    int MINIMUM_OBJECT_ANGLE = 6;
    int MINIMUM_OBJECT_EDGE_CHANGE = 15;
    int MAXIMUM_OBJECT_DISTANCE = 70;
    double IR_COEFF = 248063.4636;
    double IR_EXP = -1.3058990322;

    partial_obj_t cur_obj;
    cur_obj.isStarted = 0;

    int angle = 0;
    double last_distance = 0;
    for(angle = startAngle; angle <= endAngle; angle+=2) {
        pwm_setAngle(angle);
        timer_waitMillis(200);
        capture(angle, &scanData);
        double linearIR = IR_COEFF * pow((double)scanData.IR_raw_val, IR_EXP);

        if(fabs(last_distance - linearIR) > MINIMUM_OBJECT_EDGE_CHANGE) {
            if(cur_obj.isStarted) {
                if(angle - cur_obj.start_angle > MINIMUM_OBJECT_ANGLE) {
                    cur_obj.end_angle = angle;
                    objects[objIndex] = completeObject(cur_obj);
                    objIndex++;
                    cur_obj.isStarted = 0;
                }
                else {
                    cur_obj.isStarted = 0;
                }
            }
            if((linearIR < MAXIMUM_OBJECT_DISTANCE) && !(cur_obj.isStarted)) {
                cur_obj.isStarted = 1;
                cur_obj.start_angle = angle;
            }
        }
        last_distance = linearIR;
    }

    objects[objIndex].isObject = 0;
}

obj_t completeObject(partial_obj_t obj) {
    Scan_t scanData;
    obj_t result;
    result.angle = (obj.start_angle + obj.end_angle) / 2;
    pwm_setAngle(result.angle);
    timer_waitMillis(1000);
    capture(result.angle, &scanData);
    result.distance = scanData.sound_dist;
    result.width = 2 * result.distance * sin((obj.end_angle - obj.start_angle) * (M_PI / 180)/2);
    result.isObject = 1;

    return result;
}


void capture(int angle, Scan_t *scanData){
    pwm_setAngle(angle);
    scanData->IR_raw_val = IR_receive();
    scanData->sound_dist = ping_receive();
}
