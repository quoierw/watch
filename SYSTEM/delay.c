#include "delay.h"

void delay_init(void)
{
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    HAL_SYSTICK_Config(SystemCoreClock / (1000U /uwTickFreq));
}

void delay_us(uint32_t xus)
{
    uint32_t ticks; 
    uint32_t told, tnew;
    uint32_t tcnt = 0;
    uint32_t reload = SysTick->LOAD;
    //这里设置*100是因为这个是systemCoreClock为100MHZ
    ticks = xus * 100;
    told = SysTick->VAL;
    while(1)
    {
        tnew = SysTick->VAL;
        if(told != tnew)
        {
            if(tnew < told)
            {
                tcnt += told - tnew;
            }
            else
            {
                tcnt += (reload - tnew + told);
            }
            told = tnew;
            if(tcnt >= ticks)
            {
                break;
            }
        }

    }
}

void delay_ms(uint32_t xms)
{
    uint32_t i = 0;
    for(i = 0; i < xms; i++)
    {
        delay_us(1000);
    }
}