#ifndef TASK_AUTO_H
#define  TASK_AUTO_H
#include "stm32f4xx_hal.h"
#include "main.h"

void Caisson_TakeTask(void const *argument);
extern uint8_t app_car_ClawTake_Flag1_1;  //是否一级取弹第1箱判断
extern uint8_t app_car_ClawTake_Flag1_2;  //是否一级取弹第2箱判断
extern uint8_t app_car_ClawTake_Flag2_1;  //是否二级取弹第1箱判断
extern uint8_t app_car_ClawTake_Flag2_2;  //是否二级取弹第2箱判断
extern uint8_t app_car_ClawTake_Flag2_3;  //是否二级取弹第3箱判断
extern uint8_t app_car_ClawTake_Flag2_4;  //是否二级取弹第4箱判断

#define Claw_Out_1_ON  				HAL_GPIO_WritePin(Air_Cylinder1_GPIO_Port,Air_Cylinder1_Pin,GPIO_PIN_SET)  //一级伸爪
#define Claw_Out_1_OFF  			HAL_GPIO_WritePin(Air_Cylinder1_GPIO_Port,Air_Cylinder1_Pin,GPIO_PIN_RESET)  
#define Claw_Out_2_ON  				HAL_GPIO_WritePin(Air_Cylinder2_GPIO_Port,Air_Cylinder2_Pin,GPIO_PIN_SET)  //二级伸爪
#define Claw_Out_2_OFF 			  HAL_GPIO_WritePin(Air_Cylinder2_GPIO_Port,Air_Cylinder2_Pin,GPIO_PIN_RESET)
#define OverTurn_Claw_ON 		  HAL_GPIO_WritePin(Air_Cylinder3_GPIO_Port,Air_Cylinder3_Pin,GPIO_PIN_SET)  //翻爪
#define OverTurn_Claw_OFF  		HAL_GPIO_WritePin(Air_Cylinder3_GPIO_Port,Air_Cylinder3_Pin,GPIO_PIN_RESET) 
#define Clamp_ON              HAL_GPIO_WritePin(Air_Cylinder4_GPIO_Port,Air_Cylinder4_Pin,GPIO_PIN_SET)  //夹取
#define Clamp_OFF   				  HAL_GPIO_WritePin(Air_Cylinder4_GPIO_Port,Air_Cylinder4_Pin,GPIO_PIN_RESET)
#define Launch_ON   					HAL_GPIO_WritePin(Air_Cylinder5_GPIO_Port,Air_Cylinder5_Pin,GPIO_PIN_SET)  //弹射
#define Launch_OFF   					HAL_GPIO_WritePin(Air_Cylinder5_GPIO_Port,Air_Cylinder5_Pin,GPIO_PIN_RESET)


#endif
