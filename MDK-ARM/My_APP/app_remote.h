#ifndef __APP_REMOTE_H
#define __APP_REMOTE_H
#include "bsp_motor.hpp"

void Remote_Control_Handle(void);  //ң��������handle
void MotorInit();  //�����ʼ��
extern Motor_t DJI_Motor_3508; 
extern pid app_remote_Claw_pid_In;  //��צ�ڻ�
extern pid app_remote_Claw_pid_Out; //��צ�⻷
extern softmotor app_remote_Claw_motor;  //��צ���
extern float Claw_TargetAngle;  //צ��Ŀ��ֵ
extern float Claw_L_Lim,Claw_R_Lim;  //צ�ӵ�������ұ���λ
extern float Claw_motor_Origin;  //���צ�����

#endif
