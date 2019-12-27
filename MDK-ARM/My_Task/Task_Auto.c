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
#include "app_remote.h"
#include "global_define.h"
#include "app_remote.h"
uint8_t task_auto_ClawTake_Flag1_1 = ENDING;  //是否一级取弹第1箱判断
uint8_t task_auto_ClawTake_Flag1_2 = ENDING;  //是否一级取弹第2箱判断
uint8_t task_auto_ClawTake_Flag2_1 = ENDING;  //是否二级取弹第1箱判断
uint8_t task_auto_ClawTake_Flag2_2 = ENDING;  //是否二级取弹第2箱判断
uint8_t task_auto_ClawTake_Flag2_3 = ENDING;  //是否二级取弹第3箱判断
uint8_t task_auto_ClawTake_Flag2_4 = ENDING;  //是否二级取弹第4箱判断

void CaissonTake_First_1(uint8_t type);
void CaissonTake_First_2(uint8_t type);
void CaissonTake_Second_1(uint8_t type);
void CaissonTake_Second_2(uint8_t type);
void CaissonTake_Second_3(uint8_t type);
void CaissonTake_Second_4(uint8_t type);


void Caisson_TakeTask(void const *argument)
{
  static TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();//获取当前的系统时间
	for(;;)
	{
		CaissonTake_First_1(task_auto_ClawTake_Flag1_1);  //一次取弹
		CaissonTake_First_2(task_auto_ClawTake_Flag1_2);
		CaissonTake_Second_1(task_auto_ClawTake_Flag2_1);  //二次取弹
		CaissonTake_Second_2(task_auto_ClawTake_Flag2_2);
		CaissonTake_Second_3(task_auto_ClawTake_Flag2_3);
		CaissonTake_Second_4(task_auto_ClawTake_Flag2_4);
		vTaskDelayUntil(&xLastWakeTime,10/portTICK_PERIOD_MS); //定周期延时
	}
}



/** 
* @brief   一次取弹任务函数
* @remarks 第1箱
*           type 运行参数，即是否要运行该模式
* 日志    
*/
void CaissonTake_First_1(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAW_OUT_1_ON;  //一级气缸出
		       osDelay(100);
					 OVERTURN_CLAW_ON;  //翻出爪子
					 osDelay(500);  //等待爪子下去
					 CLAMP_OFF;  //抓取弹药箱
				   osDelay(100);  //等待夹紧弹药箱
					 OVERTURN_CLAW_OFF;  //翻回爪子
		       osDelay(800);  //等待翻回
					 Claw_TargetAngle = Claw_L_Lim + 1900.641f;  //移到最右边
		       osDelay(1200);  //等待弹丸漏尽+移爪
				   task_auto_ClawTake_Flag1_1 = ENDING;  //结束任务
		       task_auto_ClawTake_Flag1_2 = RUNNING;  //开启第二箱取弹
		break;
		case ENDING  :
		break;
	}
  
}

/** 
* @brief   一次取弹任务函数
* @remarks 第2箱
*           type 运行参数，即是否要运行该模式
* 日志    
*/
void CaissonTake_First_2(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAMP_ON;  //松开爪子，准备将弹药箱扔掉
					 osDelay(20);  //等待爪子松开弹药箱
					 LAUNCH_ON;  //将爪子弹射出去
					 osDelay(800);  //等待弹射完成
					 LAUNCH_OFF; //收回弹射气缸
					 OVERTURN_CLAW_ON;  //翻出爪子
					 osDelay(500);  //等待爪子下去
					 CLAMP_OFF;  //抓取弹药箱
				   osDelay(100);  //等待夹紧弹药箱
					 OVERTURN_CLAW_OFF;  //翻回爪子
					 osDelay(800);  //等待翻回
					 Claw_TargetAngle = Claw_L_Lim + 978.838f;  //回中间位置
		       osDelay(1000);   //漏弹+移爪
					 CLAMP_ON;  //松开爪子，准备将弹药箱扔掉
					 osDelay(20);  //等待爪子松开弹药箱
					 LAUNCH_ON;  //将爪子弹射出去
					 osDelay(800);  //等待弹射完成
					 LAUNCH_OFF; //收回弹射气缸
		       task_auto_ClawTake_Flag1_1 = ENDING;  //结束任务
				   task_auto_ClawTake_Flag1_2 = ENDING;  
		break;
		case ENDING  :
						CLAW_OUT_1_OFF;  
		break;
	}
  
}


