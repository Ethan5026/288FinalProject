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
