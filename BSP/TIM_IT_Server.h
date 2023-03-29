#ifndef IT_Server
#define IT_Server


#include "main.h"
#include "tim.h"
#include "stm32f1xx_hal_tim.h"

#include "TIM_IT_Server.h"
#include "BSP_LED.h"

void TIM_Init(void);
void TIM1_UP_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

extern uint32_t Counter1,Counter3,Counter4;
#endif