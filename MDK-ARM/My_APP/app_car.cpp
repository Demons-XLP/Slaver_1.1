///** 
//* @brief    副控功能实现
//* @details  关于气缸、电机功能的实现
//* @author   Xu LiangPu
//* @date      2019.12.8
//* @version  0.1
//* @par Copyright (c):  RM2020电控
//* @par 日志
//*				具体使用方法见Readme.md
//*				版本变更:
//*				2019.12.21		|		1.0		|		整理文件，分离模式控制
//*				
//*/
//                                                                                                                                                                                     
//#include "app_car.hpp"
//#include "main.h"
//#include "cmsis_os.h"
//#include "bsp_motor.hpp"
//#include "task_main.h"
//#include "app_my_can.h"
//#include "global_define.h"

//Motor_t DJI_Motor_3508(8192, 19);
//pid app_remote_Claw_pid_In(1.5,0,0,0,10000,0,0,0);  //移爪内环
//pid app_remote_Claw_pid_Out(1.9,0.1,20,300,10000,0,0,800); //移爪外环
//softmotor app_remote_Claw_motor(1,0x201,&DJI_Motor_3508,&app_remote_Claw_pid_In,&app_remote_Claw_pid_Out);  //移爪电机



//int16_t Slaver_Feedback[4];  //副控反馈给主控的数据
//int8_t Flag1  = 1;  //用来初始化抓取机构电机的初始位置判断信息
//uint8_t Omron[4] = {0,0,0,0};
//float Claw_motor_Origin;  //存放爪子零点
//float Claw_TargetAngle;  //爪子目标值
//float Claw_L_Lim,Claw_R_Lim;  //爪子电机的左右边限位
//uint8_t app_car_ClawTake_Flag1;  //是否一级取弹判断
//uint8_t app_car_ClawTake_Flag2;  //是否二级取弹判断




///** 
//* @brief   进行初始化
//* @remarks 
//*/
//void MotorInit()
//{
//  app_remote_Claw_motor.Enable_Block(8000,20,1);
//	osDelay(1000);  //等待电机数据初始化
//	Claw_motor_Origin = app_remote_Claw_motor.SoftAngle;  //初始化零点信息
//  Omron[0] = HAL_GPIO_ReadPin(Omron1_GPIO_Port,Omron1_Pin);  //左爪
//	Omron[1] = HAL_GPIO_ReadPin(Omron2_GPIO_Port,Omron2_Pin);  
//	Omron[2] = HAL_GPIO_ReadPin(Omron3_GPIO_Port,Omron3_Pin);  //右爪
//	Omron[3] = HAL_GPIO_ReadPin(Omron4_GPIO_Port,Omron4_Pin);
//}



///** 
//* @brief  GPIO外部中断的回调函数，扔这里就行，不用管它
//* 日志    
//*/
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	switch(GPIO_Pin){
//		case Omron1_Pin:
//			Omron[0] = HAL_GPIO_ReadPin(Omron1_GPIO_Port,Omron1_Pin);		//左爪
//		break;
//		case Omron2_Pin:
//			Omron[1] = HAL_GPIO_ReadPin(Omron2_GPIO_Port,Omron2_Pin);   
//		break;
//		case Omron3_Pin:           
//			Omron[2] = HAL_GPIO_ReadPin(Omron3_GPIO_Port,Omron3_Pin);  //右爪
//		break;
//		case Omron4_Pin:
//			Omron[3] = HAL_GPIO_ReadPin(Omron4_GPIO_Port,Omron4_Pin);
//		break;				
//	}
//}

///** 
//* @brief   解析主机发送数据并自行根据判断执行命令
//* @remarks 在主任务中周期执行
//* 日志    
//*/


//void Master_Order_Caculate(int16_t *Master_Order)
//{
//	switch (Master_Order[2])  //模式判断
//	{
//		
//case 32:  //限位初始化，左中右下
//			if(app_remote_Claw_motor.block->IsBlock == 1)
//			{
//			  app_remote_Claw_motor.Safe_Set();  //堵转停止
//			  app_remote_Claw_motor.block->Clear_BlockFlag();
//				Claw_motor_Origin = app_remote_Claw_motor.SoftAngle;  //重新初始化零点
//				if(Claw_motor_Origin < 0)  //左边限位
//				{
//					Claw_L_Lim = Claw_motor_Origin;   //记录左限位
//					Claw_TargetAngle = Claw_motor_Origin + 1100.f;  //回到中间取弹位置的目标值
//				}
//				else  //右边限位
//					{
//						Claw_R_Lim = Claw_motor_Origin;  //记录右限位
//						Claw_TargetAngle = Claw_motor_Origin - 1220.f;  //回到中间取弹位置的目标值
//					}
//				}

//			 Claw_TargetAngle += Master_Order[1] * 0.002;
//			 app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);  //控制爪子电机左右移动  
//			 app_car_ClawTake_Flag1 = ENDING;
//   
//break;
//case 12 :  //副控取弹的模式，左上右下
//			
//			if(Master_Order[0] > 0)  //拨轮上   左边箱
//			{	  
//				app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 80.f);  //限位
//			  Claw_TargetAngle = Claw_L_Lim + 50.f;					
//			}
//		  else if(Master_Order[0] < 0)  //拨轮下  右边箱
//			{
//			  app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 80.f); 
//				Claw_TargetAngle = Claw_L_Lim + 1900.641f;
//		  }
//			if(Master_Order[3] > 0)  //右边通道     上  中间箱 
//			{
//				app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 80.f); 
//				Claw_TargetAngle = Claw_L_Lim + 978.838f;
//			}
//				
//			else if(Master_Order[3] < 0)  //传感器没检测到弹药箱，即可以取弹了
//			{
//				app_car_ClawTake_Flag1 = RUNNING;  //一级取弹开始
//			}
//			app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);
//break;
//case 13:    //自动对位取弹，左上右中
//	    app_car_ClawTake_Flag1 = ENDING;
//break;			
//case 22:  //安全模式
//	    app_remote_Claw_motor.Safe_Set();
//			app_car_ClawTake_Flag1 = ENDING;
//break;
//case 33:  
//     {
//			 if(Master_Order[3] > 0)  //右边通道     上  中间箱 
//			 {
//					app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 80.f); 
//					Claw_TargetAngle = Claw_L_Lim + 978.838f;
//			 }
//					app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);
//					app_car_ClawTake_Flag1 = ENDING;
//	 	 }
//break;
//case   11:  //二级取弹
//			{
//				if(Master_Order[0] > 0)  //拨轮上   左边箱
//				{

//					app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 80.f);  //限位
//					Claw_TargetAngle = Claw_L_Lim + 50.f;

//				}
//				else if(Master_Order[0] < 0)  //拨轮下  右边箱
//				{
//					app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 80.f); 
//					Claw_TargetAngle = Claw_L_Lim + 1900.641f;
//				}
//				if(Master_Order[3] > 0)  //右边通道     上  中间箱 
//				{
//					app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 80.f); 
//					Claw_TargetAngle = Claw_L_Lim + 978.838f;
//				}

//				else if(Master_Order[3] < 0)  //传感器没检测到弹药箱，即可以取弹了
//				{
//					app_car_ClawTake_Flag2 = RUNNING;  //一级取弹开始
//				}
//					app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);
//			}
//break;
//default:
//	    app_remote_Claw_motor.Safe_Set();
//			app_car_ClawTake_Flag1 = ENDING;
//break;
//}

//}

