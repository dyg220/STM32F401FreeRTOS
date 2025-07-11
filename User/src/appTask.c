#include "appTask.h"

BaseType_t ret;
TaskHandle_t Task1_Handle_t;  // 任务1句柄
TaskHandle_t Task2_Handle_t;  // 任务2句柄
TaskHandle_t Task3_Handle_t;  // 任务2句柄
void LED_Task(void* pvParameters); // 任务1函数
void Task2(void* pvParameters); // 任务1函数
void Task3(void* pvParameters); // 任务1函数
QueueHandle_t queue; // 队列句柄

void AppStartTask(void)
{
	queue = xQueueCreate(64, sizeof(u8)); // 创建一个队列，长度为10，每个元素大小为uint8_t

	taskENTER_CRITICAL();	 // 进入临界区(关中断)
	xTaskCreate(LED_Task, "LED_Task", 128, NULL, 3, &Task1_Handle_t); //任务创建出来让任务到就绪态
	xTaskCreate(Task2, "Task2", 128, NULL, 3, &Task2_Handle_t); //任务创建出来让任务到就绪态
	xTaskCreate(Task3, "Task3", 128, NULL, 3, &Task3_Handle_t); //任务创建出来让任务到就绪态

	taskEXIT_CRITICAL(); // 退出临界区
	vTaskStartScheduler(); //开启任务调度 
}

void LED_Task(void* pvParameters)
{
	LED_Init();
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
	Key_Init();
	while (1) {
	}

}
void Task3(void* pvParameters)
{

	dcMotor_Config();
	while (1) {
		DCMotor_SetSpeed(0, 100);
		delay_ms(300);
		DCMotor_SetSpeed(0, 0);
	}
}

