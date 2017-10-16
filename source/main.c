#include "stm32f4xx.h"
#include "tim.h"
#include "gpio.h"
#include "system.h"
#include "mems.h"

Timer sysTimer;
System sys;
MEMS lis3dsh;
Timer memsTimer;
SPI memsSPI;

int main()
{
  System_Init(&sys);
  MEMS_Init(&lis3dsh);

  while(1)
  {
    DelayMs(1000);
    MEMS_Examine(&lis3dsh);
  }
}