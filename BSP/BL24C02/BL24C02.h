#ifndef __BL24C02_H
#define __BL24C02_H

#include "stm32f4xx_hal.h"
#include "../IIC/iic_hal.h"

#define BL24C02_Device_Addr 0x50

void BL24C02_Write(uint8_t address, uint16_t length, uint8_t *writeBuff);
void BL24C02_Read(uint8_t address, uint16_t length, uint8_t *readBuff);
void BL24C02_Init(void);
void BL24C02_BufferWrite(uint8_t address, uint16_t numOfwrite, uint8_t *writeBuff);

#endif  