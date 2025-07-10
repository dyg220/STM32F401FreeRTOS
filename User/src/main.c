#include "main.h"

TaskHandle_t xTask1Handle = NULL;
void Task1(void* pvParameters); // 任务函数声明

int main()
{
	BaseType_t ret;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组
	USART1_Config(115200);
	printf("Reset!\r\n");

	ret = xTaskCreate(Task1, "Task1", 128, NULL, 3, &xTask1Handle); //任务创建出来让任务到就绪态
	vTaskStartScheduler(); //开启任务调度 

	return 0;
}
void Task1(void* pvParameters)
{
	while (1)
	{
		printf("Task1 Run \r\n");
		vTaskDelay(500);
	}
}
