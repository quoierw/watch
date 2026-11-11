#include "DataSave.h"


void EEPROM_Init(void)
{
    BL24C02_Init();
}

uint8_t EEPROM_Check(void)
{
    uint8_t checkNum[2] = {0};
    uint8_t writeNum[2] = {0x55, 0xAA};
    delay_ms(5);
    BL24C02_Read(0, 2, checkNum);
    if((checkNum[0] != 0x55) || (checkNum[1] != 0xAA))
    {
        delay_ms(5);
        BL24C02_Write(0, 2, writeNum);
        delay_ms(5);
        BL24C02_Read(0, 2, checkNum);
        delay_ms(5);
        if((checkNum[0] != 0x55) || (checkNum[1] != 0xAA))
        {
            return 1;
        }
        return 0;
    }
    return 0;
}

uint8_t settingSave(uint8_t addr, uint16_t length, uint8_t *writeBuff)
{
    if(addr > 1 && !EEPROM_Check())
    {
        BL24C02_Write(addr, length, writeBuff);
        return 0;
    }
    return 1;
}

uint8_t settingGet(uint8_t addr, uint16_t length, uint8_t *readBuff)
{
    if(addr > 1 && !EEPROM_Check())
    {
        BL24C02_Read(addr, length, readBuff);
        return 0;
    }
    return 1;
}