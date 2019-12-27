/** 
* @brief    ң����ģʽ����
* @details  ң����ģʽ�ַ�
* @author   Xu LiangPU
* @date      2019.12
* @version  1.0
* @par Copyright (c):  RM2020���
* @par ��־
*				����ʹ�÷�����Readme.md
*				�汾���:
*				2019.10		|		1.0		|		��ͨCAN��������жϴ�����
*				2019.10		|		1.1		|		������������,����ѡ����ĳ��CAN��
*				2019.10		|		1.2		|		������������,��Ϊʹ���ź�������ͨ�ж�����
*				2019.10		|		1.3		|		�޸ķ�����غ�����ʹ�ø�����ʹ��
*				2019.10		|		1.4		|		���ݴ���淶�޸���һ���ֺ�������
*				2019.10		|		1.5		|		�ο�RM19���룬�޸Ķ���ӿں�������װ������
*				2019.10		|		1.6		|		�޸Ľ����жϺ�CAN�߷��ͺ����߼�
*				2019.11.11|		1.7		|		��ӶԳ�ʼ������ļ��͸��ķ��ͺ����ķ������ͣ����Խ������⣬�˴θĶ���Ϊ������C++�����
*				2019.11.15|		1.8		|		�ڻص������й̶����C++�����Ľ����������������������ֱ������ʹ�������������޸���
*				2019.11.15|		1.9		|		�޸��˺���ע��
*				2019.11.15|		2.0		|		ȥ�����ź���ʹ�õ��������룬����ֻ��Ҫѡ��ʹ����һ��CAN�����ˣ������һ��freertos����������ʹ��ѡ��
*/

#include "app_remote.h"
#include "app_my_can.h"
#include "cmsis_os.h"
#include "main.h"
#include "global_define.h"
#include "task_auto.h"

#define L_OMRON Omron[0]   //��צ
#define R_OMRON Omron[1]  //��צ
#define CLAW_IF_OUT Omron[2]  //צ���Ƿ񷭳�ȥ
Motor_t DJI_Motor_3508(8192, 19);
//pid app_remote_Claw_pid_In(1.5,0,0,0,10000,0,0,0);  //��צ�ڻ�
//pid app_remote_Claw_pid_Out(1.9,0.1,20,300,10000,0,0,800); //��צ�⻷
pid app_remote_Claw_pid_In(3,0,0,0,16000,0,0,0);  //��צ�ڻ�
pid app_remote_Claw_pid_Out(2.1,0.1,50,1000,10000,0,0,100); //��צ�⻷
softmotor app_remote_Claw_motor(1,0x201,&DJI_Motor_3508,&app_remote_Claw_pid_In,&app_remote_Claw_pid_Out);  //��צ���


static int16_t remote_mode = 22;  //��ǰң����ģʽ��Ĭ��Ϊ��ȫ
static int16_t last_mode = 0xff;  //�ϴ�����ģʽ���� 
static uint8_t If_Claw_Take_First = 0,If_Claw_Take_Second = 0;  //�Ƿ�Ҫ����1/2��ȡ����־λ,1ΪҪȡ��
float Claw_motor_Origin;  //���צ�����
float Claw_TargetAngle;  //צ��Ŀ��ֵ
float Claw_L_Lim,Claw_R_Lim;  //צ�ӵ�������ұ���λ
int16_t Slaver_Feedback[4];  //���ط��������ص�����
uint8_t Omron[4] = {0,0,0,0};  //ŷķ��ֵ���������ֵ����ʲô�� MotorInit()


//////*******************************��ģʽ���ƺ��������ڴ˷������ĺ���*******************************************************************************/
/** 
* @brief   ���г�ʼ��
* @remarks 
*/
void MotorInit()
{
  app_remote_Claw_motor.Enable_Block(8000,20,1);
	osDelay(1000);  //�ȴ�������ݳ�ʼ��\\Engineer_Slaver1\My_APP/app_remote.c\Omron[2]
	Claw_motor_Origin = app_remote_Claw_motor.SoftAngle;  //��ʼ�������Ϣ
  Omron[0] = HAL_GPIO_ReadPin(Omron1_GPIO_Port,Omron1_Pin);  //��צ
	Omron[1] = HAL_GPIO_ReadPin(Omron2_GPIO_Port,Omron2_Pin);  //��צ
	Omron[2] = HAL_GPIO_ReadPin(Omron3_GPIO_Port,Omron3_Pin);  //λ��
	Omron[3] = HAL_GPIO_ReadPin(Omron4_GPIO_Port,Omron4_Pin);
}


