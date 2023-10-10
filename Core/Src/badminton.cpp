
/*
 * badminton.c
 *
 *  Created on: 2023年10月7日
 *      Author: User
 */

#include "Servo.h"
#include "badminton.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim3;

int sig = 0;
int z = 0;

void script_badminton(){
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 600+10*90);//initialize push sg90
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 600+10*90);//initialize grab sg90
	z++;
	HAL_Delay(500);
	UART_Send_SetMotorPosition(4, 1500,300);//down
	HAL_Delay(500);
	UART_Send_SetMotorPosition(5, 900, 200);//take badminton
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 20000 * 23);
	HAL_Delay(500);
	UART_Send_SetMotorPosition(4, 2000,300);
	HAL_Delay(500);
	UART_Send_SetMotorPosition(5, 2000,300);//drop badminton
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, 600+10*90);
	HAL_Delay(500);
	UART_Send_SetMotorPosition(5, 900,300);
	HAL_Delay(500);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);//turn on motor
	HAL_Delay(500);
	UART_Send_SetMotorPosition(3, 1200,500);//lift platform
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 600+10*20);//push
	HAL_Delay(500);
	UART_Send_SetMotorPosition(3, 1500,500);//drop platform
	HAL_Delay(500);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);//turn off motor
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 600+10*90);//pull back
	HAL_Delay(500);

	sig=2;
}





