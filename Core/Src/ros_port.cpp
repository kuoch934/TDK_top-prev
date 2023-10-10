/*
 * ROS_port.cpp
 *
 *  Created on: Sep 20, 2023
 *      Author: kch93
 */
#include "ztest/ros_port.h"
#include "stm32f4xx_hal.h"
#include "STM32Hardware.h"
#include "ros.h"
#include "geometry_msgs/Point.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Int64.h"

#include "shooter.h"
#include "shooter_base.h"
#include "baseball_intake.h"
#include "reset_button.h"
#include "badminton.h"

//int run_inter0 = 0;
//
//
//
//geometry_msgs::Point angle;
extern double target_hz, target_ev, target_length;
int flag;
int dis;
std_msgs::Int8 laji,pitches,st,badminton_ok;
std_msgs::Int64 button_reset;
double shooter_init;
int laji_high = 0;
double mission = 0;

void speed_callback(const geometry_msgs::Twist &msg){
	mission = msg.linear.z;
}
void angle_callback(const geometry_msgs::Point &msg)
{
	ros_hz = msg.x;
	ros_ev = msg.y;
	ros_st = msg.z;
}
void laji_callback(const std_msgs::Int8 &msg){
	laji_cmd = msg.data;
}

void ori_callback(const std_msgs::Int8 &msg){
	flag = msg.data;
}
void badminton_callback(const std_msgs::Int8 &msg){
	dis = msg.data;
}
//void laji_ok(int state){
//	if(state){
//		laji.data = 1;
//	}
//	else{
//		laji.data = 0;
//	}
//}

//mission
ros::Subscriber<geometry_msgs::Twist> sub_speed("realspeed", speed_callback);

ros::NodeHandle nh;
ros::Subscriber<geometry_msgs::Point> sub_angle("/cmd_angle", angle_callback);
ros::Subscriber<std_msgs::Int8> sub_ori("/cmd_ori", ori_callback);

//shooter
ros::Publisher pub_st("st_ok", &st);
ros::Publisher pub_pitches("pitches", &pitches);

//laji
ros::Subscriber<std_msgs::Int8> sub_laji("cmd_laji", laji_callback);
ros::Publisher pub_laji("laji_ok", &laji);

//reset
ros::Publisher pub_reset("reset", &button_reset);

//badminton
ros::Subscriber<std_msgs::Int8> sub_badminton("dis", badminton_callback);
ros::Publisher pub_badminton("badminton_ok", &badminton_ok);

void ros_setup(void)
{
    nh.initNode();
    nh.subscribe(sub_angle);
    nh.subscribe(sub_ori);

	nh.subscribe(sub_laji);
	nh.advertise(pub_laji);

	nh.advertise(pub_reset);

	nh.advertise(pub_pitches);
	nh.advertise(pub_st);

	nh.subscribe(sub_badminton);
	nh.advertise(pub_badminton);

	nh.subscribe(sub_speed);
}
void ros_loop(void)
{
    nh.spinOnce();
}
void pub(){
	//laji
	if(laji_cmd > 0 || laji_high == 1){
		laji.data = laji_ok;
		pub_laji.publish(&laji);
		laji_high = 1;
	}
	//reset button
	button_reset.data = reset_condition();
	pub_reset.publish(&button_reset);
	//shooter
	pitches.data = return_value;
	st.data = st_set;
	//badminton
	badminton_ok.data = sig;
	pub_badminton.publish(&badminton_ok);

	if(ros_hz == 2){
		shooter_init = 1;
	}
	if(shooter_init){
		pub_pitches.publish(&pitches);
		pub_st.publish(&st);
	}
}

/* UART Communication */
//void Error_Handler(void){
//  /* USER CODE BEGIN Error_Handler_Debug */
//  /* User can add his own implementation to report the HAL error return state */
//  __disable_irq();
//  while (1)
//  {
//  }
//  /* USER CODE END Error_Handler_Debug */
//}

static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 57600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
    if(huart == &huart2){
    // set velocity 0 before uart reinitialization


  HAL_UART_DeInit(&huart2);
  MX_USART2_UART_Init();
  nh.getHardware()->init();
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    nh.getHardware()->flush();
}
