#ifndef __USER_TASKINIT_H
#define __USER_TASKINIT_H

#include "cmsis_os2.h"   

extern osMessageQueueId_t externFlagQID;


void TaskTickHook(void);
void UserTaskInit(void);



#endif
