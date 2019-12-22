/** 
	* @brief    CAN
	* @details  CAN�ص�����
	* @author   Xu LiangPu
	* @date     2019.12.6 
	* @version  1.0
	* @par 
	* @par ��־
*/

#include "bsp_my_can.h"
#include "can.h"
#include "bsp_motor.hpp"
#include "string.h"

#define MasterCan hcan2  //������ͨ��Ϊcan2ͨ��
#define MasterID 0x101  //���ر�ʶ��ID
#define SlaverID 0x102  //���ر�ʶ��ID
CAN_RxHeaderTypeDef RxHead;

uint8_t Master_Order[8];
static float data[8];//�������ݻ�����


/**
* @brief  CAN�����ж�
* @details  ���¶�������жϣ����Զ���CAN�ж��е���
*           Ϊ��ʹ�����ܽ��յ����ص�������ⲿ���¶����CAN�����ж�
* @param  NULL
* @retval  NULL
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHead, (uint8_t*)data);
	if(hcan == &MasterCan)
	{
		if(RxHead.StdId == MasterID)
		{
		  memcpy(Master_Order,data,8);
		}
		else manager::CANUpdate(hcan, &RxHead, (uint8_t*)data);
	}
	else manager::CANUpdate(hcan, &RxHead, (uint8_t*)data);
}


