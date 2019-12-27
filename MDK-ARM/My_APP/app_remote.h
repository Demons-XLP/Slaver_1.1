#ifndef __APP_REMOTE_H
#define __APP_REMOTE_H
#include "bsp_motor.hpp"

void Remote_Control_Handle(void);  //遥控器控制handle
void MotorInit();  //电机初始化
extern Motor_t DJI_Motor_3508; 
extern pid app_remote_Claw_pid_In;  //移爪内环
extern pid app_remote_Claw_pid_Out; //移爪外环
extern softmotor app_remote_Claw_motor;  //移爪电机
extern float Claw_TargetAngle;  //爪子目标值
extern float Claw_L_Lim,Claw_R_Lim;  //爪子电机的左右边限位
extern float Claw_motor_Origin;  //存放爪子零点

#endif
