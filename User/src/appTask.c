#include "appTask.h"

BaseType_t ret;
TaskHandle_t Task1_Handle_t;  // ����1���
TaskHandle_t Task2_Handle_t;  // ����2���
TaskHandle_t Task3_Handle_t;  // ����2���
void LED_Task(void* pvParameters); // ����1����
void Task2(void* pvParameters); // ����1����
void Task3(void* pvParameters); // ����1����
QueueHandle_t queue; // ���о��

void AppStartTask(void)
{
	queue = xQueueCreate(64, sizeof(u8)); // ����һ�����У�����Ϊ10��ÿ��Ԫ�ش�СΪuint8_t

	taskENTER_CRITICAL();	 // �����ٽ���(���ж�)
	xTaskCreate(LED_Task, "LED_Task", 128, NULL, 3, &Task1_Handle_t); //���񴴽����������񵽾���̬
	xTaskCreate(Task2, "Task2", 128, NULL, 3, &Task2_Handle_t); //���񴴽����������񵽾���̬
	xTaskCreate(Task3, "Task3", 128, NULL, 3, &Task3_Handle_t); //���񴴽����������񵽾���̬

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
		LED2_ON;
		delay_ms(500);
		LED2_OFF;
		delay_ms(500);
	}
}
void Task2(void* pvParameters)
{
	u8 key_num = 0;
	while (1) {
		key_num = Key_Scan();
		if (key_num == key1_click)
		{
			printf("a------------------------\r\n");
		}
		if (key_num == key1_long)
		{
			printf("b-----------------\r\n");
		}
	}

}
void Task3(void* pvParameters)
{
	while (1) {
		DCMotor_SetSpeed(100);
		delay_ms(300);
		DCMotor_SetSpeed(0);
	}
}

