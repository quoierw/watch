#include "CST816.h"

iic_bus_t CST816_bus = {
    .IIC_SCL_Pin = GPIO_PIN_6,
    .IIC_SDA_Pin = GPIO_PIN_4,
    .IIC_SCL_PORT = GPIOB,
    .IIC_SDA_PORT = GPIOB,
};

CST816_InfoTypeDef CST816_Instance;

void CST816_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_WritePin(TOUCH_RST_PORT, TOUCH_RST_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = TOUCH_RST_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_RST_PORT, &GPIO_InitStruct);

    IIC_Init(&CST816_bus);
}

void CST816_RESET(void)
{
    TOUCH_RST(0);
    HAL_Delay(10);
    TOUCH_RST(1);
    HAL_Delay(100);
}

void CST816_Init(void)
{
    CST816_GPIO_Init();
    CST816_SetAutoSleepTime(5);
}

uint8_t CST816_SetAutoSleepTime(uint8_t time)
{
    uint8_t tempVal;
    tempVal = IIC_WriteOneByte(&CST816_bus, Device_Addr, AutoSleepTime, time);
    return tempVal;
}

uint8_t CST816_IIC_ReadReg(uint8_t addr, uint8_t *recVal)
{
    return IIC_ReadOneByte(&CST816_bus, Device_Addr, addr, recVal);
}

uint8_t CST816_IIC_WriteReg(uint8_t addr ,uint8_t data)
{
    uint8_t tempVal;
    tempVal = IIC_WriteOneByte(&CST816_bus, Device_Addr, addr, data);
    return tempVal;
}

uint8_t CST816_Get_XY_AXIS(void)
{
    uint8_t recVal;
    uint8_t Data[4];
    recVal = IIC_ReadMultiByte(&CST816_bus, Device_Addr, XposH, 4, Data);
    CST816_Instance.x = (uint16_t)(((Data[0] & 0x0F) << 8) | Data[1]);
    CST816_Instance.y = (uint16_t)(((Data[2] & 0x0F) << 8) | Data[3]);
    return recVal;
}

uint8_t CST816_GetFingerNum(uint8_t *recVAL)
{
    return CST816_IIC_ReadReg(FingerNum, recVAL);
}

uint8_t CST816_GetChipID(uint8_t *recVAL)
{
    return CST816_IIC_ReadReg(ChipID, recVAL);
}

uint8_t CST816_SetMotionMask(uint8_t mode)
{
    return CST816_IIC_WriteReg(MotionMask, mode);
}

uint8_t CST816_Sleep(void)
{
    return CST816_IIC_WriteReg(SleepMode, 0x03);
}

void CST816_WakeUp(void)
{
    CST816_RESET();
}

uint8_t CST816_SetMotionS1Angle(uint8_t x_right_y_up_angle)
{
    return CST816_IIC_WriteReg(MotionSlAngle, x_right_y_up_angle);
}

uint8_t CST816_SetNorScanPer(uint8_t period)
{
    if(period >= 30)
        period = 30;
    return CST816_IIC_WriteReg(NorScanPer, period);
}

uint8_t CST816_SetIrqPulseWidth(uint8_t width)
{
    if(width >= 200)
        width = 200;
    return CST816_IIC_WriteReg(IrqPluseWidth, width);
}

uint8_t CST816_SetLpScanTh(uint8_t TH)
{
    return CST816_IIC_WriteReg(LpScanTH, TH);
}
