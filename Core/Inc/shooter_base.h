/*
 * shooter_base.h
 *
 *  Created on: Sep 14, 2023
 *      Author: rabbi
 */

/************** F446RE **************/

#ifndef INC_SHOOTER_BASE_H_
#define INC_SHOOTER_BASE_H_

#include "TIM_IT.h"
#include "shooter.h"
#include "ztest/ros_port.h"
#include <math.h>
extern int flag;

extern int st_set;

//horizontal angle
#define resolution_hz 1000
#define ratio_hz 6
#define span_hz 0.001 //(s)
//#define P_hz 0.16
//#define I_hz 0.003
//#define D_hz 0.004
//elevation angle
#define resolution_ev 1000
#define ratio_ev 1
#define span_ev 0.001 //(s)
//#define P_ev 0.16
//#define I_ev 0.0
//#define D_ev 0.007

//elevation angle
extern double target_ev;
extern int ev_ok;
//horizontal angle
extern double target_hz;
extern int hz_ok;
//reset
extern int reset;	//execute reset?
extern int hz_origin;	//zero
extern int ev_origin;	//vertical

//horizontal angle
extern int16_t enc_hz;
extern double angle_hz;
//elevation angle
extern int16_t enc_ev;
extern double angle_ev;

//PID
extern double et_hz;
extern double et_ev;
extern double sigma_et_hz;
extern double sigma_et_ev;
extern double ut_hz;
extern double ut_ev;

extern double speed_ev;

//extern double P_hz;
//extern double I_hz;
//extern double D_hz;
//
//extern double P_ev;
//extern double I_ev;
//extern double D_ev;

//limit
extern int ev_origin;
extern int hz_origin;
extern double ev_limit;
extern double hz_pos_limit;
extern double hz_neg_limit;
extern int buffer_time;
extern int num;
extern int first;
extern int reset_hz;
extern int reset_ev;

void shooter_base();
void Reset();
void base_limit();

//script
extern int step;

extern double ros_st;
extern double ros_hz;
extern double ros_ev;

extern double mission;

void Script();

#define go 90
#define stop 0

#endif /* INC_SHOOTER_BASE_H_ */
