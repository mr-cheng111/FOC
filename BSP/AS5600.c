#include "AS5600.h"

#define  RAW_Angle_Hi    0x0C
#define  RAW_Angle_Lo    0x0D
#define  AS5600_Address  0x36

uint8_t Temp_Angle_Data[2] = {0X00, 0X00};

_iq Last_Angle = _IQ(0.0);
uint32_t Last_Time = 0;

void AS5600_Get_RAWData(void)
{
	HAL_I2C_Mem_Read_DMA(&hi2c1,AS5600_Address<<1,RAW_Angle_Hi,1,Temp_Angle_Data,2);
}


float angle_1 = 0;
void AS5600_Get_Angle(void)
{
	_iq Angle_del = 0;
	uint32_t Current_Time = HAL_GetTick();
	FOC.Sensor.Angle = _IQ24mpyI32(_IQ(0.000244140625),(uint32_t)(Temp_Angle_Data[0]<<8|Temp_Angle_Data[1]));
	angle_1 = _IQtoF(FOC.Sensor.Angle);
	if((Current_Time - Last_Time) < 1)
	FOC.Sensor.Rate = _IQdiv(_IQ(Last_Angle - FOC.Sensor.Angle),_IQ(Current_Time - Last_Time));
	Last_Angle = FOC.Sensor.Angle;
	Last_Time = Current_Time;
}


/**
  * @brief This function handles DMA1 channel7 global interrupt.
  */
void DMA1_Channel7_IRQHandler(void)
{
	if (((hdma_i2c1_rx.DmaBaseAddress->ISR & (DMA_FLAG_HT1 << hdma_i2c1_rx.ChannelIndex)) != RESET) &&
			((hdma_i2c1_rx.Instance->CCR & DMA_IT_HT) != RESET))
  {
    if((hdma_i2c1_rx.Instance->CCR & DMA_CCR_CIRC) == 0U)
    {
	  CLEAR_BIT(hdma_i2c1_rx.Instance->CCR ,DMA_IT_HT);
    }
	DMA1->IFCR = DMA_FLAG_HT1;
  }

  else if (((hdma_i2c1_rx.DmaBaseAddress->ISR & (DMA_FLAG_TC1 << hdma_i2c1_rx.ChannelIndex)) != RESET) && 
	       ((hdma_i2c1_rx.Instance->CCR & DMA_IT_TC) != RESET))
  {
    if((hdma_i2c1_rx.Instance->CCR & DMA_CCR_CIRC) == 0U)
    {
			CLEAR_BIT(hdma_i2c1_rx.Instance->CCR ,DMA_IT_TE | DMA_IT_TC);
      hdma_i2c1_rx.State = HAL_DMA_STATE_READY;
			
			//当前DMA已完成传输，开启数据转换
			AS5600_Get_Angle();
			
			
    }
	DMA1->IFCR = DMA_FLAG_TC1;
  }
    __HAL_UNLOCK(&hdma_i2c1_rx);

    if(hdma_i2c1_rx.XferCpltCallback != NULL)
    {
      hdma_i2c1_rx.XferCpltCallback(&hdma_i2c1_rx);
    }
}
