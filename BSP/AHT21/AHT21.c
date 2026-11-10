#include "AHT21.h"
#include "../../SYSTEM/delay.h"

iic_bus_t AHT21_bus = {
    .IIC_SCL_Pin = GPIO_PIN_14,
    .IIC_SDA_Pin = GPIO_PIN_13,
    .IIC_SCL_PORT = GPIOB,
    .IIC_SDA_PORT = GPIOB,
};

uint8_t AHT21_IIC_ReadStatus(uint8_t addr, uint8_t *recVal)
{
    IIC_Start(&AHT21_bus);
    IIC_SendByte(&AHT21_bus, (addr << 1) | 0x01);
    if(IIC_RecAck(&AHT21_bus))
    {
        IIC_Stop(&AHT21_bus);
        return 1;
    }
    *recVal = IIC_RecByte(&AHT21_bus);
    IIC_SendAck(&AHT21_bus, 1);
    IIC_Stop(&AHT21_bus);
    return 0;
}

static uint8_t AHT21_ReadMultiByte(iic_bus_t *bus, uint8_t addr, uint8_t length, uint8_t *recBuff)
{
    uint8_t i = 0;

    IIC_Start(bus);
    IIC_SendByte(bus, (addr << 1) | 0x01);
    if(IIC_RecAck(bus))
    {
        IIC_Stop(bus);
        return 1;
    }
    for(i = 0; i < length; i++)
    {
        recBuff[i] = IIC_RecByte(bus);
        if(i < length - 1)
            IIC_SendAck(bus, 0);
    }
    IIC_SendAck(bus, 1);
    IIC_Stop(bus);
    return 0;
}


void AHT21_Init(void)
{
    uint8_t recVal;
    uint8_t tempDATA[] = {0x08, 0x00};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    IIC_Init(&AHT21_bus);
    delay_ms(40);

    AHT21_IIC_ReadStatus(AHT21_Device_Addr, &recVal);

    if((recVal & 0x08) != 0x08)
    {
      IIC_WriteMultiOneByte(&AHT21_bus, AHT21_Device_Addr, 0XBE, 2, tempDATA);
      delay_ms(10);
    }
}

uint8_t AHT21_Read(float *humi, float *temp)
{
    uint8_t tempDATA[] = {0x33, 0x00};
    uint8_t recArry[6] = {0};
    uint32_t data = 0;
    IIC_WriteMultiOneByte(&AHT21_bus, AHT21_Device_Addr, 0xAC, 2, tempDATA);

    delay_ms(80);

    AHT21_ReadMultiByte(&AHT21_bus, AHT21_Device_Addr, 6, recArry);
    data |= (uint32_t)recArry[1] << 12;
    data |= (uint32_t)recArry[2] << 4;
    data |= (uint32_t)recArry[3] >> 4;
    *humi = data * 100.0f / (1<<20) ;

    data = 0;
    data |= (uint32_t)(recArry[3] & 0x0F) << 16;
    data |= (uint32_t)recArry[4] << 8;
    data |= (uint32_t)recArry[5];
    *temp = (data * 200.0f / (1<<20)) - 50;
    
    
}
