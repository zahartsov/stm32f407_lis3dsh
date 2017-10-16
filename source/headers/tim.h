#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f4xx.h"

typedef struct
{
  TIM_TypeDef* baseTimer;
  uint32_t timerAPBFreq;        //������� ������������ �������, ��
  uint16_t timerPeriod;         //������ ������������ �������, ��
}Timer;     // ��������� �������

void TIM6_Init(void);
void userTimer_Init(Timer*);
void userTimer_Enable(Timer*);
void userTimer_Disable(Timer*);

#endif