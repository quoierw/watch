#include "../Inc/user_TaskInit.h"
#include "../../../LVGL/lvgl/src/hal/lv_hal_tick.h"
#include "../../../LVGL/lvgl/src/porting/lv_port_disp.h"
#include "../../../LVGL/lvgl/src/porting/lv_port_indev.h"
#include "../../../LVGL/lvgl/lvgl.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os.h"
#include "../../BSP/AHT21/AHT21.h"
#include "string.h"
#include "stdio.h"
void LvHandlerTask(void *argument);
void AHT21HandlerTask(void *argument);


osThreadId_t LvHandlerTaskHandle;
const osThreadAttr_t LvHandlerTask_attributes = {
  .name = "LvHandler",
  .stack_size = 128 * 30,
  .priority = (osPriority_t) osPriorityLow,
};

osThreadId_t AHT21HTaskHandle;
const osThreadAttr_t AHT21Task_attributes = {
  .name = "AHT21Handler",
  .stack_size = 128 * 10,
  .priority = (osPriority_t) osPriorityLow1,
};

osMessageQueueId_t aht211QID;


void UserTaskInit(void)
{
    aht211QID = osMessageQueueNew(1, 2 *sizeof(float), NULL);
    LvHandlerTaskHandle = osThreadNew(LvHandlerTask, NULL, &LvHandlerTask_attributes);
    AHT21HTaskHandle = osThreadNew(AHT21HandlerTask, NULL, &AHT21Task_attributes);
}

void LvHandlerTask(void *argument)
{
    uint8_t i = 0;
	float HumiTemp[2] = {0};
    char str[30] = {0};
	uint32_t wait;
	lv_init();
	lv_port_disp_init();
    lv_port_indev_init();
	
    lv_obj_t * scr = lv_scr_act();
    lv_obj_t * btn = lv_btn_create(scr);
    lv_obj_set_size(btn, 50, 50);
    lv_obj_set_align(btn, LV_ALIGN_CENTER);

    lv_obj_t* label1 = lv_label_create(btn);
    lv_label_set_text(label1, "ok");

	lv_obj_set_align(label1, LV_ALIGN_CENTER);
    lv_obj_t * slider = lv_slider_create(scr);
    lv_obj_set_size(slider, 100, 5);
    lv_obj_set_pos(slider, 50, 50);
	
    lv_obj_t* label2 = lv_label_create(scr);
    lv_obj_set_size(label2, 100, 50);
    lv_obj_set_pos(label2, 0, 200); 
	
    while(1)
    {
		if(osOK == osMessageQueueGet(aht211QID, HumiTemp, 0, 0))
		{
            sprintf(str, "humi:%.2f, temp:%.2f", HumiTemp[0], HumiTemp[1]);
			lv_label_set_text(label2, str);
		}
        wait = lv_timer_handler();
        if(wait < 1U) wait = 1U; 
        if(wait > 30U) wait = 30U;
        osDelay(wait);
    }
    
}

void AHT21HandlerTask(void *argument)
{
    float HumiTemp[2] = {0};
    while(1)
    {
        AHT21_Read(&HumiTemp[0], &HumiTemp[1]);
        osMessageQueuePut(aht211QID, HumiTemp,0,0);
        osDelay(1000);
    } 
}

void TaskTickHook(void)
{
    lv_tick_inc(1);
}
