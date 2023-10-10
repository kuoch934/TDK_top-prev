/*
 * reset_button.cpp
 *
 *  Created on: Oct 3, 2023
 *      Author: kch93
 */

#include "reset_button.h"
#include "stm32f4xx_hal.h"

int go,a,b,c,d;
int gone,aa,bb,cc,dd;
int B1,B2,B3,B4,B5;
int debounce[5] = {0,0,0,0,0};
int status = 0;

int reset_condition(){
	debounce[0]++;
	debounce[1]++;
	debounce[2]++;
	debounce[3]++;
	debounce[4]++;
	go = HAL_GPIO_ReadPin(MAIN_RESET_PORT, MAIN_RESET_PIN);
	if(go != gone)	debounce[0] = 0;
	gone = go;
	if(go && debounce[0] > 50){
		a = HAL_GPIO_ReadPin(STAGE1_PORT, STAGE1_PIN);
		b = HAL_GPIO_ReadPin(BOARD_PORT, BOARD_PIN);
		c = HAL_GPIO_ReadPin(BASEBALL_PORT, BASEBALL_PIN);
		d = HAL_GPIO_ReadPin(BADMINTON_PORT, BADMINTON_PIN);

		if(a != aa)	debounce[1] = 0;
		if(b != bb)	debounce[2] = 0;
		if(c != cc)	debounce[3] = 0;
		if(d != dd) debounce[4] = 0;

		if(debounce[1] > 50 && debounce[2] > 50 && debounce[3] > 50 && debounce[4] > 50){
			if(!a & !b & !c & !d){
				status = 1;
			}
			else if(a & !b & !c & !d){
				status = 2;
			}
			else if(a & b & !c & !d){
				status = 3;
			}
			else if(a & !b & c & !d){
				status = 4;
			}
			else if(a & !b & !c & d){
				status = 5;
			}
			else if(a & !b & c & d){
				status = 6;
			}
			else if(a & b & !c & d){
				status = 7;
			}
			else if(a & b & c & !d){
				status = 8;
			}
			else{
				status = 0;
			}
		}

		aa = a;
		bb = b;
		cc = c;
		dd = d;

	}
	else if(!go && debounce[0] > 50){
		status = 0;
	}

	return status;
}

//int debounce_count(int num,int state,int count){
//	static int arr[5];
//
//}
