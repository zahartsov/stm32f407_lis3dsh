#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32f4xx.h"
#include "tim.h"
#include "gpio.h"

typedef struct
{
  Timer* timer;
  uint16_t period;      //������ ������� �����������, ��
  uint32_t hclk;        //������� ����������, ��
  uint32_t pclk1;       //������� ������ ����������� ����, ��
  uint32_t pclk2;       //������� ������ ������������ ����, ��
}System;     // ��������� �������

extern System sys;
extern Timer sysTimer;

void System_Init(System*);
void DelayMs(uint16_t);
void DelayUs(uint16_t);
void InterruptsPrioritySet(void);

#endif