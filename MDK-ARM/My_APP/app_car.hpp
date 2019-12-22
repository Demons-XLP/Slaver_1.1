#ifndef __APP_CAR_HPP
#define __APP_CAR_HPP
#include "stm32f4xx_hal.h"
#include "bsp_motor.hpp"

void Master_Order_Caculate(int16_t *Master_Order);
void MotorInit();
extern Motor_t DJI_Motor_3508;
extern pid app_car_Claw_pid_In;  //��צ�ڻ�
extern pid app_car_Claw_pid_Out; //��צ�⻷
extern softmotor app_car_Claw_motor;  //��צ���
extern uint8_t app_car_ClawTake_Flag1;  //�Ƿ�һ��ȡ���ж�
extern uint8_t app_car_ClawTake_Flag2;  //�Ƿ����ȡ���ж�





#define L_Omron Omron[0]   //��ߴ�����
#define R_Omron Omron[2]  //�ұߴ�����

#endif

