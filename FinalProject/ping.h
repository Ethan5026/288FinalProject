/*
 * ping.h
 *
 *  Created on: Oct 30, 2023
 *      Author: ethgru
 */

#ifndef PING_H_
#define PING_H_

void ping_init();
double ping_receive();
void pulse();
void TIMER3B_Handler();
volatile unsigned int rising_time;
volatile unsigned int falling_time;
volatile unsigned int overflow;

//the coefficient modified after calibrate to determine the distance in cm
double ping_coef;


#endif /* PING_H_ */
