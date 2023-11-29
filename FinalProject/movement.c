#include "open_interface.h"
#include "movement.h"

void move_init(){
    distance_coef = 1;
    angle_coef = 1.22;
}

void turn(oi_t *sensor, int degrees) {
    double sum = 0;
    int direction_sign = (2 * (degrees > 0) - 1);
    oi_setWheels(-direction_sign * 100, direction_sign * 100);
    while(fabs(sum) < fabs(degrees * angle_coef)){
        oi_update(sensor);
        sum += sensor->angle;

    }
    oi_setWheels(0, 0);
}

drive_t drive(oi_t *sensor, int centimeters, int safeDrive){

    //total distance traveled
    double distanceTracker = 0;

    drive_t returnVal;

    //set the wheels to move slowly
    int direction_sign = (2 * (centimeters > 0) - 1);
    oi_setWheels(200 * direction_sign, 200 * direction_sign);

    //while the distance traveled is lower than distance given
    while(fabs(distanceTracker) < fabs(centimeters * 10 * distance_coef)){
        //update the distance values
        oi_update(sensor);
        distanceTracker += sensor->distance * direction_sign;

        if (!safeDrive) { continue; }
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
    }
    oi_setWheels(0,0);
    returnVal.exitCode = 0;
    returnVal.distanceTraveled = centimeters * 10;
    return returnVal;
}

