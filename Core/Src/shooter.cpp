/*
 * shooter.cpp
 *
 *  Created on: Sep 14, 2023
 *      Author: rabbi
 */

/************** F446RE **************/

#include "shooter.h"

double target_length = 0.0; //target
double actual_length = 0.0; //position
int seat_mode = 1;   		//loaded -> 1	reload -> 2
int check_hook = 0;
int check_fire = 0;
int delay = 0;
int return_value = 0;
int buffer = 0;

int16_t enc_st = 0;
double angle_st = 0.0;

int trigger = 0;
int st_set = 0;

int run = 1;
int run_once = 1;

void shooter(){

	//encoder -> angle
	enc_st = __HAL_TIM_GetCounter(&htim1);
	angle_st += 360*((double)enc_st/(4*resolution_st*ratio_st));
	__HAL_TIM_SetCounter(&htim1, 0);



	//transfer turns into lengths
	actual_length = (angle_st/360)*(2*M_PI*radius);
//	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,600+10*angle);
//	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,600+10*angle);

	//If got a new target
	if(target_length > 0){
		if(check_hook == 0){
			//__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,600+10*hold); //Hook
			if(run_once == 1){
				UART_Send_SetMotorPosition(1, 1310, 300); //Hook -> 1310
				run_once = 0;
			}
			check_hook = 1;
		}
		if((target_length - actual_length) > 0){
			if(delay <= 300) delay++;
			else if(delay > 300){
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET); //Down
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
			}
		}
//		else if(target_length - actual_length < -2){
//			if(delay <= 300) delay++;
//			else if(delay > 300){
//				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET); //Up
//				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
//			}
//		}
		else{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET); //Stop
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
			st_set = 1;
			//Comfirm that the base is in the position or not
			if(ev_ok == 222 && hz_ok == 222 && trigger == 1){
				if(check_fire == 0){
					delay++;
					if(delay > 600){
						trigger = 1;
						//__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,600+10*fire); //Fire
						if(run_once == 0){
							UART_Send_SetMotorPosition(1, 1550, 300); //Fire -> 1525
							run_once = 1;
						}
						check_fire = 1;
					}
				}
			//Reset
				if(check_fire == 1){

					if(delay > 600 && delay <= 1000)	delay++;
					else if(delay > 1000){
						check_hook = 0;
						target_length = 0.0;
						actual_length = 0.0;
						angle_st = 0.0;
						trigger = 0;
						ros_st = 0;
						st_set = 0;
						UART_Send_SetMotorPosition(1, 1400, 300); //Fire -> waiting
						if(run == 1 && return_value < 4)	step = 1;
						else step = 0;
						if(return_value < 4 )	return_value ++;
					}
				}
			}
		}
	}
}



