#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"

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
//    /*
//     * Keep the delay_ms() API, but don't busy-wait on the SysTick counter
//     * after FreeRTOS has started. SysTick belongs to the RTOS and can wrap
//     * several times while this task is preempted, which makes the original
//     * polling delay inaccurate and keeps the CPU unnecessarily busy.
//     */
//    if((__get_IPSR() == 0U) &&
//       (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING))
//    {
//        TickType_t ticks = pdMS_TO_TICKS(xms);

//        if((xms > 0U) && (ticks == 0U))
//        {
//            ticks = 1U;
//        }

//        vTaskDelay(ticks);
//        return;
//    }

    /* Before the scheduler starts, retain the original blocking delay. */
    uint32_t i = 0;
    for(i = 0; i < xms; i++)
    {
        delay_us(1000);
    }
}
