#ifndef __DATASAVE_H
#define __DATASAVE_H

#include "../../BSP/BL24C02/bl24c02.h"
#include "../../SYSTEM/delay.h"
void EEPROM_Init(void);

uint8_t EEPROM_Check(void);

uint8_t settingSave(uint8_t addr, uint16_t length, uint8_t *writeBuff);

uint8_t settingGet(uint8_t addr, uint16_t length, uint8_t *readBuff);

#endif