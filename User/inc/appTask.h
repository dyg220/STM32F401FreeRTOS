#ifndef __APP_TASK_H__
#define __APP_TASK_H__
#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "systick.h"
#include "led.h"
#include "key.h"
#include "door.h"
#include "voice.h"
#include "mg200.h"
#include "rfid.h"

void AppStartTask(void);

#endif

