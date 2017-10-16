/*################## Инициализация MEMS датчика #########################
Порядок инициализации:
1.
################################################################################*/

#include "mems.h"

void MEMS_Init(MEMS* MEMS)
{
  MEMS->memsTimer = &memsTimer;
  MEMS->memsTimer->baseTimer = (TIM_TypeDef*)TIM7_BASE;
  MEMS->memsTimer->timerAPBFreq = sys.pclk1 * 2;
  MEMS->memsTimer->timerPeriod = 1000;
  userTimer_Init(MEMS->memsTimer);
  userTimer_Enable(MEMS->memsTimer);

  userGPIO_Init(GPIOA, 5, af, pullUp, AF5);
  userGPIO_Init(GPIOA, 6, af, pullUp, AF5);
  userGPIO_Init(GPIOA, 7, af, pullUp, AF5);
  userGPIO_Init(GPIOE, 3, output, pullUp, AF0);

  MEMS->memsSPI = &memsSPI;
  MEMS->memsSPI->baseSPI = (SPI_TypeDef*)SPI1_BASE;
  MEMS->memsSPI->dataCaptureEdge = SPI_CAPTURE_EDGE_DROOP;
  MEMS->memsSPI->dataFrameFormat = SPI_FRAME_16BIT;
  MEMS->memsSPI->DMAEnable = SPI_DMADIS;
  MEMS->memsSPI->fpclkDIV = SPI_FREQ_FPCLK_DIV256;
  MEMS->memsSPI->frameTransitionDirection = SPI_FRAME_TRANS_DIR_MSBFIRST;
  MEMS->memsSPI->idleCLKPolarity = SPI_CLK_IDLE_HIGH;
  MEMS->memsSPI->interrupts = SPI_NO_INTERRUPTS;
  MEMS->memsSPI->mode = SPI_MASTER;
  MEMS->memsSPI->softwareSlaveManagement = SPI_SLAVE_MANAGEMENT_SOFT;
  MEMS->memsSPI->slaveSelect = SPI_SLAVE_SELECT;
  userSPI_Init(MEMS->memsSPI);
  userSPI_Enable(MEMS->memsSPI);

  MEMS_WriteReg(MEMS, CTRL_REG4, 0x67);
  DelayUs(20);
  MEMS_WriteReg(MEMS, CTRL_REG5, 0x00);
}

void MEMS_Examine(MEMS* MEMS)
{
    MEMS->memsSPI->baseSPI->CR2 |= SPI_CR2_RXNEIE;
    DelayUs(20);
    MEMS->memsSPI->RxCounter = 0;

    MEMS_ReadReg(MEMS, OUT_X_L);
    DelayUs(20);
    MEMS_ReadReg(MEMS, OUT_X_H);
    DelayUs(20);
    MEMS_ReadReg(MEMS, OUT_Y_L);
    DelayUs(20);
    MEMS_ReadReg(MEMS, OUT_Y_H);
    DelayUs(20);
    MEMS_ReadReg(MEMS, OUT_Z_L);
    DelayUs(20);
    MEMS_ReadReg(MEMS, OUT_Z_H);
    DelayUs(20);
    MEMS_ReadReg(MEMS, OUT_T);
    DelayUs(20);
    MEMS_ReadReg(MEMS, CTRL_REG5);

    MEMS->memsSPI->baseSPI->CR2 &= ~SPI_CR2_RXNEIE;
}

void MEMS_AccelToAngle(MEMS* MEMS)
{
  MEMS->x_Angle = 57.298*Arctan(MEMS->x_Accel/Sqroot(MEMS->y_Accel*MEMS->y_Accel + MEMS->z_Accel*MEMS->z_Accel));
  MEMS->y_Angle = 57.298*Arctan(MEMS->y_Accel/Sqroot(MEMS->x_Accel*MEMS->x_Accel + MEMS->z_Accel*MEMS->z_Accel));
  MEMS->z_Angle = 57.298*Arctan(MEMS->z_Accel/Sqroot(MEMS->y_Accel*MEMS->y_Accel + MEMS->x_Accel*MEMS->x_Accel));
}