/** 
* @brief  GPIO�ⲿ�жϵĻص���������������У����ù���
* ��־    
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin){
		case Omron1_Pin:
			Omron[0] = HAL_GPIO_ReadPin(Omron1_GPIO_Port,Omron1_Pin);		//��צ
		break;
		case Omron2_Pin:
			Omron[1] = HAL_GPIO_ReadPin(Omron2_GPIO_Port,Omron2_Pin);   
		break;
		case Omron3_Pin:           
			Omron[2] = HAL_GPIO_ReadPin(Omron3_GPIO_Port,Omron3_Pin);  //��צ
		break;
		case Omron4_Pin:
			Omron[3] = HAL_GPIO_ReadPin(Omron4_GPIO_Port,Omron4_Pin);
		break;				
	}
}

////////******************************ң����ģʽ�ַ���غ���*****************************************************************************************/
/** 
    * @brief ��ȫģʽ
*/
static void Safe_Mode(uint8_t type)
{
	switch(type)
	{
		case STARTING:
		break;
		case RUNNING: 
					app_remote_Claw_motor.Safe_Set();    //��ȫ
					task_auto_ClawTake_Flag1_1 = ENDING;   //ȡ��Flag�ر�
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
    * @brief צ�ӳ�ʼ��
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
						app_remote_Claw_motor.Safe_Set();  //��תֹͣ
						app_remote_Claw_motor.block->Clear_BlockFlag();
						Claw_motor_Origin = app_remote_Claw_motor.SoftAngle;  //���³�ʼ�����
						if(Claw_motor_Origin < 0)  //�����λ
						{
							Claw_L_Lim = Claw_motor_Origin;   //��¼����λ
							Claw_TargetAngle = Claw_motor_Origin + 1100.f;  //�ص��м�ȡ��λ�õ�Ŀ��ֵ
						}
						else  //�ұ���λ
						{
								Claw_R_Lim = Claw_motor_Origin;  //��¼����λ
								Claw_TargetAngle = Claw_motor_Origin - 1220.f;  //�ص��м�ȡ��λ�õ�Ŀ��ֵ
						}
					}
					Claw_TargetAngle += Master_Order[1] * 0.002;   //ң��������צ�ӵ�������ƶ�
					app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);    
					task_auto_ClawTake_Flag1_1 = ENDING;   //ȡ��Flag�ر�
					task_auto_ClawTake_Flag1_2 = ENDING;  
					task_auto_ClawTake_Flag2_1 = ENDING;    
					task_auto_ClawTake_Flag2_2 = ENDING;    
					task_auto_ClawTake_Flag2_3 = ENDING;    
					task_auto_ClawTake_Flag2_4 = ENDING;   
		break;
		case ENDING:
					 Safe_Mode(RUNNING);    //��ȫ
		break;
	}
}

/** 
    * @brief �Ѿ��Ժ�λ�ø��ص�һ������ȡ��
*/
static void Slaver_First_ClawTake(uint8_t type)
{
  switch (type)
	{
	  case STARTING:
					app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 40.f);  //��λ
		      Claw_TargetAngle = Claw_L_Lim + 50.f;  //�Ƶ������
		break;
		case RUNNING:
		      if(Master_Order[0] > 0)  //������  Ҫȡ��
					{
						If_Claw_Take_First = 1;  
				  }
					if(If_Claw_Take_First == 1)  //�ж��Ƿ����ȡ��
					{
						if(ABS(app_remote_Claw_motor.SoftAngle - Claw_TargetAngle) < 1)
						{
							task_auto_ClawTake_Flag1_1 = RUNNING;  //һ��ȡ����ʼ
							If_Claw_Take_First = 0;
						} 
				  }
					app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);
    break;
		case ENDING:
					 Safe_Mode(RUNNING);    //��ȫ
		break;
	}
}


