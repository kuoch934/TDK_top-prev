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
double target_ev=0.0; //SP
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

//double P_hz = 0.25;
//double I_hz = 0.003;
//double D_hz = 0.004;

//double P_ev = 0.3;
//double I_ev = 0.0;
//double D_ev = 0.007;

void shooter_base(){

//horizontal angle
	enc_hz = __HAL_TIM_GetCounter(&htim2);
	angle_hz += 360*((double)enc_hz/(4*resolution_hz*ratio_hz))*2;	//PV
	__HAL_TIM_SetCounter(&htim2, 0);

	//et
	et_hz = target_hz - angle_hz;
	sigma_et_hz += et_hz;

	//ut -> pulse
	ut_hz = P_hz*et_hz + I_hz*sigma_et_hz*span_hz + D_hz*(et_hz-last_et_hz)/span_hz;	//PID Control
	if(ut_hz > 0.2) ut_hz = 0.2;
	else if(ut_hz < -0.2) ut_hz = -0.2;

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

	//record et
	last_et_hz = et_hz;

//elevation angle
	enc_ev = __HAL_TIM_GetCounter(&htim3);
	angle_ev += 360*((double)enc_ev/(4*resolution_ev*ratio_ev))*2;	//PV
	__HAL_TIM_SetCounter(&htim3, 0);

	//et
	et_ev = target_ev - angle_ev;
	sigma_et_ev += et_ev;

	//ut -> pulse
	ut_ev = P_ev*et_ev + I_ev*sigma_et_ev*span_ev + D_ev*(et_ev-last_et_ev)/span_ev;	//PID Control
	if(ut_ev > 0.3) ut_ev = 0.3;
	else if(ut_ev < -0.3) ut_ev = -0.3;

	if(ut_ev>0){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	else if(ut_ev<0){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
	}
	else if(ut_ev==0){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	__HAL_TIM_SET_COMPARE(&htim12,TIM_CHANNEL_1,fabs(ut_ev)*20000);

	//record et
	last_et_ev = et_ev;
}

int reset = 1;
int hz_origin = 0;	//zero
int ev_origin = 0;	//vertical
double hz_pos_limit = 60;
double hz_neg_limit = -60;
double ev_limit = 90;

void Reset(){
	hz_origin = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
	ev_origin = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);

	if(ev_origin != 1){
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
	}
	else{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);
		angle_ev = 0;
	}

	if(hz_origin != 1){
		if(angle_hz > 0){
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
		}
		else if(angle_hz < 0){
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
		}
		else if(angle_hz == hz_pos_limit){
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
		}
		else if(angle_hz == hz_neg_limit){
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);
		}
	}
	else{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
		angle_hz = 0;
	}

	if(hz_origin == 1 && ev_origin == 1)	reset = 0;
}


void base_limit(){
	//elevation angle limit
	if(target_ev <= 0)	target_ev = 0;
	else if (target_ev >= ev_limit)	target_ev = ev_limit;
	//horizontal angle limit
	if(target_hz >= hz_pos_limit)	target_hz = hz_pos_limit;
	else if(target_hz <= hz_neg_limit)	target_hz = hz_neg_limit;
}