void MEMS_Parse(MEMS* MEMS)
{
  uint8_t tmp = (MEMS->memsSPI->RxBuffer[7] >> 3) & 0x07;
  switch(tmp)
  {
  case 0x00:
    MEMS->sensitivity = 0.06;
    MEMS->mes_Range = 4;
    break;
  case 0x01:
    MEMS->sensitivity = 0.12;
    MEMS->mes_Range = 8;
    break;
  case 0x02:
    MEMS->sensitivity = 0.18;
    MEMS->mes_Range = 12;
    break;
  case 0x03:
    MEMS->sensitivity = 0.24;
    MEMS->mes_Range = 16;
    break;
  case 0x04:
    MEMS->sensitivity = 0.73;
    MEMS->mes_Range = 32;
    break;
  default:
    break;
  }

  MEMS->x_Accel = ((float)(MEMS->memsSPI->RxBuffer[0] | (MEMS->memsSPI->RxBuffer[1] << 8))) * MEMS->sensitivity / 1000;
  if(MEMS->x_Accel > 2) MEMS->x_Accel -= MEMS->mes_Range;
  MEMS->y_Accel = ((float)(MEMS->memsSPI->RxBuffer[2] | (MEMS->memsSPI->RxBuffer[3] << 8))) * MEMS->sensitivity / 1000;
  if(MEMS->y_Accel > 2) MEMS->y_Accel -= MEMS->mes_Range;
  MEMS->z_Accel = ((float)(MEMS->memsSPI->RxBuffer[4] | (MEMS->memsSPI->RxBuffer[5] << 8))) * MEMS->sensitivity / 1000;
  if(MEMS->z_Accel > 2) MEMS->z_Accel -= MEMS->mes_Range;
  MEMS->temperature = MEMS->memsSPI->RxBuffer[6];
}

void MEMS_ReadReg(MEMS* MEMS, uint8_t reg)
{
  uint8_t reg_address = 0;

  GPIOE->BSRRH |= GPIO_BSRR_BS_3;

  reg_address = reg | 0x80;
  while(!(MEMS->memsSPI->baseSPI->SR & SPI_SR_TXE));
  MEMS->memsSPI->baseSPI->DR = reg_address << 8;
  for(uint32_t i=0; i < 10; i++)
  {
    __NOP();
  }
  while(MEMS->memsSPI->baseSPI->SR & SPI_SR_BSY);
  while(MEMS->memsSPI->baseSPI->SR & SPI_SR_RXNE);

  GPIOE->BSRRL |= GPIO_BSRR_BS_3;
}

void MEMS_WriteReg(MEMS* MEMS, uint8_t reg, uint8_t data)
{
  uint16_t reg_address = 0;

  GPIOE->BSRRH |= GPIO_BSRR_BS_3;

  reg_address = (reg << 8) | data;
  while(!(MEMS->memsSPI->baseSPI->SR & SPI_SR_TXE));
  MEMS->memsSPI->baseSPI->DR = reg_address;
  for(uint32_t i=0; i < 10; i++)
  {
    __NOP();
  }
  while(MEMS->memsSPI->baseSPI->SR & SPI_SR_BSY);
  //while(!(SPI1->SR & SPI_SR_RXNE));

  GPIOE->BSRRL |= GPIO_BSRR_BS_3;
}

void TIM7_IRQHandler()
{
  TIM7->SR &= ~TIM_SR_UIF;
  //MEMS_Examine(&lis3dsh);
}

void SPI1_IRQHandler()
{
  if((SPI1->SR & SPI_SR_RXNE) && (SPI1->CR2 & SPI_CR2_RXNEIE))
  {
    lis3dsh.memsSPI->RxBuffer[lis3dsh.memsSPI->RxCounter] = lis3dsh.memsSPI->baseSPI->DR;
    lis3dsh.memsSPI->RxCounter++;
    if(lis3dsh.memsSPI->RxCounter > 7)
    {
      lis3dsh.memsSPI->RxCounter = 0;
      MEMS_Parse(&lis3dsh);
      MEMS_AccelToAngle(&lis3dsh);
    }
  }

  if((SPI1->SR & SPI_SR_TXE) && (SPI1->CR2 & SPI_CR2_TXEIE))
  {
    SPI1->CR2 &= ~SPI_CR2_TXEIE;

  }
}