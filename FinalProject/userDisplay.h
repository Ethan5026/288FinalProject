/*
 * userDisplay.h
 *
 *  Created on: Nov 7, 2023
 *      Author: ethgru
 */

#ifndef USERDISPLAY_H_
#define USERDISPLAY_H_

//Information of the ice cream option
typedef struct creamStruct{
    char creamName[10];
    int creamCount;
    double creamCost;
}cream_t;

//The array of ice cream options and information of each
cream_t creamOptions[4];

//Amount of money earned
double totalProfit;

//set the structs to represent 4 different ice cream options and initialize LCD and buttons
void userDisplay_init();

/*
 * Show item options and prompt user to select
 * Based on selected ice cream, update values, total profit
 * Ask if user will purchase more
 * Send purchase data to UART
 * NOTE: You can't buy ice cream that is out of stock
 */
void userDisplay_sell();

#endif /* USERDISPLAY_H_ */
