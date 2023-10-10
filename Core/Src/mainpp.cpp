/*
 * mainpp.cpp
 *
 *  Created on: Sep 14, 2023
 *      Author: rabbi
 */

#include "mainpp.h"
#include "main.h"
#include "ztest/ros_port.h"
#include "baseball_intake.h"
#include "stm32f4xx_hal.h"
#include "shooter.h"
#include "badminton.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim12;

uint16_t angle = 90.0;
int debounce_dis = 0;
int r = 0;
int script_badminton_ENA = 0;
int dised = 0;
int g = 0;

void main_function(){
	ros_setup();
	stm_setup();
//	UART_Send_SetMotorPosition(1, 1400, 200);
	UART_Send_SetMotorPosition(3, 1500, 200);
	__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,600+10*80);
	while(1){
		//software	reset
		if(r==1){
		  HAL_NVIC_SystemReset();
		  r = 0;
		}

		//Servo test
//		__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,600+10*angle);
//		UART_Send_SetMotorPosition(1, 1500, 300); //800 - 2200
//		HAL_Delay(500);
//		UART_Send_SetMotorPosition(3, 2000, 200);
//		HAL_Delay(500);
		g++;
		//VL53 distance detect
		debounce_dis++;
		if(dis != dised)	debounce_dis = 0;
		if(dis == 1 && debounce_dis > 5){
			script_badminton_ENA = 1;
		}
		if(script_badminton_ENA ){
			script_badminton();
			script_badminton_ENA = 0;
		}
		dised = dis;

		//ros
		ros_loop();
	}
}

void stm_setup(void){
	  //PWM
	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);	//PB6 -> attach servo
	  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);	//PB7 -> ball_seat servo

	  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);	//PC6 -> rail servo
	  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);	//PC8 -> badminton
	  HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);	//PC9 -> badminton

	  HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);	//PB14 -> horizontal_angle speed
	  HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);	//PB15 -> elevation_angle speed

	  //Encoder
	  HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_1);	//Shooter Encoder tl1
	  HAL_TIM_Encoder_Start(&htim1,TIM_CHANNEL_2);	//Shooter Encoder tl2
	  HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_1);	//horizontal_angle Encoder tl1
	  HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_2);	//horizontal_angle Encoder tl2
	  HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);	//elevation_angle Encoder tl1
	  HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_2);	//elevation_angle Encoder tl2
	  //Timer Interupt
	  HAL_TIM_Base_Start_IT(&htim5); //PSC 63 , ARR 999 -> 1kHz
}


