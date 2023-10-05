/*
 * baseball_intake.h
 *
 *  Created on: Sep 18, 2023
 *      Author: kch93
 */

#ifndef INC_BASEBALL_INTAKE_H_
#define INC_BASEBALL_INTAKE_H_
#include "stm32f4xx_hal.h"

//fliping
#define FLIPING_INA_PORT GPIOA
#define FLIPING_INA_PIN GPIO_PIN_4
#define FLIPING_INB_PORT GPIOB
#define FLIPING_INB_PIN GPIO_PIN_0

#define FLIPING_UP_LIMIT_PORT GPIOC
#define FLIPING_UP_LIMIT_PIN GPIO_PIN_12
#define FLIPING_DOWN_LIMIT_PORT GPIOA
#define FLIPING_DOWN_LIMIT_PIN GPIO_PIN_15

//lifting
#define LIFTING_INA_PORT GPIOC
#define LIFTING_INA_PIN GPIO_PIN_3
#define LIFTING_INB_PORT GPIOC
#define LIFTING_INB_PIN GPIO_PIN_2

#define LIFTING_UP_LIMIT_PORT GPIOC
#define LIFTING_UP_LIMIT_PIN GPIO_PIN_13
#define LIFTING_DOWN_LIMIT_PORT GPIOC
#define LIFTING_DOWN_LIMIT_PIN GPIO_PIN_10


int fliping_state(int f_dir);
int lifting_state(int l_dir);

void laji_control();
void laji_Reset();
void laji_stop();

extern int laji_cmd, cmd, laji_ok;
extern int laji_reset;

class laji{
public:
 int ENA;
 void move(int INA, int INB, GPIO_TypeDef* GPIOxA,uint16_t GPIO_PinA, GPIO_TypeDef* GPIOxB,uint16_t GPIO_PinB);
};

extern laji fliping;
extern laji lifting;

#endif /* INC_BASEBALL_INTAKE_H_ */
