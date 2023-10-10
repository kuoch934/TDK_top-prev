/*
 * shooter.h
 *
 *  Created on: Sep 14, 2023
 *      Author: rabbi
 */

/************** F446RE **************/

#ifndef INC_SHOOTER_H_
#define INC_SHOOTER_H_

#include "TIM_IT.h"
#include "shooter_base.h"
#include "servo.h"
#include <math.h>

#define radius 10.0 //(mm)
#define loaded 80
#define reload	170
#define fire 1010
#define hook 800

#define resolution_st 1000
#define ratio_st 1
#define span_st 0.001 //(s)

extern double target_length;
extern double actual_length;
extern double angle_seat;
extern int seat_mode;
extern int check_hook;
extern int check_fire;
extern int check;
extern int delay;
extern int return_value;

extern int16_t enc_st;
extern double angle_st;

void shooter();

#endif /* INC_SHOOTER_H_ */