//void CaissonTake_FirstRow(uint8_t type)
//{
//	switch(type)
//	{
//		case CLOSE:
//			CLAW_OUT_1_OFF;
//    break;
//    case OPEN:
//			CLAW_OUT_1_ON;  //一级伸爪
//			osDelay(150);
//			OVERTURN_CLAW_ON;  //翻出爪子
//			osDelay(500);  //等待爪子下去
//			CLAMP_OFF;  //抓取弹药箱
//			osDelay(100);  //等待夹紧弹药箱
//			OVERTURN_CLAW_OFF;  //翻回爪子
//		  
//			osDelay(1500);  //等待子弹落尽
//			CLAMP_ON;  //松开爪子，准备将弹药箱扔掉
//			osDelay(100);  //等待爪子松开弹药箱
//			LAUNCH_ON;  //将爪子弹射出去
//			osDelay(700);  //等待弹射完成
//			LAUNCH_OFF; //收回弹射气缸
//		  app_car_ClawTake_Flag1 = CLOSE;
//		break;
//		default:
//		break;
//	}
//  
//}

/** 
* @brief   第二次取弹任务函数
* @remarks 第1箱
*           type 运行参数，即是否要运行该模式
* 日志    
*/
void CaissonTake_Second_1(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAW_OUT_1_ON;  //一级气缸出
					 OVERTURN_CLAW_ON;  //翻出爪子
					 osDelay(500);  //等待爪子下去
					 CLAMP_OFF;  //抓取弹药箱
				   osDelay(100);  //等待夹紧弹药箱
					 OVERTURN_CLAW_OFF;  //翻回爪子
					 osDelay(800);  //等待翻回
					 Claw_TargetAngle = Claw_L_Lim + 50.f;  //准备取第2箱，移到最左边
		       osDelay(1000);  //等待弹丸漏尽
				   task_auto_ClawTake_Flag2_1 = ENDING;  //结束第1箱任务
		       task_auto_ClawTake_Flag2_2 = RUNNING;  //开启第二箱取弹
		break;
		case ENDING  :
		break;
	}
  
}

/** 
* @brief   二次取弹任务函数
* @remarks 第2箱
*           type 运行参数，即是否要运行该模式
* 日志    
*/
void CaissonTake_Second_2(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 
					 CLAMP_ON;  //松开爪子，准备将弹药箱扔掉
					 osDelay(30);  //等待爪子松开弹药箱
					 LAUNCH_ON;  //将爪子弹射出去
					 osDelay(700);  //等待弹射完成
					 LAUNCH_OFF; //收回弹射气缸
					 CLAW_OUT_2_ON;  //二级伸爪
		       osDelay(200);
					 OVERTURN_CLAW_ON;  //翻出爪子
					 osDelay(500);  //等待爪子下去
					 CLAMP_OFF;  //抓取弹药箱
				   osDelay(100);  //等待夹紧弹药箱
					 OVERTURN_CLAW_OFF;  //翻回爪子
					 osDelay(800);  //等待翻回
					 Claw_TargetAngle = Claw_L_Lim + 978.838f;   //准备取第三箱，在中间
		       osDelay(1000);   //漏弹+移爪
					 task_auto_ClawTake_Flag2_2 = ENDING;  
		       task_auto_ClawTake_Flag2_3 = RUNNING;
		break;
		case ENDING  :
		break;
	}
  
}
/** 
* @brief   二次取弹任务函数
* @remarks 第3箱
*           type 运行参数，即是否要运行该模式
* 日志    
*/
void CaissonTake_Second_3(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAMP_ON;  //松开爪子，准备将弹药箱扔掉
					 osDelay(30);  //等待爪子松开弹药箱
					 LAUNCH_ON;  //将爪子弹射出去
					 osDelay(700);  //等待弹射完成
					 LAUNCH_OFF; //收回弹射气缸
					 OVERTURN_CLAW_ON;  //翻出爪子
					 osDelay(500);  //等待爪子下去
					 CLAMP_OFF;  //抓取弹药箱
				   osDelay(100);  //等待夹紧弹药箱
					 OVERTURN_CLAW_OFF;  //翻回爪子
					 osDelay(800);  //等待翻回
					 Claw_TargetAngle = Claw_L_Lim + 1900.641f;   //准备取第4箱，在最右边
		       osDelay(1000);   //漏弹+移爪
					 task_auto_ClawTake_Flag2_3 = ENDING;  
		       task_auto_ClawTake_Flag2_4 = RUNNING;
		break;
		case ENDING  :
		break;
	}
  
}

