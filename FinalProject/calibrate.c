/*
 * calibrate.c
 *
 *  Created on: Nov 7, 2023
 *      Author: ethgru
 */
#include "IR.h"
#include "uart.h"
#include "movement.h"
#include "ping.h"
#include "button.h"
#include "pwm.h"

void calibrate_IR(oi_t *sensor){
    IR_init();
    ping_init();
    //Continuously output the Ping sensor and IR values
    char message[60];
    uart_sendStr("IR   PING");

    sprintf(message, "Ping\tIR");

    while(1){
        double pingVal = ping_receive();
        int irVal = IR_receive();
        uart_sendStr(message);
        sprintf(message, "%.3f\t%.3f\n", pingVal, irVal);
        uart_sendStr(message);
        lcd_printf(message);
        timer_waitMillis(500);
    }
}
void calibrate_driveDistance(oi_t *sensor){
    distance_coef = 1;
    drive(sensor, 100);
    lcd_printf("100/ActCM = distOff");
}

void calibrate_angle(oi_t *sensor){
    angle_coef = 1;
    turn(sensor, 90);
    lcd_printf("360/ActAng = angOff");
}
void calibrate_ping(){
    ping_coef = 1;
    char message[50];
    double pingVal;
    while(1){
        pingVal = ping_receive();
        sprintf(message, "%.2lf", pingVal);
        uart_sendStr(message);
        lcd_printf("%.2lf",pingVal);
    }
}
void calibrate_everything(oi_t *sensor){
    pwm_init();
    move_init();
    button_init();
    ping_init();
    lcd_printf("Press buttons to move to next steps");
    while(button_getButton() == 0){

    }
    timer_waitMillis(1000);
    lcd_printf("PWM Calibration");
    while(button_getButton() == 0){

    }
    timer_waitMillis(1000);
    pwm_config();
    pwm_setAngle(90);
    timer_waitMillis(1000);
    while(button_getButton() == 0){

    }
    timer_waitMillis(1000);

    lcd_printf("Calibrate drive\nDriving 1m\nMeasure Actual");
    while(button_getButton() == 0){

    }
    timer_waitMillis(1000);
    calibrate_driveDistance(sensor);
    while(button_getButton() == 0){

    }
    timer_waitMillis(1000);
    lcd_printf("Calibrate angle\nTurning 360\nMeasureActual");
    while(button_getButton() == 0){

    }
    timer_waitMillis(1000);
    calibrate_angle(sensor);
    while(button_getButton() == 0){

    }
    timer_waitMillis(1000);
    lcd_printf("Calibration Complete");
}

