/** 
* @brief    遥控器模式设置
* @details  遥控器模式分发
* @author   Xu LiangPU
* @date      2019.12
* @version  1.0
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

#include "app_remote.h"
#include "app_my_can.h"
#include "cmsis_os.h"
#include "main.h"
#include "global_define.h"
#include "task_auto.h"

#define L_OMRON Omron[0]   //左爪
#define R_OMRON Omron[1]  //右爪
#define CLAW_IF_OUT Omron[2]  //爪子是否翻出去
Motor_t DJI_Motor_3508(8192, 19);
//pid app_remote_Claw_pid_In(1.5,0,0,0,10000,0,0,0);  //移爪内环
//pid app_remote_Claw_pid_Out(1.9,0.1,20,300,10000,0,0,800); //移爪外环
pid app_remote_Claw_pid_In(3,0,0,0,16000,0,0,0);  //移爪内环
pid app_remote_Claw_pid_Out(2.1,0.1,50,1000,10000,0,0,100); //移爪外环
softmotor app_remote_Claw_motor(1,0x201,&DJI_Motor_3508,&app_remote_Claw_pid_In,&app_remote_Claw_pid_Out);  //移爪电机


static int16_t remote_mode = 22;  //当前遥控器模式，默认为安全
static int16_t last_mode = 0xff;  //上次运行模式参数 
static uint8_t If_Claw_Take_First = 0,If_Claw_Take_Second = 0;  //是否要进行1/2次取弹标志位,1为要取弹
float Claw_motor_Origin;  //存放爪子零点
float Claw_TargetAngle;  //爪子目标值
float Claw_L_Lim,Claw_R_Lim;  //爪子电机的左右边限位
int16_t Slaver_Feedback[4];  //副控反馈给主控的数据
uint8_t Omron[4] = {0,0,0,0};  //欧姆龙值，具体各个值代表什么见 MotorInit()


//////*******************************非模式控制函数但放在此方便管理的函数*******************************************************************************/
/** 
* @brief   进行初始化
* @remarks 
*/
void MotorInit()
{
  app_remote_Claw_motor.Enable_Block(8000,20,1);
	osDelay(1000);  //等待电机数据初始化\\Engineer_Slaver1\My_APP/app_remote.c\Omron[2]
	Claw_motor_Origin = app_remote_Claw_motor.SoftAngle;  //初始化零点信息
  Omron[0] = HAL_GPIO_ReadPin(Omron1_GPIO_Port,Omron1_Pin);  //左爪
	Omron[1] = HAL_GPIO_ReadPin(Omron2_GPIO_Port,Omron2_Pin);  //右爪
	Omron[2] = HAL_GPIO_ReadPin(Omron3_GPIO_Port,Omron3_Pin);  //位置
	Omron[3] = HAL_GPIO_ReadPin(Omron4_GPIO_Port,Omron4_Pin);
}


/** 
* @brief  GPIO外部中断的回调函数，扔这里就行，不用管它
* 日志    
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin){
		case Omron1_Pin:
			Omron[0] = HAL_GPIO_ReadPin(Omron1_GPIO_Port,Omron1_Pin);		//左爪
		break;
		case Omron2_Pin:
			Omron[1] = HAL_GPIO_ReadPin(Omron2_GPIO_Port,Omron2_Pin);   
		break;
		case Omron3_Pin:           
			Omron[2] = HAL_GPIO_ReadPin(Omron3_GPIO_Port,Omron3_Pin);  //右爪
		break;
		case Omron4_Pin:
			Omron[3] = HAL_GPIO_ReadPin(Omron4_GPIO_Port,Omron4_Pin);
		break;				
	}
}

////////******************************遥控器模式分发相关函数*****************************************************************************************/
/** 
    * @brief 安全模式
*/
static void Safe_Mode(uint8_t type)
{
	switch(type)
	{
		case STARTING:
		break;
		case RUNNING: 
					app_remote_Claw_motor.Safe_Set();    //安全
					task_auto_ClawTake_Flag1_1 = ENDING;   //取弹Flag关闭
					task_auto_ClawTake_Flag1_2 = ENDING;  
					task_auto_ClawTake_Flag2_1 = ENDING;    
					task_auto_ClawTake_Flag2_2 = ENDING;    
					task_auto_ClawTake_Flag2_3 = ENDING;    
					task_auto_ClawTake_Flag2_4 = ENDING;   
		break;
		case ENDING:
		break;
		
  }
}

