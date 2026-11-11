#include "BL24C02.h"

#define BL24C02_CLKEN   __HAL_RCC_GPIOA_CLK_ENABLE()
#define PAGE_WRITE  16

iic_bus_t BL24C02_bus = {
    .IIC_SCL_Pin = GPIO_PIN_12,
    .IIC_SDA_Pin = GPIO_PIN_11,
    .IIC_SCL_PORT = GPIOA,
    .IIC_SDA_PORT = GPIOA,
};

void BL24C02_Write(uint8_t address, uint16_t length, uint8_t *writeBuff)
{
    IIC_WriteMultiByte(&BL24C02_bus, BL24C02_Device_Addr, address, length, writeBuff);

}

void BL24C02_Read(uint8_t address, uint16_t length, uint8_t *readBuff)
{
    IIC_ReadMultiByte(&BL24C02_bus, BL24C02_Device_Addr, address, length, readBuff);
}

void BL24C02_AcknowledgePolling()
{
    uint8_t recVal = 0;
    do{
        IIC_Start(&BL24C02_bus);

        IIC_SendByte(&BL24C02_bus, BL24C02_Device_Addr << 1);

        recVal = IIC_RecAck(&BL24C02_bus);
        IIC_Stop(&BL24C02_bus);
    }while(recVal != 0);

}

void BL24C02_BufferWrite(uint8_t address, uint16_t numOfwrite, uint8_t *writeBuff)
{
    uint8_t numOfPage = numOfwrite / PAGE_WRITE;
    uint8_t numOfModulo = numOfwrite % PAGE_WRITE;
    uint8_t addr = numOfwrite % PAGE_WRITE;
    uint8_t count = PAGE_WRITE - addr;
    uint8_t temp;
    if(0 == addr)
    {
        if(numOfPage > 0)
        {
            while(numOfPage--)
            {
                BL24C02_Write(address, PAGE_WRITE, writeBuff);
                BL24C02_AcknowledgePolling();
                address += PAGE_WRITE;
                writeBuff += PAGE_WRITE;
            }
            if(numOfModulo != 0)
            {
                BL24C02_Write(address, numOfModulo, writeBuff);
                BL24C02_AcknowledgePolling();
            }
        }
        else
        {
            BL24C02_Write(address, numOfModulo, writeBuff);
            BL24C02_AcknowledgePolling();
        }
    }
    else
    {
        if(numOfPage > 0)
        {
            numOfwrite -= count;
            numOfPage = numOfwrite / PAGE_WRITE;  
            numOfModulo = numOfwrite % PAGE_WRITE;
            BL24C02_Write(address, count, writeBuff);
            BL24C02_AcknowledgePolling();
            address += count;
            writeBuff += count;
            while(numOfPage--)
            {
                BL24C02_Write(address, PAGE_WRITE, writeBuff);
                BL24C02_AcknowledgePolling();
                address += PAGE_WRITE;
                writeBuff += PAGE_WRITE;
            }
            if(numOfModulo != 0)
            {
                BL24C02_Write(address, numOfModulo, writeBuff);
                BL24C02_AcknowledgePolling();
            }
        }
        else
        {
            if(count > numOfModulo)
            {
                BL24C02_Write(address, numOfModulo, writeBuff);
                BL24C02_AcknowledgePolling();
            }
            else
            {
                temp = numOfModulo - count;
                BL24C02_Write(address, count, writeBuff);
                BL24C02_AcknowledgePolling();
                address += count;
                writeBuff += count;

                BL24C02_Write(address, temp, writeBuff);
                BL24C02_AcknowledgePolling();
            }
        }
    }


}


void BL24C02_Init(void)
{
    BL24C02_CLKEN;
    IIC_Init(&BL24C02_bus);

}
