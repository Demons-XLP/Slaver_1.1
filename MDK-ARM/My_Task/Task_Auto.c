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
#include "app_car.hpp"
#include "global_define.h"




void Caisson_TakeTask(void const *argument)
{
  static TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();//��ȡ��ǰ��ϵͳʱ��
	for(;;)
	{
		CaissonTake_FirstRow(app_car_ClawTake_Flag1);  //һ��ȡ��
		CaissonTake_SecondRow(app_car_ClawTake_Flag2);  //����ȡ��
		vTaskDelayUntil(&xLastWakeTime,5/portTICK_PERIOD_MS); //��������ʱ
	}
}



/** 
* @brief   һ��ȡ��������
* @remarks ȡ��һ�ŵ�ҩ��
*           type ���в��������Ƿ�Ҫ���и�ģʽ
* ��־    
*/
void CaissonTake_FirstRow(uint8_t type)
{
	switch(type)
	{
		case ENDING:
			Claw_Out_1_OFF;
    break;
    case RUNNING:
			Claw_Out_1_ON;  //һ����צ
			osDelay(150);
			OverTurn_Claw_ON;  //����צ��
			osDelay(500);  //�ȴ�צ����ȥ
			Clamp_OFF;  //ץȡ��ҩ��
			osDelay(100);  //�ȴ��н���ҩ��
			OverTurn_Claw_OFF;  //����צ��
			osDelay(1500);  //�ȴ��ӵ��価
			Clamp_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
			osDelay(100);  //�ȴ�צ���ɿ���ҩ��
			Launch_ON;  //��צ�ӵ����ȥ
			osDelay(700);  //�ȴ��������
			Launch_OFF; //�ջص�������
		  app_car_ClawTake_Flag1 = ENDING;
		break;
		default:
		break;
	}
  
}

/** 
* @brief   ����ȡ��������
* @remarks ȡ�ڶ��ŵ�ҩ��
*           type ���в��������Ƿ�Ҫ���и�ģʽ
* ��־    
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
			Claw_Out_1_ON;  //һ����צ
			osDelay(150);
			Claw_Out_2_ON;  //������צ
			osDelay(150);
			OverTurn_Claw_ON;  //����צ��
			osDelay(500);  //�ȴ�צ����ȥ
			Clamp_OFF;  //ץȡ��ҩ��
			osDelay(100);  //�ȴ��н���ҩ��
			OverTurn_Claw_OFF;  //����צ��
			osDelay(1500);  //�ȴ��ӵ��価
			Clamp_ON;  //�ɿ�צ�ӣ�׼������ҩ���ӵ�
			osDelay(100);  //�ȴ�צ���ɿ���ҩ��
			Launch_ON;  //��צ�ӵ����ȥ
			osDelay(700);  //�ȴ��������
			Launch_OFF; //�ջص�������
		  app_car_ClawTake_Flag2 = ENDING;
		break;
		default:
		break;
	}
  
}
