#include "iic_hal.h"
#include "../../SYSTEM/delay.h"

void IIC_Init(iic_bus_t *bus)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = bus->IIC_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(bus->IIC_SDA_PORT, &GPIO_InitStruct);
		
	GPIO_InitStruct.Pin = bus->IIC_SCL_Pin ;
    HAL_GPIO_Init(bus->IIC_SCL_PORT, &GPIO_InitStruct);

}

void IIC_SCL_W(iic_bus_t *bus, uint8_t value)
{
    if(value)
    {
        bus->IIC_SCL_PORT->BSRR |= bus->IIC_SCL_Pin;
    }
    else
    {
        bus->IIC_SCL_PORT->BSRR = (uint32_t)(bus->IIC_SCL_Pin << 16U);
    }
    delay_us(1);
}

void IIC_SDA_W(iic_bus_t *bus, uint8_t value)
{
    if(value)
    {
        bus->IIC_SDA_PORT->BSRR |= bus->IIC_SDA_Pin;
    }
    else
    {
        bus->IIC_SDA_PORT->BSRR = (uint32_t)(bus->IIC_SDA_Pin << 16U);
    }
    delay_us(1);
}

uint8_t IIC_SDA_R(iic_bus_t *bus)
{
    uint8_t temp;
    temp =  (bus->IIC_SDA_PORT->IDR & bus->IIC_SDA_Pin) ? 1 : 0;
    delay_us(1);
    return temp;
}

void IIC_Start(iic_bus_t *bus)
{
    IIC_SDA_W(bus, 1);
    IIC_SCL_W(bus, 1);
    IIC_SDA_W(bus, 0);
    IIC_SCL_W(bus, 0);
}

void IIC_Stop(iic_bus_t *bus)
{
    IIC_SDA_W(bus, 0);
    IIC_SCL_W(bus, 1);
    IIC_SDA_W(bus, 1);
}

void IIC_SendByte(iic_bus_t *bus, uint8_t data)
{
    uint8_t i = 0;
    for(i = 0; i < 8; i++)
    {
        IIC_SDA_W(bus, data & (0x80 >> i));
        IIC_SCL_W(bus, 1);
        IIC_SCL_W(bus, 0);
    }
}

uint8_t IIC_RecByte(iic_bus_t *bus)
{
    uint8_t tempVal = 0;
    uint8_t i = 0;
    IIC_SDA_W(bus, 1);
    for(i = 0; i < 8; i++)
    {
        IIC_SCL_W(bus, 1);
        if(IIC_SDA_R(bus))
        {
            tempVal |= (0x80 >> i);
        }
        IIC_SCL_W(bus, 0);
    }
    return tempVal;
}

void IIC_SendAck(iic_bus_t *bus, uint8_t data)
{
    IIC_SDA_W(bus, data);
    IIC_SCL_W(bus, 1);
    IIC_SCL_W(bus, 0);
}

uint8_t IIC_RecAck(iic_bus_t *bus)
{
    uint8_t tempVal = 0;
    IIC_SDA_W(bus, 1);
    IIC_SCL_W(bus, 1);
    tempVal = IIC_SDA_R(bus);
    IIC_SCL_W(bus, 0);
    return tempVal;
}


uint8_t IIC_WriteOneByte(iic_bus_t *bus, uint8_t addr, uint8_t reg, uint8_t data)
{
    IIC_Start(bus);

    IIC_SendByte(bus, addr << 1);

    if(IIC_RecAck(bus))
    {
        IIC_Stop(bus);
        return 1;
    }
    IIC_SendByte(bus, reg);
    IIC_RecAck(bus);

    IIC_SendByte(bus, data);
    IIC_RecAck(bus);

    IIC_Stop(bus);
    return 0;

}

uint8_t IIC_WriteMultiOneByte(iic_bus_t *bus, uint8_t addr, uint8_t reg, uint8_t length, uint8_t data[])
{
    uint8_t i = 0;
    IIC_Start(bus);

    IIC_SendByte(bus, addr << 1);

    if(IIC_RecAck(bus))
    {
        IIC_Stop(bus);
        return 1;
    }
    IIC_SendByte(bus, reg);
    IIC_RecAck(bus);
    for(i = 0; i < length; i++)
    {
        IIC_SendByte(bus, data[i]);
        IIC_RecAck(bus);
    }

    IIC_Stop(bus);
    return 0;

}

uint8_t IIC_ReadOneByte(iic_bus_t *bus, uint8_t addr, uint8_t reg, uint8_t *recVal)
{
    IIC_Start(bus);
    IIC_SendByte(bus, addr << 1);

    if(IIC_RecAck(bus))
    {
        IIC_Stop(bus);
        return 1;
    }
    IIC_SendByte(bus, reg);
    IIC_RecAck(bus);

    IIC_Start(bus);
    IIC_SendByte(bus, (addr << 1) | 0x01);
    if(IIC_RecAck(bus))
    {
        IIC_Stop(bus);
        return 1;
    }
    *recVal = IIC_RecByte(bus);
    IIC_SendAck(bus, 1);
    IIC_Stop(bus);
    return 0;
}

uint8_t IIC_ReadMultiByte(iic_bus_t *bus, uint8_t addr, uint8_t reg, uint8_t length, uint8_t *recBuff)
{
    uint8_t i = 0;
    IIC_Start(bus);
    IIC_SendByte(bus, addr << 1);

    if(IIC_RecAck(bus))
    {
        IIC_Stop(bus);
        return 1;
    }
    IIC_SendByte(bus, reg);
    IIC_RecAck(bus);

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