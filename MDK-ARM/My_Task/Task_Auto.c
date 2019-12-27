/** 
* @brief    �����Զ�ȡ��
* @details  �ⲿͨ���ı�app_car_ClawTake_Flag1��app_car_ClawTake_Flag2�������Ƿ�ȡ��
* @author   Xu LiangPu
* @date      2019.12.21
* @version  1.8
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


#include "task_auto.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "app_remote.h"
#include "global_define.h"
#include "app_remote.h"
uint8_t task_auto_ClawTake_Flag1_1 = ENDING;  //�Ƿ�һ��ȡ����1���ж�
uint8_t task_auto_ClawTake_Flag1_2 = ENDING;  //�Ƿ�һ��ȡ����2���ж�
uint8_t task_auto_ClawTake_Flag2_1 = ENDING;  //�Ƿ����ȡ����1���ж�
uint8_t task_auto_ClawTake_Flag2_2 = ENDING;  //�Ƿ����ȡ����2���ж�
uint8_t task_auto_ClawTake_Flag2_3 = ENDING;  //�Ƿ����ȡ����3���ж�
uint8_t task_auto_ClawTake_Flag2_4 = ENDING;  //�Ƿ����ȡ����4���ж�

void CaissonTake_First_1(uint8_t type);
void CaissonTake_First_2(uint8_t type);
void CaissonTake_Second_1(uint8_t type);
void CaissonTake_Second_2(uint8_t type);
void CaissonTake_Second_3(uint8_t type);
void CaissonTake_Second_4(uint8_t type);


void Caisson_TakeTask(void const *argument)
{
  static TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();//��ȡ��ǰ��ϵͳʱ��
	for(;;)
	{
		CaissonTake_First_1(task_auto_ClawTake_Flag1_1);  //һ��ȡ��
		CaissonTake_First_2(task_auto_ClawTake_Flag1_2);
		CaissonTake_Second_1(task_auto_ClawTake_Flag2_1);  //����ȡ��
		CaissonTake_Second_2(task_auto_ClawTake_Flag2_2);
		CaissonTake_Second_3(task_auto_ClawTake_Flag2_3);
		CaissonTake_Second_4(task_auto_ClawTake_Flag2_4);
		vTaskDelayUntil(&xLastWakeTime,10/portTICK_PERIOD_MS); //��������ʱ
	}
}



/** 
* @brief   һ��ȡ��������
* @remarks ��1��
*           type ���в��������Ƿ�Ҫ���и�ģʽ
* ��־    
*/
void CaissonTake_First_1(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAW_OUT_1_ON;  //һ�����׳�
		       osDelay(100);
					 OVERTURN_CLAW_ON;  //����צ��
					 osDelay(500);  //�ȴ�צ����ȥ
					 CLAMP_OFF;  //ץȡ��ҩ��
				   osDelay(100);  //�ȴ��н���ҩ��
					 OVERTURN_CLAW_OFF;  //����צ��
		       osDelay(800);  //�ȴ�����
					 Claw_TargetAngle = Claw_L_Lim + 1900.641f;  //�Ƶ����ұ�
		       osDelay(1200);  //�ȴ�����©��+��צ
				   task_auto_ClawTake_Flag1_1 = ENDING;  //��������
		       task_auto_ClawTake_Flag1_2 = RUNNING;  //�����ڶ���ȡ��
		break;
		case ENDING  :
		break;
	}
  
}