/** 
    * @brief �Ѿ��Ժ�λ�ø��صڶ�������ȡ��
*/

static void Slaver_Second_ClawTake(uint8_t type)
{
  switch (type)
	{
	  case STARTING:
					 app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 50.f);  //��λ
					 Claw_TargetAngle = Claw_L_Lim + 978.838f;  //�Ƶ��м�
		break;
		case RUNNING:
					if(Master_Order[0] > 0)  //������  Ҫȡ��
					{
						If_Claw_Take_Second = 1;  //��ʾҪһ��ȡ��
				  }
					if(If_Claw_Take_Second == 1)  //�ж��Ƿ����ȡ��
					{
						if(ABS(app_remote_Claw_motor.SoftAngle - Claw_TargetAngle) < 1)
						{
							task_auto_ClawTake_Flag2_1 = RUNNING;  //����ȡ����ʼ
							If_Claw_Take_Second = 0;
						}  
				  }
					app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);
    break;
		case ENDING:
					 Safe_Mode(RUNNING);    //��ȫ
		break;
	}
}

/** 
    * @brief �����˶�ʱצ�Ӳ���
*/
static void Keep_Claw_Mode(uint8_t type)
{
	switch(type)
	{   
		 case STARTING:
						app_remote_Claw_motor.Limit(Claw_R_Lim - 80.f,Claw_L_Lim + 50.f);  //��λ
						Claw_TargetAngle = Claw_L_Lim + 978.838f;	//צ�����м�λ��
						task_auto_ClawTake_Flag1_1 = ENDING;   //ȡ��Flag�ر�
						task_auto_ClawTake_Flag1_2 = ENDING;  
						task_auto_ClawTake_Flag2_1 = ENDING;    
						task_auto_ClawTake_Flag2_2 = ENDING;    
						task_auto_ClawTake_Flag2_3 = ENDING;    
						task_auto_ClawTake_Flag2_4 = ENDING;   
	 	 break;
		 case RUNNING:
						app_remote_Claw_motor.Angle_Set(Claw_TargetAngle);  //ʹ�������м�λ��
		 case ENDING:
		 break;
   }
}


/**
* @brief  ң����mode�ַ�
* @param [in]  mode ң����s1*10+s2
* @param [in]  type ���в���
* @retval  NULL
*/
void Remote_Distribute(int16_t mode,uint8_t type)
{
  switch(mode)
	{
		case 32:	//צ�ӵ����ʼ��
					Claw_Init_Mode(type);
		break;
		case 12:	//��һ������ȡ��
					Slaver_First_ClawTake(type);
		break;
		case 11:  //�ڶ�������ȡ��
					Slaver_Second_ClawTake(type);
		break;
		case 33:  //�����˶�ʱצ�Ӳ���
					Keep_Claw_Mode(type);
		break;
		case 22:	//��ȫ
					Safe_Mode(type);
		break;
		default :		//��ȫ
					Safe_Mode(type);
		break;
	}
}

/**
* @brief  ң�������ƺ������
* @details  ����ִ�иþ��
* @param  NULL
* @retval  NULL
*/
void Remote_Control_Handle(void)
{
  remote_mode = Master_Order[2];  //Master_Order[3]Ϊs1*10 + s2
	if(remote_mode != last_mode)  //�����ǰģʽ��֮ǰ��ģʽ��һ���Ļ�
	{
	  Remote_Distribute(last_mode,ENDING);  //�˳�֮ǰ��ģʽ
		Remote_Distribute(remote_mode,STARTING);  //���õ�ǰģʽ ��ʼ����
		Remote_Distribute(remote_mode,RUNNING);  //��һ�ε�ǰģʽ��running
		last_mode = remote_mode;
	}
	else Remote_Distribute(last_mode,RUNNING);  //������ǰģʽ
}

