#ifndef __APP_CAR_HPP
#define __APP_CAR_HPP
#include "stm32f4xx_hal.h"
#include "bsp_motor.hpp"

void Master_Order_Caculate(int16_t *Master_Order);
void MotorInit();
extern Motor_t DJI_Motor_3508;
extern pid app_car_Claw_pid_In;  //移爪内环
extern pid app_car_Claw_pid_Out; //移爪外环
extern softmotor app_car_Claw_motor;  //移爪电机
extern uint8_t app_car_ClawTake_Flag1;  //是否一级取弹判断
extern uint8_t app_car_ClawTake_Flag2;  //是否二级取弹判断





#define L_Omron Omron[0]   //左边传感器
#define R_Omron Omron[2]  //右边传感器

#endif

