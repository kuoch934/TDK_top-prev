/*
 * reset_button.cpp
 *
 *  Created on: Oct 3, 2023
 *      Author: kch93
 */

#include "reset_button.h"
#include "stm32f4xx_hal.h"

int go,a,b,c,d;
int reset_condition(){
	go = HAL_GPIO_ReadPin(MAIN_RESET_PORT, MAIN_RESET_PIN);
	if(go){
		a = HAL_GPIO_ReadPin(STAGE1_PORT, STAGE1_PIN);
		b = HAL_GPIO_ReadPin(BOARD_PORT, BOARD_PIN);
		c = HAL_GPIO_ReadPin(BASEBALL_PORT, BASEBALL_PIN);
		d = HAL_GPIO_ReadPin(BADMINTON_PORT, BADMINTON_PIN);
		if(!a & !b & !c & !d){
			return 1;
		}
		else if(a & !b & !c & !d){
			return 2;
		}
		else if(a & b & !c & !d){
			return 3;
		}
		else if(a & b & c & !d){
			return 4;
		}
		else if(a & b & !c & d){
			return 5;
		}
	}
	else{
		return 0;
	}
	return 0;
}
