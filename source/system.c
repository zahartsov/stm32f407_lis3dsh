/*################## Инициализация системной периферии #########################
Порядок инициализации:
1.
################################################################################*/

#include "system.h"

uint8_t mode = 0;

void System_Init(System* system)
{
  system->timer = &sysTimer;
  system->period = 1000;
  system->hclk = 168000000;
  system->pclk1 = 42000000;
  system->pclk2 = 82000000;

  system->timer->baseTimer = (TIM_TypeDef*)TIM6_BASE;
  system->timer->timerAPBFreq = system->pclk1 * 2;
  system->timer->timerPeriod = system->period;
  userTimer_Init(system->timer);
  userTimer_Enable(system->timer);

  userGPIO_Init(GPIOD,12,output,pullUp,AF0);
  userGPIO_Init(GPIOD,13,output,pullUp,AF0);
  userGPIO_Init(GPIOD,14,output,pullUp,AF0);
  userGPIO_Init(GPIOD,15,output,pullUp,AF0);
}

void DelayMs(uint16_t ms)
{
  uint32_t clockCycle = ms * (sys.hclk / 1000);
  clockCycle /= 5;
  for(uint32_t i = 0; i < clockCycle; i++);
}

void DelayUs(uint16_t us)
{
  uint32_t clockCycle = us * (sys.hclk / 1000000);
  clockCycle /= 5;
  for(uint32_t i = 0; i < clockCycle; i++);
}

void InterruptsPrioritySet()
{
  NVIC_SetPriority(SPI1_IRQn, 0);
  NVIC_SetPriority(TIM7_IRQn, 14);               //Таймер GSM
  NVIC_SetPriority(TIM6_DAC_IRQn, 15);          //Таймер системный
}

void TIM6_DAC_IRQHandler()
{
  sys.timer->baseTimer->SR &= ~TIM_SR_UIF;

  switch(mode)
  {
    case 0:
      GPIOD->BSRRL |= GPIO_BSRR_BS_12;
      GPIOD->BSRRH |= GPIO_BSRR_BS_15;
      break;

    case 1:
      GPIOD->BSRRL |= GPIO_BSRR_BS_13;
      GPIOD->BSRRH |= GPIO_BSRR_BS_12;
      break;

    case 2:
      GPIOD->BSRRL |= GPIO_BSRR_BS_14;
      GPIOD->BSRRH |= GPIO_BSRR_BS_13;
      break;

    case 3:
      GPIOD->BSRRL |= GPIO_BSRR_BS_15;
      GPIOD->BSRRH |= GPIO_BSRR_BS_14;
      break;

    default:
      break;
  }
  mode++;
  if(mode > 3) mode = 0;
}