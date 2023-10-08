/*
 * ros_port.h
 *
 *  Created on: Sep 20, 2023
 *      Author: kch93
 */

#ifndef INC_ROS_PORT_H_
#define INC_ROS_PORT_H_

#include "ros.h"
#include "geometry_msgs/Point.h"
#include "std_msgs/Int8.h"

void angle_callback(const geometry_msgs::Point &msg);
void laji_callback(const std_msgs::Int8 &msg);
void ori_callback(const std_msgs::Int8 &msg);

extern int flag;

void pub();

void ros_setup(void);
void ros_loop(void);

#endif /* INC_ROS_PORT_H_ */