/** 
    * @brief 爪子初始化
*/
static void Claw_Init_Mode(uint8_t type)
{
  switch(type)
	{
	  case STARTING:
		break;
		case RUNNING:
					if(app_remote_Claw_motor.block->IsBlock == 1)
					{
						app_remote_Claw_motor.Safe_Set();  //堵转停止
						app_remote_Claw_motor.block->Clear_BlockFlag();
						Claw_motor_Origin = app_remote_Claw_motor.SoftAngle;  //重新初始化零点
						if(Claw_motor_Origin < 0)  //左边限位
						{
							Claw_L_Lim = Claw_motor_Origin;   //记录左限位
							Claw_TargetAngle = Claw_motor_Origin + 1100.f;  //回到中间取弹位置的目标值
						}
						else  //右边限位
						{
								Claw_R_Lim = Claw_motor_Origin;  //记录右限位
								Claw_TargetAngle = Claw_motor_Origin - 1220.f;  //回到中间取弹位置的目标值
						}
					}
					Claw_TargetAngle += Master_Order[1] * 0.002;   //遥控器控制爪子电机左右移动
					app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);    
					task_auto_ClawTake_Flag1_1 = ENDING;   //取弹Flag关闭
					task_auto_ClawTake_Flag1_2 = ENDING;  
					task_auto_ClawTake_Flag2_1 = ENDING;    
					task_auto_ClawTake_Flag2_2 = ENDING;    
					task_auto_ClawTake_Flag2_3 = ENDING;    
					task_auto_ClawTake_Flag2_4 = ENDING;   
		break;
		case ENDING:
					 Safe_Mode(RUNNING);    //安全
		break;
	}
}

/** 
    * @brief 已经对好位置副控第一次升起取弹
*/
static void Slaver_First_ClawTake(uint8_t type)
{
  switch (type)
	{
	  case STARTING:
					app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 40.f);  //限位
		      Claw_TargetAngle = Claw_L_Lim + 50.f;  //移到最左边
		break;
		case RUNNING:
		      if(Master_Order[0] > 0)  //拨轮上  要取弹
					{
						If_Claw_Take_First = 1;  
				  }
					if(If_Claw_Take_First == 1)  //判断是否可以取弹
					{
						if(ABS(app_remote_Claw_motor.SoftAngle - Claw_TargetAngle) < 1)
						{
							task_auto_ClawTake_Flag1_1 = RUNNING;  //一级取弹开始
							If_Claw_Take_First = 0;
						} 
				  }
					app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);
    break;
		case ENDING:
					 Safe_Mode(RUNNING);    //安全
		break;
	}
}


/** 
    * @brief 已经对好位置副控第二次升起取弹
*/

static void Slaver_Second_ClawTake(uint8_t type)
{
  switch (type)
	{
	  case STARTING:
					 app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 50.f);  //限位
					 Claw_TargetAngle = Claw_L_Lim + 978.838f;  //移到中间
		break;
		case RUNNING:
					if(Master_Order[0] > 0)  //拨轮上  要取弹
					{
						If_Claw_Take_Second = 1;  //表示要一次取弹
				  }
					if(If_Claw_Take_Second == 1)  //判断是否可以取弹
					{
						if(ABS(app_remote_Claw_motor.SoftAngle - Claw_TargetAngle) < 1)
						{
							task_auto_ClawTake_Flag2_1 = RUNNING;  //二次取弹开始
							If_Claw_Take_Second = 0;
						}  
				  }
					app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);
    break;
		case ENDING:
					 Safe_Mode(RUNNING);    //安全
		break;
	}
}

/** 
    * @brief 底盘运动时爪子不动
*/
static void Keep_Claw_Mode(uint8_t type)
{
	switch(type)
	{   
		 case STARTING:
						app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 50.f);  //限位
						Claw_TargetAngle = Claw_L_Lim + 978.838f;	//爪子在中间位置
						task_auto_ClawTake_Flag1_1 = ENDING;   //取弹Flag关闭
						task_auto_ClawTake_Flag1_2 = ENDING;  
						task_auto_ClawTake_Flag2_1 = ENDING;    
						task_auto_ClawTake_Flag2_2 = ENDING;    
						task_auto_ClawTake_Flag2_3 = ENDING;    
						task_auto_ClawTake_Flag2_4 = ENDING;   
	 	 break;
		 case RUNNING:
						app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);  //使保持在中间位置
		 case ENDING:
		 break;
   }
}


/**
* @brief  遥控器mode分发
* @param [in]  mode 遥控器s1*10+s2
* @param [in]  type 运行参数
* @retval  NULL
*/
void Remote_Distribute(int16_t mode,uint8_t type)
{
  switch(mode)
	{
		case 32:	//爪子电机初始化
					Claw_Init_Mode(type);
		break;
		case 12:	//第一次升起取弹
					Slaver_First_ClawTake(type);
		break;
		case 11:  //第二次升起取弹
					Slaver_Second_ClawTake(type);
		break;
		case 33:  //底盘运动时爪子不动
					Keep_Claw_Mode(type);
		break;
		case 22:	//安全
					Safe_Mode(type);
		break;
		default :		//安全
					Safe_Mode(type);
		break;
	}
}

/**
* @brief  遥控器控制函数句柄
* @details  周期执行该句柄
* @param  NULL
* @retval  NULL
*/
void Remote_Control_Handle(void)
{
  remote_mode = Master_Order[2];  //Master_Order[3]为s1*10 + s2
	if(remote_mode != last_mode)  //如果当前模式和之前的模式不一样的话
	{
	  Remote_Distribute(last_mode,ENDING);  //退出之前的模式
		Remote_Distribute(remote_mode,STARTING);  //启用当前模式 开始部分
		Remote_Distribute(remote_mode,RUNNING);  //跑一次当前模式的running
		last_mode = remote_mode;
	}
	else Remote_Distribute(last_mode,RUNNING);  //持续当前模式
}

