#include <pwm.h>
#include "button.h"
#include "lcd.h"
/*
 * pwm.c
 *
 *  Created on: Oct 31, 2023
 *      Author: ethgru
 */

int left_calibration = 282517;
int right_calibration = 310876;
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
}

void pwm_setMATCH(int input){
    TIMER1_TBMATCHR_R = input & 0xFFFF;
    TIMER1_TBPMR_R = (input >> 16) & 0xFF;
}

void pwm_config(){
    lcd_init();
    button_init();
    int matchValue = 298238;
    lcd_printf("Press Buttons 1 and 2 to\nposition at 0 degrees\nPress 4 to enter");
    while(button_getButton() != 4){
        if(button_getButton() == 2){
            matchValue++;
        }
        else if(button_getButton() == 1){
            matchValue--;
        }
        pwm_setMATCH(matchValue);
        timer_waitMicros(200);

    }
    while(button_getButton() == 4){}
    timer_waitMillis(20);
    rightCalibration = matchValue;

    lcd_printf("Press Buttons 1 and 2 to\nposition at 180 degrees\nPress 4 to enter");
    while(button_getButton() != 4){
        if(button_getButton() == 2){
            matchValue++;
        }
        else if(button_getButton() == 1){
            matchValue--;
        }
        pwm_setMATCH(matchValue);
        timer_waitMicros(200);

    }
    leftCalibration = matchValue;
    lcd_printf("Right: %d\nLeft: %d", rightCalibration, leftCalibration);
}

void pwm_setAngle(int degree){
    if(degree > 180 || degree < 0){
        return;
    }
    double eachDegree = (leftCalibration - rightCalibration) / 180.0;
    pwm_setMATCH(eachDegree * degree + rightCalibration);
}