/** 
* @brief   二次取弹任务函数
* @remarks 第4箱
*           type 运行参数，即是否要运行该模式
* 日志    
*/
void CaissonTake_Second_4(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAMP_ON;  //松开爪子，准备将弹药箱扔掉
					 osDelay(30);  //等待爪子松开弹药箱
					 LAUNCH_ON;  //将爪子弹射出去
					 osDelay(700);  //等待弹射完成
					 LAUNCH_OFF; //收回弹射气缸
					 OVERTURN_CLAW_ON;  //翻出爪子
					 osDelay(500);  //等待爪子下去
					 CLAMP_OFF;  //抓取弹药箱
				   osDelay(100);  //等待夹紧弹药箱
					 OVERTURN_CLAW_OFF;  //翻回爪子
					 osDelay(800);  //等待翻回
					 Claw_TargetAngle = Claw_L_Lim + 978.838f;   //回到中间
		       osDelay(1000);   //漏弹+移爪
					 CLAMP_ON;  //松开爪子，准备将弹药箱扔掉
					 osDelay(30);  //等待爪子松开弹药箱
					 LAUNCH_ON;  //将爪子弹射出去
					 osDelay(700);  //等待弹射完成
					 LAUNCH_OFF; //收回弹射气缸
					 task_auto_ClawTake_Flag2_1 = ENDING;  //结束任务
					 task_auto_ClawTake_Flag2_2 = ENDING;
					 task_auto_ClawTake_Flag2_3 = ENDING;
					 task_auto_ClawTake_Flag2_4 = ENDING;
		break;
		case ENDING  :
						CLAW_OUT_1_OFF;  //收回气缸
						CLAW_OUT_2_OFF;
		break;
	}
  
}

//void CaissonTake_SecondRow(uint8_t type)
//{
//	switch(type)
//	{
//		case CLOSE:
//			CLAW_OUT_1_OFF;
//		  CLAW_OUT_2_OFF;
//    break;
//    case OPEN:
//			CLAW_OUT_1_ON;  //一级伸爪
//			osDelay(150);
//			CLAW_OUT_2_ON;  //二级伸爪
//			osDelay(150);
//			OVERTURN_CLAW_ON;  //翻出爪子
//			osDelay(500);  //等待爪子下去
//			CLAMP_OFF;  //抓取弹药箱
//			osDelay(100);  //等待夹紧弹药箱
//			OVERTURN_CLAW_OFF;  //翻回爪子
//			osDelay(1500);  //等待子弹落尽
//			CLAMP_ON;  //松开爪子，准备将弹药箱扔掉
//			osDelay(100);  //等待爪子松开弹药箱
//			LAUNCH_ON;  //将爪子弹射出去
//			osDelay(700);  //等待弹射完成
//			LAUNCH_OFF; //收回弹射气缸
//		  app_car_ClawTake_Flag2 = CLOSE;
//		break;
//		default:
//		break;
//	}
//  
//}
