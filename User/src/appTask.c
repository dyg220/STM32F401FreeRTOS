#include "appTask.h"

BaseType_t ret;
TaskHandle_t Task1_Handle_t;  // ����1���
TaskHandle_t Task2_Handle_t;  // ����2���
void LED_Task(void* pvParameters); // ����1����
void Task2(void* pvParameters); // ����1����
QueueHandle_t queue; // ���о��

void AppStartTask(void)
{
	queue = xQueueCreate(64, sizeof(u8)); // ����һ�����У�����Ϊ10��ÿ��Ԫ�ش�СΪuint8_t

	taskENTER_CRITICAL();	 // �����ٽ���(���ж�)
	xTaskCreate(LED_Task, "LED_Task", 128, NULL, 4, &Task1_Handle_t); //���񴴽����������񵽾���̬
	xTaskCreate(Task2, "Task2", 128, NULL, 3, &Task2_Handle_t); //���񴴽����������񵽾���̬


	taskEXIT_CRITICAL(); // �˳��ٽ���
	vTaskStartScheduler(); //����������� 
}

void LED_Task(void* pvParameters)
{
	while (1)
	{
		LED1_ON;
		delay_ms(500);
		LED1_OFF;
		delay_ms(500);
	}
}
void Task2(void* pvParameters)
{
	u8 i = 0;
	u8 data[5] = { 0 };
	while (1)
	{
		for (i = 0; i < 5; i++)
		{
			xQueueReceive(queue, data + i, portMAX_DELAY);
		}
		for (i = 0; i < 5; i++)
		{
			printf("%d \r\n", data[i]);
		}
		printf("Task2 Run \r\n");
		//vTaskDelay(500);//��������״̬
		delay_ms(500);
	}
}

