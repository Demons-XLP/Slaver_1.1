#ifndef TASK_AUTO_H
#define  TASK_AUTO_H
#include "stm32f4xx_hal.h"
#include "main.h"

void Caisson_TakeTask(void const *argument);
extern uint8_t task_auto_ClawTake_Flag1_1;  //是否一级取弹第1箱判断
extern uint8_t task_auto_ClawTake_Flag1_2;  //是否一级取弹第2箱判断
extern uint8_t task_auto_ClawTake_Flag2_1;  //是否二级取弹第1箱判断
extern uint8_t task_auto_ClawTake_Flag2_2;  //是否二级取弹第2箱判断
extern uint8_t task_auto_ClawTake_Flag2_3;  //是否二级取弹第3箱判断
extern uint8_t task_auto_ClawTake_Flag2_4;  //是否二级取弹第4箱判断

#define CLAW_OUT_1_ON  				HAL_GPIO_WritePin(Air_Cylinder1_GPIO_Port,Air_Cylinder1_Pin,GPIO_PIN_SET)  //一级伸爪
#define CLAW_OUT_1_OFF  			HAL_GPIO_WritePin(Air_Cylinder1_GPIO_Port,Air_Cylinder1_Pin,GPIO_PIN_RESET)  
#define CLAW_OUT_2_ON  				HAL_GPIO_WritePin(Air_Cylinder2_GPIO_Port,Air_Cylinder2_Pin,GPIO_PIN_SET)  //二级伸爪
#define CLAW_OUT_2_OFF 			  HAL_GPIO_WritePin(Air_Cylinder2_GPIO_Port,Air_Cylinder2_Pin,GPIO_PIN_RESET)
#define OVERTURN_CLAW_ON 		  HAL_GPIO_WritePin(Air_Cylinder3_GPIO_Port,Air_Cylinder3_Pin,GPIO_PIN_SET)  //翻爪
#define OVERTURN_CLAW_OFF  		HAL_GPIO_WritePin(Air_Cylinder3_GPIO_Port,Air_Cylinder3_Pin,GPIO_PIN_RESET) 
#define CLAMP_ON              HAL_GPIO_WritePin(Air_Cylinder4_GPIO_Port,Air_Cylinder4_Pin,GPIO_PIN_SET)  //夹取
#define CLAMP_OFF   				  HAL_GPIO_WritePin(Air_Cylinder4_GPIO_Port,Air_Cylinder4_Pin,GPIO_PIN_RESET)
#define LAUNCH_ON   					HAL_GPIO_WritePin(Air_Cylinder5_GPIO_Port,Air_Cylinder5_Pin,GPIO_PIN_SET)  //弹射
#define LAUNCH_OFF   					HAL_GPIO_WritePin(Air_Cylinder5_GPIO_Port,Air_Cylinder5_Pin,GPIO_PIN_RESET)


#endif
