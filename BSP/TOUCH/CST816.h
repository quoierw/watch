#ifndef __CST816_H
#define __CST816_H

#include "stm32f4xx_hal.h"
#include "../IIC/iic_hal.h"
#define TOUCH_RST_PIN GPIO_PIN_15
#define TOUCH_RST_PORT GPIOA

//TOUCH_INT_PIN is unconnect to MCU,PULL_UP 3.3V.
// #define TOUCH_INT_PIN GPIO_PIN_1
// #define TOUCH_INT_PORT GPIOB

#define TOUCH_RST(x)    HAL_GPIO_WritePin(TOUCH_RST_PORT, TOUCH_RST_PIN, ((x) ? GPIO_PIN_SET: GPIO_PIN_RESET))

/* 设备地址 */
#define Device_Addr                   0x15

/* 触摸屏寄存器 */
#define GestureID                     0x01
#define FingerNum                     0x02
#define XposH                         0x03
#define XposL                         0x04
#define YposH                         0x05
#define YposL                         0x06
#define ChipID                        0xA7
#define SleepMode                     0xE5
#define MotionMask                    0xEC
#define IrqPluseWidth                 0xED
#define NorScanPer                    0xEE
#define MotionSlAngle                 0xEF
#define LpAutoWakeTime                0xF4
#define LpScanTH                      0xF5
#define LpScanWin                     0xF6
#define LpScanFreq                    0xF7
#define LpScanIdac                    0xF8
#define AutoSleepTime                 0xF9
#define IrqCtl                        0xFA
#define AutoReset                     0xFB
#define LongPressTime                 0xFC
#define IOCtl                         0xFD
#define DisAutoSleep                  0xFE

typedef struct 
{
    uint16_t x;
    uint16_t y;
}CST816_InfoTypeDef;

typedef enum
{
	NOGESTURE = 	0x00,
	DOWNGLIDE = 	0x01,
	UPGLIDE = 		0x02,
	LEFTGLIDE = 	0x03,
	RIGHTGLIDE = 	0x04,
	CLICK = 			0x05,
	DOUBLECLICK = 0x0B,
	LONGPRESS = 	0x0C,
} GestureID_TypeDef;

/* 连续动作配置选项 */
typedef enum
{
	M_DISABLE = 	0x00,
	EnConLR = 		0x01,
	EnConUD = 		0x02,
	EnDClick = 		0x03,
	M_ALLENABLE = 0x07,
} MotionMask_TypeDef;

/* 中断低脉冲发射方式选项 */
typedef enum
{
	OnceWLP = 		0x00,
	EnMotion = 		0x10,
	EnChange = 		0x20,
	EnTouch = 		0x40,
	EnTest = 			0x80,
} IrqCtl_TypeDef;


extern CST816_InfoTypeDef CST816_Instance;




void CST816_GPIO_Init(void);

void CST816_RESET(void);

void CST816_Init(void);


uint8_t CST816_IIC_ReadReg(uint8_t addr, uint8_t *recVal);

uint8_t CST816_IIC_WriteReg(uint8_t addr ,uint8_t data);


uint8_t CST816_Get_XY_AXIS(void);

uint8_t CST816_GetFingerNum(uint8_t *recVAL);

uint8_t CST816_GetChipID(uint8_t *recVAL);



uint8_t CST816_SetAutoSleepTime(uint8_t time);

uint8_t CST816_SetMotionMask(uint8_t mode);

uint8_t CST816_SetMotionS1Angle(uint8_t x_right_y_up_angle);

uint8_t CST816_SetNorScanPer(uint8_t period);

uint8_t CST816_SetIrqPulseWidth(uint8_t width);

uint8_t CST816_SetLpScanTh(uint8_t TH);

uint8_t CST816_Sleep(void);

void CST816_WakeUp(void);


#endif