/** 
* @brief   һ��ȡ��������
* @remarks ��2��
*           type ���в��������Ƿ�Ҫ���и�ģʽ
* ��־    
*/
void CaissonTake_First_2(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAMP_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
					 osDelay(20);  //�ȴ�צ���ɿ���ҩ��
					 LAUNCH_ON;  //��צ�ӵ����ȥ
					 osDelay(800);  //�ȴ��������
					 LAUNCH_OFF; //�ջص�������
					 OVERTURN_CLAW_ON;  //����צ��
					 osDelay(500);  //�ȴ�צ����ȥ
					 CLAMP_OFF;  //ץȡ��ҩ��
				   osDelay(100);  //�ȴ��н���ҩ��
					 OVERTURN_CLAW_OFF;  //����צ��
					 osDelay(800);  //�ȴ�����
					 Claw_TargetAngle = Claw_L_Lim + 978.838f;  //���м�λ��
		       osDelay(1000);   //©��+��צ
					 CLAMP_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
					 osDelay(20);  //�ȴ�צ���ɿ���ҩ��
					 LAUNCH_ON;  //��צ�ӵ����ȥ
					 osDelay(800);  //�ȴ��������
					 LAUNCH_OFF; //�ջص�������
		       task_auto_ClawTake_Flag1_1 = ENDING;  //��������
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
//			CLAW_OUT_1_ON;  //һ����צ
//			osDelay(150);
//			OVERTURN_CLAW_ON;  //����צ��
//			osDelay(500);  //�ȴ�צ����ȥ
//			CLAMP_OFF;  //ץȡ��ҩ��
//			osDelay(100);  //�ȴ��н���ҩ��
//			OVERTURN_CLAW_OFF;  //����צ��
//		  
//			osDelay(1500);  //�ȴ��ӵ��価
//			CLAMP_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
//			osDelay(100);  //�ȴ�צ���ɿ���ҩ��
//			LAUNCH_ON;  //��צ�ӵ����ȥ
//			osDelay(700);  //�ȴ��������
//			LAUNCH_OFF; //�ջص�������
//		  app_car_ClawTake_Flag1 = CLOSE;
//		break;
//		default:
//		break;
//	}
//  
//}

/** 
* @brief   �ڶ���ȡ��������
* @remarks ��1��
*           type ���в��������Ƿ�Ҫ���и�ģʽ
* ��־    
*/
void CaissonTake_Second_1(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAW_OUT_1_ON;  //һ�����׳�
					 OVERTURN_CLAW_ON;  //����צ��
					 osDelay(500);  //�ȴ�צ����ȥ
					 CLAMP_OFF;  //ץȡ��ҩ��
				   osDelay(100);  //�ȴ��н���ҩ��
					 OVERTURN_CLAW_OFF;  //����צ��
					 osDelay(800);  //�ȴ�����
					 Claw_TargetAngle = Claw_L_Lim + 50.f;  //׼��ȡ��2�䣬�Ƶ������
		       osDelay(1000);  //�ȴ�����©��
				   task_auto_ClawTake_Flag2_1 = ENDING;  //������1������
		       task_auto_ClawTake_Flag2_2 = RUNNING;  //�����ڶ���ȡ��
		break;
		case ENDING  :
		break;
	}
  
}

