#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f4xx.h"

typedef struct
{
  TIM_TypeDef* baseTimer;
  uint32_t timerAPBFreq;        //Частота тактирования таймера, Гц
  uint16_t timerPeriod;         //Период срабатывания таймера, мс
}Timer;     // Параметры таймера

void TIM6_Init(void);
void userTimer_Init(Timer*);
void userTimer_Enable(Timer*);
void userTimer_Disable(Timer*);

#endif