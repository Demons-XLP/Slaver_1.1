
/** 
	* @brief    ���ؽ���
	* @details  ͨ��CAN���ղ��������ص����ݣ�����������Master_Order[]����
	* @author   Xu LiangPu
	* @date     2019.12.21 
	* @version  1.0
	* @par 
	* @par ��־
*/

#include "app_my_can.h"
#include "can.h"
#include "bsp_motor.hpp"
#include "string.h"

#define MasterCan hcan2  //������ͨ��Ϊcan2ͨ��
#define Master_To_Slaver_ID 0x101  //���ر�ʶ��ID
#define SlaverID 0x102  //���ر�ʶ��ID
CAN_RxHeaderTypeDef RxHead;

int16_t Master_Order[4];  //�������ŵ���������
uint8_t data[8];//�������ݻ�����


void Master_Order_Caculate(uint8_t data[]);  //���ؽ�������




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
		if(RxHead.StdId == Master_To_Slaver_ID)
		{
		  Master_Order_Caculate(data);
		}
		else manager::CANUpdate(hcan, &RxHead, (uint8_t*)data);
	}
	else manager::CANUpdate(hcan, &RxHead, (uint8_t*)data);
}


/** 
* @brief   �������ط��͵�����
* @remarks ��������������ִ��
* ��־    
*/
void Master_Order_Caculate(uint8_t *data)
{
	Master_Order[0] = data[0]  << 8 | data[1];  //�����ؽ��յ��Ĳ�����Ϣ
	Master_Order[1] = data[2]  << 8 | data[3];  //����ץȡ������������ƶ�
	Master_Order[2] = data[4]  << 8 | data[5];  //ģʽ����
	Master_Order[3] = data[6]  << 8 | data[7];  //CH1
}
