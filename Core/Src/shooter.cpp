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
double angle_seat = 180.0;
int seat_mode = 2;   		//badminton -> 1	baseball -> 2	badminton reload -> 3	baseball reload -> 4
int seat_ok = 0;
int check_hook = 0;
int check_fire = 0;
int delay = 0;
int return_value = 0;

int16_t enc_st = 0;
double angle_st = 0.0;

void shooter(){

	//encoder -> angle
	enc_st = __HAL_TIM_GetCounter(&htim1);
	angle_st += 360*((double)enc_st/(4*resolution_st*ratio_st));
	__HAL_TIM_SetCounter(&htim1, 0);



	//transfer turns into lengths
	actual_length = (angle_st/360)*(2*M_PI*radius);

	//If got a new target
	if(target_length > 0){

		//ball seat set!
		switch(seat_mode){
			case 1:
				if(angle_seat > badminton){
					angle_seat--;
					__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,600+10*angle_seat/ratio_seat);
				}
				else if(angle_seat == badminton)	seat_ok = 1;
			case 2:
				if(angle_seat < baseball){
					angle_seat++;
					__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,600+10*angle_seat/ratio_seat);
				}
				else if(angle_seat == baseball)	seat_ok = 1;
			case 3:
				__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,600+10*reload/ratio_seat);
				angle_seat = reload;
				seat_ok = 1;
			case 4:
				__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,600+10*reload/ratio_seat);
				angle_seat = reload;
				seat_ok = 1;
		}

		if(target_length > actual_length){
			if(check_hook == 0){
				delay = 0;
				__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,600+10*hold); //Hook
				check_hook = 1;
			}
			check_fire = 0;
			if(delay <= 300) delay++;
			else if(delay > 300){
				HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET); //Roll
			}
		}
		else{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET); //Stop
			//Comfirm that the base is in the position or not
			if(ev_ok == 1 && hz_ok == 1 && seat_ok == 1){
				if(check_fire == 0){
					delay++;
					if(delay > 600){
						__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,600+10*fire);//Fire
						check_fire = 1;
					}
				}
			//Reset
				if(check_fire == 1){
					check_hook = 0;
					target_length = 0.0;
					actual_length = 0.0;
					angle_st = 0.0;

					seat_ok = 0;

					if(return_value < 8 )	return_value ++;
					else	return_value = 0;
				}
			}
		}
	}
}


