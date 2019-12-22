/** 
* @brief    副控自动取弹
* @details  外部通过改变app_car_ClawTake_Flag1和app_car_ClawTake_Flag2来控制是否取弹
* @author   Xu LiangPu
* @date      2019.12.21
* @version  1.8
* @par Copyright (c):  RM2020电控
* @par 日志
*				具体使用方法见Readme.md
*				版本变更:
*				2019.10		|		1.0		|		调通CAN总线相关中断处理函数
*				2019.10		|		1.1		|		加入条件编译,自主选择开启某个CAN线
*				2019.10		|		1.2		|		加入条件编译,分为使用信号量和普通中断两种
*				2019.10		|		1.3		|		修改发送相关函数，使得更方便使用
*				2019.10		|		1.4		|		根据代码规范修改了一部分函数名称
*				2019.10		|		1.5		|		参考RM19代码，修改对外接口函数，封装更彻底
*				2019.10		|		1.6		|		修改接收中断和CAN线发送函数逻辑
*				2019.11.11|		1.7		|		添加对初始化结果的检测和更改发送函数的返回类型，并对结果做检测，此次改动是为了适配C++电机库
*				2019.11.15|		1.8		|		在回调参数中固定添加C++电机库的解析函数，现在两个库可以直接捆绑使用无需做过多修改了
*				2019.11.15|		1.9		|		修改了函数注释
*				2019.11.15|		2.0		|		去除了信号量使用的条件编译，现在只需要选择使用哪一个CAN总线了，添加了一个freertos的条件编译使用选项
*/


#include "task_auto.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "app_car.hpp"
#include "global_define.h"




void Caisson_TakeTask(void const *argument)
{
  static TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();//获取当前的系统时间
	for(;;)
	{
		CaissonTake_FirstRow(app_car_ClawTake_Flag1);  //一级取弹
		CaissonTake_SecondRow(app_car_ClawTake_Flag2);  //二级取弹
		vTaskDelayUntil(&xLastWakeTime,5/portTICK_PERIOD_MS); //定周期延时
	}
}



/** 
* @brief   一级取弹任务函数
* @remarks 取第一排弹药箱
*           type 运行参数，即是否要运行该模式
* 日志    
*/
void CaissonTake_FirstRow(uint8_t type)
{
	switch(type)
	{
		case ENDING:
			Claw_Out_1_OFF;
    break;
    case RUNNING:
			Claw_Out_1_ON;  //一级伸爪
			osDelay(150);
			OverTurn_Claw_ON;  //翻出爪子
			osDelay(500);  //等待爪子下去
			Clamp_OFF;  //抓取弹药箱
			osDelay(100);  //等待夹紧弹药箱
			OverTurn_Claw_OFF;  //翻回爪子
			osDelay(1500);  //等待子弹落尽
			Clamp_ON;  //松开爪子，准备将弹药箱扔掉
			osDelay(100);  //等待爪子松开弹药箱
			Launch_ON;  //将爪子弹射出去
			osDelay(700);  //等待弹射完成
			Launch_OFF; //收回弹射气缸
		  app_car_ClawTake_Flag1 = ENDING;
		break;
		default:
		break;
	}
  
}

/** 
* @brief   二级取弹任务函数
* @remarks 取第二排弹药箱
*           type 运行参数，即是否要运行该模式
* 日志    
*/
void CaissonTake_SecondRow(uint8_t type)
{
	switch(type)
	{
		case ENDING:
			Claw_Out_1_OFF;
		  Claw_Out_2_OFF;
    break;
    case RUNNING:
			Claw_Out_1_ON;  //一级伸爪
			osDelay(150);
			Claw_Out_2_ON;  //二级伸爪
			osDelay(150);
			OverTurn_Claw_ON;  //翻出爪子
			osDelay(500);  //等待爪子下去
			Clamp_OFF;  //抓取弹药箱
			osDelay(100);  //等待夹紧弹药箱
			OverTurn_Claw_OFF;  //翻回爪子
			osDelay(1500);  //等待子弹落尽
			Clamp_ON;  //松开爪子，准备将弹药箱扔掉
			osDelay(100);  //等待爪子松开弹药箱
			Launch_ON;  //将爪子弹射出去
			osDelay(700);  //等待弹射完成
			Launch_OFF; //收回弹射气缸
		  app_car_ClawTake_Flag2 = ENDING;
		break;
		default:
		break;
	}
  
}