/** 
* @brief   ����ȡ��������
* @remarks ��2��
*           type ���в��������Ƿ�Ҫ���и�ģʽ
* ��־    
*/
void CaissonTake_Second_2(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 
					 CLAMP_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
					 osDelay(30);  //�ȴ�צ���ɿ���ҩ��
					 LAUNCH_ON;  //��צ�ӵ����ȥ
					 osDelay(700);  //�ȴ��������
					 LAUNCH_OFF; //�ջص�������
					 CLAW_OUT_2_ON;  //������צ
		       osDelay(200);
					 OVERTURN_CLAW_ON;  //����צ��
					 osDelay(500);  //�ȴ�צ����ȥ
					 CLAMP_OFF;  //ץȡ��ҩ��
				   osDelay(100);  //�ȴ��н���ҩ��
					 OVERTURN_CLAW_OFF;  //����צ��
					 osDelay(800);  //�ȴ�����
					 Claw_TargetAngle = Claw_L_Lim + 978.838f;   //׼��ȡ�����䣬���м�
		       osDelay(1000);   //©��+��צ
					 task_auto_ClawTake_Flag2_2 = ENDING;  
		       task_auto_ClawTake_Flag2_3 = RUNNING;
		break;
		case ENDING  :
		break;
	}
  
}
/** 
* @brief   ����ȡ��������
* @remarks ��3��
*           type ���в��������Ƿ�Ҫ���и�ģʽ
* ��־    
*/
void CaissonTake_Second_3(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAMP_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
					 osDelay(30);  //�ȴ�צ���ɿ���ҩ��
					 LAUNCH_ON;  //��צ�ӵ����ȥ
					 osDelay(700);  //�ȴ��������
					 LAUNCH_OFF; //�ջص�������
					 OVERTURN_CLAW_ON;  //����צ��
					 osDelay(500);  //�ȴ�צ����ȥ
					 CLAMP_OFF;  //ץȡ��ҩ��
				   osDelay(100);  //�ȴ��н���ҩ��
					 OVERTURN_CLAW_OFF;  //����צ��
					 osDelay(800);  //�ȴ�����
					 Claw_TargetAngle = Claw_L_Lim + 1900.641f;   //׼��ȡ��4�䣬�����ұ�
		       osDelay(1000);   //©��+��צ
					 task_auto_ClawTake_Flag2_3 = ENDING;  
		       task_auto_ClawTake_Flag2_4 = RUNNING;
		break;
		case ENDING  :
		break;
	}
  
}

/** 
* @brief   ����ȡ��������
* @remarks ��4��
*           type ���в��������Ƿ�Ҫ���и�ģʽ
* ��־    
*/
void CaissonTake_Second_4(uint8_t type)
{
	switch(type)
	{
		case STARTING:  
    break;
    case RUNNING:
					 CLAMP_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
					 osDelay(30);  //�ȴ�צ���ɿ���ҩ��
					 LAUNCH_ON;  //��צ�ӵ����ȥ
					 osDelay(700);  //�ȴ��������
					 LAUNCH_OFF; //�ջص�������
					 OVERTURN_CLAW_ON;  //����צ��
					 osDelay(500);  //�ȴ�צ����ȥ
					 CLAMP_OFF;  //ץȡ��ҩ��
				   osDelay(100);  //�ȴ��н���ҩ��
					 OVERTURN_CLAW_OFF;  //����צ��
					 osDelay(800);  //�ȴ�����
					 Claw_TargetAngle = Claw_L_Lim + 978.838f;   //�ص��м�
		       osDelay(1000);   //©��+��צ
					 CLAMP_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
					 osDelay(30);  //�ȴ�צ���ɿ���ҩ��
					 LAUNCH_ON;  //��צ�ӵ����ȥ
					 osDelay(700);  //�ȴ��������
					 LAUNCH_OFF; //�ջص�������
					 task_auto_ClawTake_Flag2_1 = ENDING;  //��������
					 task_auto_ClawTake_Flag2_2 = ENDING;
					 task_auto_ClawTake_Flag2_3 = ENDING;
					 task_auto_ClawTake_Flag2_4 = ENDING;
		break;
		case ENDING  :
						CLAW_OUT_1_OFF;  //�ջ�����
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
//			CLAW_OUT_1_ON;  //һ����צ
//			osDelay(150);
//			CLAW_OUT_2_ON;  //������צ
//			osDelay(150);
//			OVERTURN_CLAW_ON;  //����צ��
//			osDelay(500);  //�ȴ�צ����ȥ
//			CLAMP_OFF;  //ץȡ��ҩ��
//			osDelay(100);  //�ȴ��н���ҩ��
//			OVERTURN_CLAW_OFF;  //����צ��
//			osDelay(1500);  //�ȴ��ӵ��価
//			CLAMP_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
//			osDelay(100);  //�ȴ�צ���ɿ���ҩ��
//			LAUNCH_ON;  //��צ�ӵ����ȥ
//			osDelay(700);  //�ȴ��������
//			LAUNCH_OFF; //�ջص�������
//		  app_car_ClawTake_Flag2 = CLOSE;
//		break;
//		default:
//		break;
//	}
//  
//}
