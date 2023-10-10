/*
 * baseball_intake.cpp
 *
 *  Created on: Sep 18, 2023
 *      Author: kch93
 */

#include "baseball_intake.h"
#include "ztest/ros_port.h"

laji fliping;
laji lifting;
int laji_reset = 1;
int laji_cmd, cmd, laji_ok = 1;

int fliping_state(int f_dir){
 int ENA = 0;
 if(f_dir == 1){
  fliping.move(1,0, FLIPING_INA_PORT,FLIPING_INA_PIN,FLIPING_INB_PORT,FLIPING_INB_PIN);
  if(HAL_GPIO_ReadPin(FLIPING_DOWN_LIMIT_PORT, FLIPING_DOWN_LIMIT_PIN)){
   fliping.move(0,0, FLIPING_INA_PORT,FLIPING_INA_PIN,FLIPING_INB_PORT,FLIPING_INB_PIN);
   ENA = 1;
  }
 }
 else{ //f_dir == 0
  fliping.move(0,1, FLIPING_INA_PORT,FLIPING_INA_PIN,FLIPING_INB_PORT,FLIPING_INB_PIN);
  if(HAL_GPIO_ReadPin(FLIPING_UP_LIMIT_PORT, FLIPING_UP_LIMIT_PIN)){
   fliping.move(0,0, FLIPING_INA_PORT,FLIPING_INA_PIN,FLIPING_INB_PORT,FLIPING_INB_PIN);
   ENA = 1;
  }
 }
 return ENA;
}
int lifting_state(int l_dir){
 int ENB = 0;
 if(l_dir == 1){
  lifting.move(1,0, LIFTING_INA_PORT,LIFTING_INA_PIN,LIFTING_INB_PORT,LIFTING_INB_PIN);
  if(HAL_GPIO_ReadPin(LIFTING_DOWN_LIMIT_PORT, LIFTING_DOWN_LIMIT_PIN)){
   lifting.move(0,0, LIFTING_INA_PORT,LIFTING_INA_PIN,LIFTING_INB_PORT,LIFTING_INB_PIN);
   ENB = 1;
  }
 }
 else{ //l_dir == 0
  lifting.move(0,1, LIFTING_INA_PORT,LIFTING_INA_PIN,LIFTING_INB_PORT,LIFTING_INB_PIN);
  if(HAL_GPIO_ReadPin(LIFTING_UP_LIMIT_PORT, LIFTING_UP_LIMIT_PIN)){
   lifting.move(0,0, LIFTING_INA_PORT,LIFTING_INA_PIN,LIFTING_INB_PORT,LIFTING_INB_PIN);
   ENB = 1;
  }
 }
 return ENB;
}
void laji::move(int INA, int INB, GPIO_TypeDef* GPIOxA,uint16_t GPIO_PinA, GPIO_TypeDef* GPIOxB,uint16_t GPIO_PinB){
 if( (INA == 0 && INB == 0) || (INA == 1 && INB == 1) ){
  HAL_GPIO_WritePin(GPIOxA, GPIO_PinA, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOxB, GPIO_PinB, GPIO_PIN_RESET);
 }
 else if( INA == 1 && INB == 0 ){
  HAL_GPIO_WritePin(GPIOxA, GPIO_PinA, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOxB, GPIO_PinB, GPIO_PIN_RESET);
 }
 else if( INA == 0 && INB == 1 ){
  HAL_GPIO_WritePin(GPIOxA, GPIO_PinA, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOxB, GPIO_PinB, GPIO_PIN_SET);
 }
}
int fliping_RisingEdge(int current){
 static int previous = 0;
 int temp;
    temp = current && !previous;
    previous = current;
    return temp;
}
int lifting_RisingEdge(int current){
 static int previous = 0;
 int temp;
 temp = current && !previous;
 previous = current;
 return temp;
}

//if(msg->pin == 1){
// reset = 1;
//}
void laji_Reset(){
 if(laji_reset != 0){
 lifting.ENA = lifting_state(0);
 fliping.ENA = fliping_state(0);
 }
 if(lifting.ENA == 1 && fliping.ENA == 1){
 laji_reset = 0;
 }
}

//if(msg->data == 1){
// cmd = 1;
//}
//if(msg->data == 2){
// cmd = 2;
//}
void laji_control(){
 static int a = 0;
 if(cmd == 1){
  laji_ok = 0;
  fliping.ENA = fliping_state(1);
  lifting.ENA = lifting_state(0);
  if(fliping.ENA == 1 && lifting.ENA == 1){
  cmd = 0;
  laji_ok = 1;
  }
 }
 else if(cmd == 2){
  laji_ok = 0;
  if(a%3 == 0){
   lifting.ENA = lifting_state(1);
   if(lifting.ENA == 1){
    a++;
   }
  }
  else if(a%3 == 1){
   lifting.ENA = lifting_state(0);
   if(lifting.ENA == 1){
    a++;
   }
  }
  else if(a%3 == 2){
   fliping.ENA = fliping_state(0);
   if(fliping.ENA == 1){
    a++;
    cmd = 0;
    laji_ok = 1;
   }
  }
 }
 else if(cmd == 3){
   laji_ok = 0;
   fliping.ENA = fliping_state(0);
   if(fliping.ENA == 1){
    cmd = 0;
     laji_ok = 1;
  }
  }
 else if(cmd == 4){
    laji_ok = 0;
    fliping.ENA = fliping_state(1);
    if(fliping.ENA == 1){
     cmd = 0;
      laji_ok = 1;
   }
   }
  else if(cmd == 5){
   laji_ok = 0;
   lifting.ENA = lifting_state(1);
   if(lifting.ENA == 1){
    cmd = 0;
    laji_ok = 1;
  }
  }
  else if(cmd == 6){
   laji_ok = 0;
   lifting.ENA = lifting_state(0);
   if(lifting.ENA == 1){
    cmd = 0;
    laji_ok = 1;
     }
    }
  else if(cmd == -1){
   laji_stop();
  }
}

void laji_stop(){
 fliping.move(0,0, FLIPING_INA_PORT,FLIPING_INA_PIN,FLIPING_INB_PORT,FLIPING_INB_PIN);
 lifting.move(0,0, LIFTING_INA_PORT,LIFTING_INA_PIN,LIFTING_INB_PORT,LIFTING_INB_PIN);
}
