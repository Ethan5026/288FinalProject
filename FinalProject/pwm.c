#include <pwm.h>
#include "button.h"
#include "lcd.h"
/*
 * pwm.c
 *
 *  Created on: Oct 31, 2023
 *      Author: ethgru
 */

void pwm_init(){
    SYSCTL_RCGCGPIO_R |= 0x2; // set clock for port b
    SYSCTL_RCGCTIMER_R |= 0x2; // enable timer1 clock

    GPIO_PORTB_AFSEL_R |= 0x20; //sets peripheral signal for pin 5
    GPIO_PORTB_PCTL_R &= ~0x000F00000; //sets pin 5 for device 7
    GPIO_PORTB_PCTL_R |=   0x00700000; //sets pin 5 for device 7
    GPIO_PORTB_DIR_R |= 0x20; // set as output
    GPIO_PORTB_DEN_R |= 0x20; // enable port


    TIMER1_CTL_R &= ~0x100; // disable the timer
    TIMER1_CFG_R = 0x4; // set to 16 bit mode
    TIMER1_TBILR_R = 0xE200; // set time turning
    TIMER1_TBPR_R = 0x4; // set the prescaler to 8
    TIMER1_TBPMR_R = 0x4; //set prescaler match
    TIMER1_TBMATCHR_R = 0xE200; //set the periodish
    TIMER1_TBMR_R |= 0xA; //periodic, pwm, count-down, match update
    TIMER1_CTL_R |= 0x100; // enable timer and pwm

    leftCalibration = 282517;
    rightCalibration = 310876;
}

void pwm_setMATCH(int input){
    TIMER1_TBMATCHR_R = input & 0xFFFF;
    TIMER1_TBPMR_R = (input >> 16) & 0xFF;
}

void pwm_setAngle(int degree){
    if(degree > 180 || degree < 0){
        return;
    }
    double eachDegree = (leftCalibration - rightCalibration) / 180.0;
    pwm_setMATCH(eachDegree * degree + rightCalibration);
}



