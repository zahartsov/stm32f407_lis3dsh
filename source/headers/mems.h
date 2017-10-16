#ifndef __MEMS_H__
#define __MEMS_H__

#include "stm32f4xx.h"
#include "tim.h"
#include "spi.h"
#include "gpio.h"
#include "system.h"
#include "math.h"

#define CTRL_REG4       0x20
#define CTRL_REG5       0x24

#define OUT_X_L         0x28
#define OUT_X_H         0x29
#define OUT_Y_L         0x2A
#define OUT_Y_H         0x2B
#define OUT_Z_L         0x2C
#define OUT_Z_H         0x2D

#define OUT_T           0x0C

typedef struct
{
  float x_Accel;
  float y_Accel;
  float z_Accel;
  float x_Angle;
  float y_Angle;
  float z_Angle;
  float sensitivity;
  uint8_t mes_Range;
  uint8_t temperature;
  Timer* memsTimer;
  SPI* memsSPI;
}MEMS;     // Параметры системы

extern MEMS lis3dsh;
extern Timer memsTimer;
extern SPI memsSPI;

void MEMS_Init(MEMS*);
void MEMS_Examine(MEMS*);
void MEMS_ReadReg(MEMS*, uint8_t);
void MEMS_WriteReg(MEMS*, uint8_t, uint8_t);
void MEMS_Parse(MEMS*);
void MEMS_AccelToAngle(MEMS*);

#endif