/*
 * servo.cpp
 *
 *  Created on: Oct 5, 2023
 *      Author: rabbi
 */

#include "servo.h"
#include"math.h"
extern UART_HandleTypeDef huart3;
int Checksum_Calc, count=0;
void UART_Send(uint8_t u8_data){
 uint8_t *u8_pointer = &u8_data;
 HAL_UART_Transmit(&huart3, u8_pointer, 1,100);

 Checksum_Calc += u8_data;
}

void UART_Send_SetMotorPosition(uint16_t motorId, uint16_t Position, uint16_t Time){
 Checksum_Calc = 0;
 UART_Send(0x80 + motorId);
 UART_Send(0x83);
 UART_Send(0x05);
 UART_Send((Position / 256)& 0x7F);
 UART_Send(Position % 256);
 UART_Send((Time / 256)& 0x7F);
 UART_Send(Time % 256);
 UART_Send(Checksum_Calc);
}


