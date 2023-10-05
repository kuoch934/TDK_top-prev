/*
 * mainpp.h
 *
 *  Created on: Sep 14, 2023
 *      Author: rabbi
 */

#ifndef INC_MAINPP_H_
#define INC_MAINPP_H_

#include "shooter_base.h"
#include "TIM_IT.h"

#ifdef __cplusplus
 extern "C" {
#endif

 void main_function();

	//STM_main
	void stm_setup(void);
	//timer operate automatically after setup, so STM don't need STM_loop?






#ifdef __cplusplus
}
#endif

#endif /* INC_MAINPP_H_ */
