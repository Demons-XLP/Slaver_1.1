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
	xLastWakeTime = xTaskGetTickCount();//获取当前的系统时间
	
  bsp_ADC_Sensor_Init();  //夏普红外初始化
	bsp_can_Init();  //CAN初始化
  manager::CANSelect(&hcan1,&hcan2);  //选择CAN1和CAN2
	MotorInit();  //电机相关初始化
	osThreadDef(Caisson_TakeTask,Caisson_TakeTask,osPriorityAboveNormal,0,512);
	osThreadCreate(osThread(Caisson_TakeTask),NULL);
	
	for(;;)
	{
		Remote_Control_Handle();  //遥控器分发句柄
	  manager::CANSend();  //电机托管
		vTaskDelayUntil(&xLastWakeTime,1/portTICK_PERIOD_MS); //定周期延时
	}
	
}
 


