#include "bsp_can.h"

extern CAN_HandleTypeDef hcan;
static uint32_t             send_mail_box;
static CAN_TxHeaderTypeDef  can_tx_message;
       uint8_t              can_tx_data[8];

void process_motor_measure(motor_measure_t *motor_measure);
/**
  * @brief          
  * @param[in]      
  * @retval         
  */
void Can_Filter_Init(void)
{

    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO1;
    HAL_CAN_ConfigFilter(&hcan, &can_filter_st);
    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING);

}

void CanSendMessage(CAN_HandleTypeDef *hcan, uint32_t id, uint32_t dlc, uint8_t *message)
{
    can_tx_message.IDE = CAN_ID_STD;
    can_tx_message.RTR = CAN_RTR_DATA;
    can_tx_message.DLC = dlc;
    
    if (can_tx_message.DLC > 8)
	{
        can_tx_message.DLC = 8;
    }
    can_tx_message.StdId = id;
    
    HAL_CAN_AddTxMessage(hcan, &can_tx_message, message, &send_mail_box);
}



void Can_Send_Data(CAN_HandleTypeDef *hcan,uint16_t frame,uint8_t Data_Length,uint8_t *data)
{
	  if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0)
		{
			uint32_t send_mail_box_h;
			can_tx_message.StdId = frame;
			can_tx_message.IDE = CAN_ID_STD;
			can_tx_message.RTR = CAN_RTR_DATA;
			can_tx_message.DLC = Data_Length;
			can_tx_data[0] = *(data+0);
			can_tx_data[1] = *(data+1); 
			can_tx_data[2] = *(data+2);
			can_tx_data[3] = *(data+3);
			can_tx_data[4] = *(data+4);
			can_tx_data[5] = *(data+5);
			can_tx_data[6] = *(data+6);
			can_tx_data[7] = *(data+7);
			HAL_CAN_AddTxMessage(hcan, &can_tx_message,can_tx_data, &send_mail_box);
		}
		
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rx_header, rx_data);
    

    switch (rx_header.StdId)
    {
        default:
        {
            break;
        }
    }
}






