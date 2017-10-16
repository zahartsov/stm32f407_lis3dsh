#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f4xx.h"

#define AF0  0x00
#define AF1  0x01
#define AF2  0x02
#define AF3  0x03
#define AF4  0x04
#define AF5  0x05
#define AF6  0x06
#define AF7  0x07
#define AF8  0x08
#define AF9  0x09
#define AF10 0x0A
#define AF11 0x0B
#define AF12 0x0C
#define AF13 0x0D
#define AF14 0x0E
#define AF15 0x0F

#define noPull    0x00
#define pullUp    0x01
#define pullDown  0x02

#define input     0x00
#define output    0x01
#define af        0x02
#define analog    0x03

typedef struct
{
  GPIO_TypeDef* baseGPIO;
  uint8_t pin;          //
  uint8_t mode;         //
  uint8_t type;         //
  uint8_t pull;         //
  uint8_t speed;        //
  uint8_t AF;           //
}gpio;     // Параметры таймера

//extern gpio

void GPIO_Init(void);
void userGPIO_Init(GPIO_TypeDef* GPIO, uint8_t pin, uint8_t mode, uint8_t pull, uint8_t AF);

#endif