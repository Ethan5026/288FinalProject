/*
*
*   uart.h
*
*   Used to set up the UART
*   uses UART1 at 115200
*
*
*   @author Dane Larson
*   @date 07/18/2016
*   Phillip Jones updated 9/2019, removed WiFi.h
*/

#ifndef UART_H_
#define UART_H_

#include "Timer.h"
#include <inc/tm4c123gh6pm.h>

//is 1 when data has been received (set to 0 for detection purposes in main)
volatile unsigned char uart_flag;

//The data last received by uart
volatile unsigned char uart_data;

//initializes the registers to implement a UART connection
void uart_init(void);

//sends a single character to the uart connection
void uart_sendChar(char data);

//wait and receive the character inputted from uart connection. CANNOT BE USED WITH INTERRUPTS
char uart_receive(void);

//sends a string to the uart connection
void uart_sendStr(const char *data);

int uart_hasNextByte(void);

//initialize and sets up uart handler
void uart_handlerInit();

#endif /* UART_H_ */
