#include "key.h"
#include "cmsis_os2.h"   

void key_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  

    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);  

    HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    
}

uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;
    static uint8_t key_down = 0;
    uint8_t retVal = 0;
    if(mode)
    {
        key_up = 1;
        key_down = 0;
    }

    if(key_up && ( !KEY1 || KEY2))
    {
	    osDelay(3);
        if(!KEY1)
            key_down = 1;
        if(KEY2)
            key_down = 2;
        if(key_down)
            key_up = 0;
    }

    if(key_down && (KEY1 && !KEY2))
    {
        osDelay(3);
        if(KEY1 && !KEY2)
        {
            retVal = key_down;
            key_up = 1;
            key_down = 0;
        }
    }
    return retVal;
}