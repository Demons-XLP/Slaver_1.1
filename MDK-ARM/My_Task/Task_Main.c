#include "task_main.h"
#include "bsp_can.hpp"
#include "bsp_motor.hpp"
#include "bsp_adc_deal.h"
#include "cmsis_os.h"
#include "app_remote.h"
#include "task_auto.h"
#include "app_my_can.h"


void MainTask(void const * argument)
{
	static TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();//��ȡ��ǰ��ϵͳʱ��
	
  bsp_ADC_Sensor_Init();  //���պ����ʼ��
	bsp_can_Init();  //CAN��ʼ��
  manager::CANSelect(&hcan1,&hcan2);  //ѡ��CAN1��CAN2
	MotorInit();  //�����س�ʼ��
	osThreadDef(Caisson_TakeTask,Caisson_TakeTask,osPriorityAboveNormal,0,512);
	osThreadCreate(osThread(Caisson_TakeTask),NULL);
	
	for(;;)
	{
		Remote_Control_Handle();  //ң�����ַ����
	  manager::CANSend();  //����й�
		vTaskDelayUntil(&xLastWakeTime,1/portTICK_PERIOD_MS); //��������ʱ
	}
	
}
 


