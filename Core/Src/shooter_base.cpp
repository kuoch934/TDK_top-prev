/*
 * shooter_base.cpp
 *
 *  Created on: Sep 14, 2023
 *      Author: rabbi
 */

/************** F446RE **************/

#include "shooter_base.h"

int16_t enc_hz = 0;
int16_t enc_ev = 0;

double target_hz = 0.0; //SP
double target_ev = 0.0; //SP

double angle_hz = 0.0;
double angle_ev = 0.0;

int hz_ok = 0;
int ev_ok = 0;

double et_hz = 0.0;
double et_ev = 0.0;
double sigma_et_hz = 0.0;
double sigma_et_ev = 0.0;
double ut_hz = 0.0;
double ut_ev = 0.0;
double last_et_hz = 0.0;
double last_et_ev = 0.0;

double speed_ev = 0.4;

double P_hz = 0.16;
double I_hz = 0.003;
double D_hz = 0.004;

double P_ev = 0.16;
double I_ev = 0.0005;
double D_ev = 0.005;

int once_hz = 1;
int once_ev = 1;

void shooter_base(){
//horizontal angle
	enc_hz = __HAL_TIM_GetCounter(&htim2);
	angle_hz += 360*((double)enc_hz/(4*resolution_hz*ratio_hz))*2;	//PV
	__HAL_TIM_SetCounter(&htim2, 0);

	//et
	et_hz = target_hz - angle_hz;
	sigma_et_hz += et_hz;

	//P_hz
	if(once_hz == 1){
		if(fabs(et_hz)<10) P_hz = 0.16;
		else P_hz = 0.2;
		once_hz = 0;
	}

	//ut -> pulse
	ut_hz = P_hz*et_hz + I_hz*sigma_et_hz*span_hz + D_hz*(et_hz-last_et_hz)/span_hz;	//PID Control
	if(ut_hz > 0.3) ut_hz = 0.3;
	else if(ut_hz < -0.3) ut_hz = -0.3;

	if(ut_hz>0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	}
	else if(ut_hz<0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	}
	else if(ut_hz==0){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
	}
	__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_2,fabs(ut_hz)*20000);

	//report set
	if(last_et_hz == et_hz && hz_ok < 222)	hz_ok++;
	else if(last_et_hz != et_hz) hz_ok = 0;

	//record et
	last_et_hz = et_hz;

//elevation angle
	enc_ev = __HAL_TIM_GetCounter(&htim3);
	angle_ev += 360*((double)enc_ev/(4*resolution_ev*ratio_ev))*2;	//PV
	__HAL_TIM_SetCounter(&htim3, 0);

	//et
	et_ev = target_ev - angle_ev;
	sigma_et_ev += et_ev;

	//P_ev
//	if(once_ev == 1){
//		if(fabs(et_ev)<=10) P_ev = 0.16;
//		else P_ev = 0.16;
//		once_ev = 0;
//	}

	//ut -> pulse
	ut_ev = P_ev*et_ev + I_ev*sigma_et_ev*span_ev + D_ev*(et_ev-last_et_ev)/span_ev;	//PID Control

	if(ut_ev<0){
		if(angle_ev < 45)	speed_ev = 0.08;
		else speed_ev = 0.1;
	}
	if(ut_ev>0){
		if(angle_ev < 20)	speed_ev = 0.8;
		else speed_ev = 1.0;
	}


	if(ut_ev > speed_ev) ut_ev = speed_ev;
	else if(ut_ev < -speed_ev) ut_ev = -speed_ev;

	if(ut_ev>0){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
	}
	else if(ut_ev<0){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	else if(ut_ev==0){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_1,fabs(ut_ev)*20000);

	//report set
	if(last_et_ev == et_ev  && ev_ok < 222) ev_ok++;
	else if(last_et_ev != et_ev) ev_ok = 0;

	//record et
	last_et_ev = et_ev;
}

int reset = 1;
int hz_origin = 0;	//zero
int ev_origin = 0;	//vertical
double hz_pos_limit = 45.0;
double hz_neg_limit = -45.0;
double ev_limit = 90;
int buffer_time = 0;
int num = 0;
int first = 1;
int reset_hz = 0.0;
int reset_ev = 0.0;

void Reset(){
	//horizontal angle
	enc_hz = __HAL_TIM_GetCounter(&htim2);
	angle_hz += 360*((double)enc_hz/(4*resolution_hz*ratio_hz))*2;	//PV
	__HAL_TIM_SetCounter(&htim2, 0);

	if(hz_origin == 1){
		if(angle_hz > 0 && first == 0){
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_2,0.4*20000);
		}
		else if(angle_hz < 0 && first == 0){
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_2,0.4*20000);
		}
		else if(first == 1){
//			reset_ev = 10;
//			reset_ev = 5;
			num++;
			if(num < 5000){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
				__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_2,0.4*20000);
			}
			else if(num < 10000 && num >= 5000){
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
				__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_2,0.4*20000);
			}
			else{
				num = 0;
			}
		}
	}
	else{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_2,0);
		angle_hz = 0;
	}

	//elevation angle
	enc_ev = __HAL_TIM_GetCounter(&htim3);
	angle_ev += 360*((double)enc_ev/(4*resolution_ev*ratio_ev))*2;	//PV
	__HAL_TIM_SetCounter(&htim3, 0);

	if(hz_origin == 0){
		if(ev_origin == 0){
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_1,0.15*20000);

		}
		else{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_1,0);
			angle_ev = 0;
		}
	}


	if(hz_origin == 0 && ev_origin == 1){
//		reset_hz = 0;
//		reset_ev = 0;
		target_hz = 0;
		target_ev = 0;
		//buffer
		buffer_time++;
		if(buffer_time >= 222){
			buffer_time = 0;
			first = 0;
			reset = 0;
		}
	}
}


void base_limit(){
	//elevation angle limit
	if(target_ev <= 0)	target_ev = 0;
	else if (target_ev >= ev_limit)	target_ev = ev_limit;
	//horizontal angle limit
	if(target_hz >= hz_pos_limit)	target_hz = hz_pos_limit;
	else if(target_hz <= hz_neg_limit)	target_hz = hz_neg_limit;
}

int step = -1;

double ros_st = 0.0;
double ros_hz = 0.0;
double ros_ev = 0.0;
int time = 0;

void Script(){
	switch(step){
		case 1:
			time++;
			if(time>50){
				target_length = 90;
				time = 0;
				step = 2;
			}
			break;
		case 2:
			Reset();
			if(hz_origin == 0 && ev_origin == 1)	step = 3;
			break;
		case 3:
			target_ev = 80;
			if(st_set == 1 && ev_ok == 222)	step = 4;
			break;
		case 4:
			seat_mode = 2;
			time++;
			if(time>200){
				time = 0;
				step = 5;
			}
			break;
		case 5:
			//ball drop
			time++;

			if(time>1000){
				time = 0;
				step = 6;
			}
			break;
		case 6:
			target_ev = 75;
			if(ev_ok == 222)	step = 7;
			break;
		case 7:
			seat_mode = 1;
			time++;
			if(time>200){
				time = 0;
				step = 8;
			}
			break;
		case 8:
			Reset();
			if(hz_origin == 0 && ev_origin == 1)	step = 0;
			break;
		case 0:
			target_length = ros_st;
			target_hz = ros_hz;
			target_ev = ros_ev;
			break;
		case -1:
			if(first == 0 && flag == 8)	step = 1;
	}
}
