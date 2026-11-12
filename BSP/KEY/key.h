#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx_hal.h"
#define KEY1    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)
#define KEY2    HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)

void key_init(void);
uint8_t key_scan(uint8_t mode);



#endif
