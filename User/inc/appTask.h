#ifndef _APPTASK_H_
#define _APPTASK_H_

#include "stm32f4xx.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "usart1.h"
#include "queue.h"
#include "systick.h"
#include "led.h"
#include "key.h"
#include "door.h"
#include "voice.h"
#include "mg200.h"
#include "rfid.h"
#include "at24c02.h"
#include "cy8cmbr3116.h"

void AppStartTask(void);

#endif

