#ifndef __AHT21_H
#define __AHT21_H

#include "stm32f4xx_hal.h"
#include "../IIC/iic_hal.h"

#define AHT21_Device_Addr 0x38

void AHT21_Init(void);

uint8_t AHT21_Read(float *humi, float *temp);


#endif
