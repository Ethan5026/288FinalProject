/*
 * IR.c
 *
 *  Created on: Oct 30, 2023
 *      Author: ethgru
 */
#include "IR.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "uart.h"
#include "movement.h"
#include "open_interface.h"
#include "ping.h"
#include<limits.h>
#include<math.h>

volatile long double coefficient = 248063.4636;
volatile long double exponent = -1.3058990322;

void IR_init(){
    //GPIO
    SYSCTL_RCGCGPIO_R |= 0b00000010; //start the clock for port B
    GPIO_PORTB_AMSEL_R |= 0b00010000; //set pin4 to read analog
    GPIO_PORTB_DIR_R &= ~0b00010000; //set to input
    GPIO_PORTB_DEN_R &= ~0b00010000; //not digitally enabled
    GPIO_PORTB_AFSEL_R |= 0b00010000; //uses alternate device
    GPIO_PORTB_ADCCTL_R = 0; // sets pin 4 to trigger the pin to ACD

    //ADC
    SYSCTL_RCGCADC_R |= 0b1; // clock set for ADC0
    timer_waitMillis(100);
    ADC0_ACTSS_R &= ~0b1; // disable ss0


    ADC0_SSCTL0_R &= ~0xF;
    ADC0_SSCTL0_R |=  0x2; // set to use end
    ADC0_SSMUX0_R &= ~0xF;
    ADC0_SSMUX0_R |=  0b1010; // sets to analog function 10

    ADC0_SAC_R |=  0b100;
    ADC0_SAC_R &= ~0b011;// enable 16 scans for average


    ADC0_ACTSS_R |= 0b1; // enable ss0
    ADC0_PSSI_R |= 0b1; //initiate ss0


}
int IR_receive(){
    int i = 0;
    int ir_data = 0;
    for(i = 0; i < 4; i++){
        while(1){
            ADC0_PSSI_R |= 0b1; //initiate ss0
            if(ADC0_SSFSTAT0_R & 0x100){ // checks if its empty
               continue;
             }
             ir_data += ADC0_SSFIFO0_R & 0xFFF; //adds the data to ir_data removing reserved bits
             break;
        }
    }
    ir_data /= 4;
    return ir_data;
}


double IR_receiveCm(){
    int i = 0;
    int ir_data = 0;
    for(i = 0; i < 4; i++){
        while(1){
            ADC0_PSSI_R |= 0b1; //initiate ss0
            if(ADC0_SSFSTAT0_R & 0x100){ // checks if its empty
               continue;
             }
             ir_data += ADC0_SSFIFO0_R & 0xFFF; //adds the data to ir_data removing reserved bits
             break;
        }
    }
    ir_data /= 4;
    return coefficient * pow((double)ir_data, exponent);
}

