#include "appTask.h"
#include <stddef.h>

BaseType_t ret;
TaskHandle_t Task1_Handle_t;       // ����1���
TaskHandle_t Task2_Handle_t;       // ����2���
TaskHandle_t Task3_Handle_t;       // ����2���
void LED_Task(void* pvParameters); // ����1����
void Task2(void* pvParameters);    // ����1����
void Task3(void* pvParameters);    // ����1����
QueueHandle_t queue;               // ���о��

void AppStartTask(void)
{
	queue = xQueueCreate(64, sizeof(u8)); // ����һ�����У�����Ϊ10��ÿ��Ԫ�ش�СΪuint8_t

	taskENTER_CRITICAL();                                             // �����ٽ���(���ж�)
	xTaskCreate(LED_Task, "LED_Task", 128, NULL, 3, &Task1_Handle_t); // ���񴴽����������񵽾���̬
	xTaskCreate(Task2, "Task2", 128, NULL, 3, &Task2_Handle_t);       // ���񴴽����������񵽾���̬
	xTaskCreate(Task3, "Task3", 128, NULL, 3, &Task3_Handle_t);       // ���񴴽����������񵽾���̬

	taskEXIT_CRITICAL();   // �˳��ٽ���
	vTaskStartScheduler(); // �����������
}

void LED_Task(void* pvParameters)
{
	while (1)
	{
		LEDB_ON;
		delay_ms(500);
		LEDB_OFF;
		delay_ms(500);
		LEDR_ON;
		delay_ms(500);
		LEDR_OFF;
		delay_ms(500);
	}
}
void Task2(void* pvParameters)
{
	u8 i = 0;
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
	}
}
void Task3(void* pvParameters)
{
	while (1)
	{
		u8 id = 0, ret = 0;
		while (1)
		{
			ret = Enroll(&id);
			//ret = Match1N(&id);
			if (ret == 0)printf("%d\r\n", id);
			delay_ms(500);
		}
	}
}
