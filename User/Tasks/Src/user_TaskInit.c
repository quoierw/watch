#include "../Inc/user_TaskInit.h"
#include "../../../LVGL/lvgl/src/hal/lv_hal_tick.h"
#include "../../../LVGL/lvgl/src/porting/lv_port_disp.h"
#include "../../../LVGL/lvgl/src/porting/lv_port_indev.h"
#include "../../../LVGL/lvgl/lvgl.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os.h"


void LvHandlerTask(void *argument);


osThreadId_t LvHandlerTaskHandle;
const osThreadAttr_t LvHandlerTaskHandle_attributes = {
  .name = "LvHandler",
  .stack_size = 128 * 30,
  .priority = (osPriority_t) osPriorityLow,
};

void UserTaskInit(void)
{
    LvHandlerTaskHandle = osThreadNew(LvHandlerTask, NULL, &LvHandlerTaskHandle_attributes);
}

void LvHandlerTask(void *argument)
{
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

    while(1)
    {
        uint32_t wait = lv_timer_handler();
        if(wait < 1U) wait = 1U; 
        if(wait > 30U) wait = 30U;
        osDelay(wait);
    }
    
}


void TaskTickHook(void)
{
    lv_tick_inc(1);
}
