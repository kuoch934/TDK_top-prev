/*
 * reset_button.h
 *
 *  Created on: Oct 3, 2023
 *      Author: kch93
 */

#ifndef INC_RESET_BUTTON_H_
#define INC_RESET_BUTTON_H_

#define MAIN_RESET_PORT GPIOC
#define MAIN_RESET_PIN GPIO_PIN_5

#define STAGE1_PORT GPIOA
#define STAGE1_PIN GPIO_PIN_11

#define BOARD_PORT GPIOB
#define BOARD_PIN GPIO_PIN_2

#define BASEBALL_PORT GPIOB
#define BASEBALL_PIN GPIO_PIN_13

#define BADMINTON_PORT GPIOC
#define BADMINTON_PIN GPIO_PIN_4

int reset_condition();


#endif /* INC_RESET_BUTTON_H_ */
