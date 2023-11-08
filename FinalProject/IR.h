/*
 * IR.h
 *
 *  Created on: Oct 30, 2023
 *      Author: ethgru
 */

#ifndef IR_H_
#define IR_H_

#include "open_interface.h"

void IR_init();
int IR_receive();
void IR_calibrate(oi_t *sensor);
double IR_receiveCm();



#endif /* IR_H_ */
