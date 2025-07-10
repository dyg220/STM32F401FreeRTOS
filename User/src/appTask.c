#include "appTask.h"

BaseType_t ret;
TaskHandle_t Task1_Handle_t;  // 任务1句柄
TaskHandle_t Task2_Handle_t;  // 任务2句柄
void Task1(void* pvParameters); // 任务1函数
void Task2(void* pvParameters); // 任务1函数
QueueHandle_t queue; // 队列句柄

void AppStartTask(void)
{
	queue = xQueueCreate(64, sizeof(u8)); // 创建一个队列，长度为10，每个元素大小为uint8_t

	taskENTER_CRITICAL();	 // 进入临界区(关中断)
	xTaskCreate(Task1, "Task1", 128, NULL, 4, &Task1_Handle_t); //任务创建出来让任务到就绪态
	xTaskCreate(Task2, "Task2", 128, NULL, 3, &Task2_Handle_t); //任务创建出来让任务到就绪态


	taskEXIT_CRITICAL(); // 退出临界区
	vTaskStartScheduler(); //开启任务调度 
}

void Task1(void* pvParameters)
{
	u8 data[5] = { 1,2,3,4,5 };
	int i = 0;
	while (1)
	{
		for (i = 0; i < 5; i++)
		{
			xQueueSend(queue, data + i, 0); // 向队列发送数据
		}
		//printf("Task1 Run \r\n");
		//vTaskDelay(500); //进入阻塞状态
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
		//vTaskDelay(500);//进入阻塞状态
		delay_ms(500);
	}
}

