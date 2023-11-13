/*
 * calibrate.c
 *
 *  Created on: Nov 7, 2023
 *      Author: ethgru
 */
#include "IR.h"

void calibrate_IR(oi_t *sensor){
    IR_init();
    //Continuously output the Ping sensor and IR values
    char message[60];

    //move 2 meters and output values
    int i;
    double sumIR = 0;
    double sumIR2 = 0;
    double sumCm = 0;
    double sumIRCm = 0;
    uart_sendStr("IR   PING");

    //output and store all values
    int count = 50;
    for(i = 0; i < count; i++){
        double pingVal = ping_receive();
        int irVal = IR_receive();
        sumIR += log(irVal);
        sumIR2 += log(irVal) * log(irVal);
        sumCm += log(pingVal);
        sumIRCm += log(irVal) * log(pingVal);

        sprintf(message, "%d %.2f\n\r", irVal, pingVal);
        uart_sendStr(message);
        move_backward(sensor, 1);
        timer_waitMillis(500);
    }
    double ex = (count * sumIRCm - sumIR * sumCm) / (count * sumIR2 - sumIR * sumIR);
    double coff = exp((sumIR2 * sumCm - sumIRCm * sumIR) / (count * sumIR2 - sumIR * sumIR));
    sprintf(message, "Exponent value: %.3f\nCoefficient Value: %.3f\n", ex, coff);
    uart_sendStr(message);
    lcd_printf(message);
}

void calibrate_pwm(){
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