/*##################################### TIM ###################################
������� �������������:
1. ��������� ������������ ������� � �������� RCC->APB2ENR (TIM 1,8,9,10,11) � RCC->APB1ENR (���������) ���������� ���� RCC_APB1ENR_TIM�EN, ��� � - ����� �������.
2. ��������� ������������������� ������ ������ ���������� �������� � TIM�->CR2, ��� � - ����� �������.
3. ��������� ���������� ������� � �������� TIM�->DIER, ��� � - ����� �������:
    TIM_DIER_UIE - ��������� ���������� �� ������������ (?);
    TIM_DIER_UDE - ��������� ������� � DMA �� ������������ (?).
4. ��������� ������������ � �������� TIM�->PSC
5. ��������� ������� ����� TIM�->ARR
    ������
    ������� ���� ���1 = 84��� (��. ���� system_stm32f4xx.c ������� SetSysClock())
    ������������ PSC = 8400
    !!!!!� ������� ������������ 8400 - 1 = 8399!!!!!
    ������ ARR = 10 000
    !!!!!� ������� ������������ 10 000 - 1 = 9999!!!!!
    (84 000 000 / 8400) * 10 000 = 1 ���
    ������ ����������� 1 ��� � �������.
6. ��������� ���������� � NVIC:
    NVIC_EnableIRQ(IRQn_Type IRQn).
7. ��������� ������ ������� � ��� ��������� � �������� TIM�->CR1:
    TIM_CR1_CEN - ��������� �������;
    TIM_CR1_ARPE - ��������� ������������ ��������� ������� ����� �������.
################################################################################*/


#include "tim.h"

void TIM6_Init()
{
  //�������� ������������ ����������
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

  //��������� ���������� �� ������������
  TIM6->DIER |= TIM_DIER_UIE;

  //��������� ������������
  TIM6->PSC = 8399;
  //��������� ������� ����� ������� (1�)
  TIM6->ARR = 9999;

  //��������� ���������� �� ������������
  NVIC_EnableIRQ(TIM6_DAC_IRQn);

  //��������� ������� � ��������� ��������� �������
  TIM6->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
}

void userTimer_Init(Timer* timer)
{
  if(timer->baseTimer == (TIM_TypeDef*)TIM1_BASE)
  {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    NVIC_EnableIRQ(TIM1_CC_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM2_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    NVIC_EnableIRQ(TIM2_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM3_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    NVIC_EnableIRQ(TIM3_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM4_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    NVIC_EnableIRQ(TIM4_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM5_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    NVIC_EnableIRQ(TIM5_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM6_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM7_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    NVIC_EnableIRQ(TIM7_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM8_BASE)
  {
    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
    NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
    NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
    NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
    NVIC_EnableIRQ(TIM8_CC_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM9_BASE)
  {
    RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM10_BASE)
  {
    RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM11_BASE)
  {
    RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM12_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
    NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM13_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
    NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM14_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  }

  timer->baseTimer->DIER |= TIM_DIER_UIE;
  timer->baseTimer->PSC = (timer->timerAPBFreq / 10000) - 1;
  timer->baseTimer->ARR = (timer->timerPeriod * 10) - 1;

  timer->baseTimer->CR1 |= TIM_CR1_ARPE;
}

void userTimer_Enable(Timer* timer)
{
  timer->baseTimer->CR1 |= TIM_CR1_CEN;
}

void userTimer_Disable(Timer* timer)
{
  timer->baseTimer->CR1 &= ~TIM_CR1_CEN;
}