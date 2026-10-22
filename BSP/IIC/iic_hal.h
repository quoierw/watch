#ifndef __IIC_HAL_H
#define __IIC_HAL_H

#include "stm32f4xx_hal.h"

typedef struct 
{
    GPIO_TypeDef * IIC_SCL_PORT;
    GPIO_TypeDef * IIC_SDA_PORT;
    uint16_t IIC_SCL_Pin;  
    uint16_t IIC_SDA_Pin;  
}iic_bus_t;


void IIC_Init(iic_bus_t *bus);

void IIC_SCL_W(iic_bus_t *bus, uint8_t value);

void IIC_SDA_W(iic_bus_t *bus, uint8_t value);

uint8_t IIC_SDA_R(iic_bus_t *bus);

void IIC_Start(iic_bus_t *bus);

void IIC_Stop(iic_bus_t *bus);

void IIC_SendByte(iic_bus_t *bus, uint8_t data);

uint8_t IIC_RecByte(iic_bus_t *bus);

void IIC_SendAck(iic_bus_t *bus, uint8_t data);

uint8_t IIC_RecAck(iic_bus_t *bus);


uint8_t IIC_WriteOneByte(iic_bus_t *bus, uint8_t addr, uint8_t reg, uint8_t data);

uint8_t IIC_WriteMultiOneByte(iic_bus_t *bus, uint8_t addr, uint8_t reg, uint8_t length, uint8_t data[]);

uint8_t IIC_ReadOneByte(iic_bus_t *bus, uint8_t addr, uint8_t reg, uint8_t *recVal);

uint8_t IIC_ReadMultiByte(iic_bus_t *bus, uint8_t addr, uint8_t reg, uint8_t length, uint8_t *recBuff);


#endif
