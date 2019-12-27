
/** 
	* @brief    主控解析
	* @details  通过CAN接收并解析主控的数据，解析后存放在Master_Order[]里面
	* @author   Xu LiangPu
	* @date     2019.12.21 
	* @version  1.0
	* @par 
	* @par 日志
*/

#include "app_my_can.h"
#include "can.h"
#include "bsp_motor.hpp"
#include "string.h"

#define MasterCan hcan2  //主副控通信为can2通信
#define Master_To_Slaver_ID 0x101  //主控标识符ID
#define SlaverID 0x102  //副控标识符ID
CAN_RxHeaderTypeDef RxHead;

int16_t Master_Order[4];  //解析后存放的主控数据
uint8_t data[8];//接收数据缓冲区


void Master_Order_Caculate(uint8_t data[]);  //主控解析函数




/**
* @brief  CAN接收中断
* @details  重新定义接收中断，会自动在CAN中断中调用
*           为了使副控能接收到主控的命令，在外部重新定义的CAN接收中断
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
* @brief   解析主控发送的数据
* @remarks 在主任务中周期执行
* 日志    
*/
void Master_Order_Caculate(uint8_t *data)
{
	Master_Order[0] = data[0]  << 8 | data[1];  //从主控接收到的拨轮信息
	Master_Order[1] = data[2]  << 8 | data[3];  //控制抓取机构电机左右移动
	Master_Order[2] = data[4]  << 8 | data[5];  //模式设置
	Master_Order[3] = data[6]  << 8 | data[7];  //CH1
}
