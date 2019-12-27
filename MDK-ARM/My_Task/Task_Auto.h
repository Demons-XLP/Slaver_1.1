#ifndef TASK_AUTO_H
#define  TASK_AUTO_H
#include "stm32f4xx_hal.h"
#include "main.h"

void Caisson_TakeTask(void const *argument);
extern uint8_t task_auto_ClawTake_Flag1_1;  //�Ƿ�һ��ȡ����1���ж�
extern uint8_t task_auto_ClawTake_Flag1_2;  //�Ƿ�һ��ȡ����2���ж�
extern uint8_t task_auto_ClawTake_Flag2_1;  //�Ƿ����ȡ����1���ж�
extern uint8_t task_auto_ClawTake_Flag2_2;  //�Ƿ����ȡ����2���ж�
extern uint8_t task_auto_ClawTake_Flag2_3;  //�Ƿ����ȡ����3���ж�
extern uint8_t task_auto_ClawTake_Flag2_4;  //�Ƿ����ȡ����4���ж�

#define CLAW_OUT_1_ON  				HAL_GPIO_WritePin(Air_Cylinder1_GPIO_Port,Air_Cylinder1_Pin,GPIO_PIN_SET)  //һ����צ
#define CLAW_OUT_1_OFF  			HAL_GPIO_WritePin(Air_Cylinder1_GPIO_Port,Air_Cylinder1_Pin,GPIO_PIN_RESET)  
#define CLAW_OUT_2_ON  				HAL_GPIO_WritePin(Air_Cylinder2_GPIO_Port,Air_Cylinder2_Pin,GPIO_PIN_SET)  //������צ
#define CLAW_OUT_2_OFF 			  HAL_GPIO_WritePin(Air_Cylinder2_GPIO_Port,Air_Cylinder2_Pin,GPIO_PIN_RESET)
#define OVERTURN_CLAW_ON 		  HAL_GPIO_WritePin(Air_Cylinder3_GPIO_Port,Air_Cylinder3_Pin,GPIO_PIN_SET)  //��צ
#define OVERTURN_CLAW_OFF  		HAL_GPIO_WritePin(Air_Cylinder3_GPIO_Port,Air_Cylinder3_Pin,GPIO_PIN_RESET) 
#define CLAMP_ON              HAL_GPIO_WritePin(Air_Cylinder4_GPIO_Port,Air_Cylinder4_Pin,GPIO_PIN_SET)  //��ȡ
#define CLAMP_OFF   				  HAL_GPIO_WritePin(Air_Cylinder4_GPIO_Port,Air_Cylinder4_Pin,GPIO_PIN_RESET)
#define LAUNCH_ON   					HAL_GPIO_WritePin(Air_Cylinder5_GPIO_Port,Air_Cylinder5_Pin,GPIO_PIN_SET)  //����
#define LAUNCH_OFF   					HAL_GPIO_WritePin(Air_Cylinder5_GPIO_Port,Air_Cylinder5_Pin,GPIO_PIN_RESET)


#endif
