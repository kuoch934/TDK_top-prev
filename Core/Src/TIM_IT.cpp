/*
 * TIM_IT.cpp
 *
 *  Created on: Sep 14, 2023
 *      Author: rabbi
 */

/************** F446RE **************/

#include "TIM_IT.h"
#include "baseball_intake.h"
#include "shooter_base.h"
#include "ztest/ros_port.h"
//#include "ros_port.h"

extern int seat_mode;

int fu,fl,lu,ll;
int t = 0;
int x = 0;
int debounce_ev = 0;
int buttoned_ev = 0;
int button_ev = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance == TIM5){
		//reset sensors
		debounce_ev++;
//		hz_origin = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
		button_ev = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);

		if(buttoned_ev != button_ev)	debounce_ev = 0;
		buttoned_ev = button_ev;

		if(debounce_ev > 500)	ev_origin = button_ev;

		//ball seat
		switch(seat_mode){
			case 1: //loaded
				__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,600+10*loaded);
				break;
			case 2:	//reload
				__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,600+10*reload);
				break;
		}

		//shooter
		if(reset == 1)	Reset();
		else{
			shooter_base();
			base_limit();
		}
		Script();
		shooter();
		//laji car
		fl = HAL_GPIO_ReadPin(FLIPING_DOWN_LIMIT_PORT, FLIPING_DOWN_LIMIT_PIN);
		fu = HAL_GPIO_ReadPin(FLIPING_UP_LIMIT_PORT, FLIPING_UP_LIMIT_PIN);
		ll = HAL_GPIO_ReadPin(LIFTING_DOWN_LIMIT_PORT, LIFTING_DOWN_LIMIT_PIN);
		lu = HAL_GPIO_ReadPin(LIFTING_UP_LIMIT_PORT, LIFTING_UP_LIMIT_PIN);
		if(laji_cmd == 1){
			cmd = laji_cmd;
		}
		else if(laji_cmd == 2){
			cmd = laji_cmd;
		}
		if(laji_reset == 1){
			laji_Reset();
		}
		else{
			laji_control();
		}
		t++;
		if(t%20 == 0){
			t = 0;
			x++;
			pub();
		}
	}
}
