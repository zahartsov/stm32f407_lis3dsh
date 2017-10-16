#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32f4xx.h"
#include "tim.h"
#include "gpio.h"

typedef struct
{
  Timer* timer;
  uint16_t period;      //период мигани€ светодиодом, мс
  uint32_t hclk;        //частота процессора, √ц
  uint32_t pclk1;       //частота первой приферийной шины, √ц
  uint32_t pclk2;       //частота второй периферийной шины, √ц
}System;     // ѕараметры системы

extern System sys;
extern Timer sysTimer;

void System_Init(System*);
void DelayMs(uint16_t);
void DelayUs(uint16_t);
void InterruptsPrioritySet(void);

#endif