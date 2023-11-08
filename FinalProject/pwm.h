/*
 * pwm.h
 *
 *  Created on: Oct 31, 2023
 *      Author: ethgru
 */

#ifndef PWM_H_
#define PWM_H_

int rightCalibration;
int leftCalibration;

void pwm_init();
void pwm_setMATCH(int input);
void pwm_setAngle(int degrees);


#endif /* PWM_H_ */
