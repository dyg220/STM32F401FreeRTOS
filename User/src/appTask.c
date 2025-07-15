#include "appTask.h"
#include <stddef.h>

BaseType_t ret;
TaskHandle_t Task1_Handle_t;       // ����1���
TaskHandle_t Task2_Handle_t;       // ����2���
TaskHandle_t Task3_Handle_t;       // ����2���

void Task1(void* pvParameters); 	// ����1����
void Task2(void* pvParameters);    // ����1����
void Task3(void* pvParameters);    // ����1����

QueueHandle_t queue;               // ���о��

u8 RFID_ID1[4] = { 0 };

void AppStartTask(void)
{
	queue = xQueueCreate(64, sizeof(u8)); // ����һ�����У�����Ϊ10��ÿ��Ԫ�ش�СΪuint8_t

	taskENTER_CRITICAL();                                             // �����ٽ���(���ж�)
	xTaskCreate(Task1, "Task1", 128, NULL, 3, &Task1_Handle_t);		  // ���񴴽����������񵽾���̬
	xTaskCreate(Task2, "Task2", 128, NULL, 3, &Task2_Handle_t);       // ���񴴽����������񵽾���̬
	xTaskCreate(Task3, "Task3", 128, NULL, 3, &Task3_Handle_t);       // ���񴴽����������񵽾���̬

	taskEXIT_CRITICAL();   // �˳��ٽ���
	vTaskStartScheduler(); // �����������
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
			RFID_Register(RFID_ID1);//��Ƭע��
		}
		else if (i == 2)
		{
			RFID_Open_Door();//��Ƭ����
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
