#include "appTask.h"
#include <stddef.h>

BaseType_t ret;
TaskHandle_t Task1_Handle_t;       // 任务1句柄
TaskHandle_t Task2_Handle_t;       // 任务2句柄
TaskHandle_t Task3_Handle_t;       // 任务2句柄

void Task1(void* pvParameters); 	// 任务1函数
void Task2(void* pvParameters);    // 任务1函数
void Task3(void* pvParameters);    // 任务1函数

QueueHandle_t queue;               // 队列句柄

u8 RFID_ID1[4] = { 0 };

void AppStartTask(void)
{
	queue = xQueueCreate(64, sizeof(u8)); // 创建一个队列，长度为10，每个元素大小为uint8_t

	taskENTER_CRITICAL();                                             // 进入临界区(关中断)
	xTaskCreate(Task1, "Task1", 128, NULL, 3, &Task1_Handle_t);		  // 任务创建出来让任务到就绪态
	xTaskCreate(Task2, "Task2", 128, NULL, 3, &Task2_Handle_t);       // 任务创建出来让任务到就绪态
	xTaskCreate(Task3, "Task3", 128, NULL, 3, &Task3_Handle_t);       // 任务创建出来让任务到就绪态

	taskEXIT_CRITICAL();   // 退出临界区
	vTaskStartScheduler(); // 开启任务调度
}

void Task1(void* pvParameters)
{
	//u8 PCD_ID[4] = { 0 };
	while (1)
	{
		//PCD_distinguish_PICC(PCD_ID);
	}
}

void Task2(void* pvParameters)
{
	/*u8 i = 0;
	u8 key_num = 0;
	while (1)
	{
		key_num = Key_Scan();
		if (key_num == key1_click)
		{
			Door_Open();
		}
		if (key_num == key1_long)
		{
			Door_Close();
		}
		if (key_num == 2)
		{
			printf("%#x\r\n", 0x00 + i);
			Voice_SendCmd(0x00 + i);
			i++;
		}
	}*/

	u8 i = 0, keynum = 0;
	while (1)
	{
		keynum = Key_Scan();
		if (keynum == 2)
		{
			i++;
			if (i > 2) i = 1;
			printf("i = %d\r\n", i);
		}
		if (i == 1)
		{
			RFID_Register(RFID_ID1);//卡片注册
		}
		else if (i == 2)
		{
			RFID_Open_Door();//卡片开门
		}
	}
}

void Task3(void* pvParameters)
{
	while (1)
	{
		u8 id = 0, ret = 0;
		while (1)
		{
			//ret = Enroll(&id);
			//ret = Match1N(&id);
			//if (ret == 0)printf("%d\r\n", id);
			//delay_ms(500);
		}
	}
}
