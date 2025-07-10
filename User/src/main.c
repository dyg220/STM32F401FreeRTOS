#include "main.h"

TaskHandle_t Task1_Handle_t;  // ����1���
TaskHandle_t Task2_Handle_t;  // ����2���
void Task1(void* pvParameters); // ����1����
void Task2(void* pvParameters); // ����1����
int main()
{
	BaseType_t ret;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����
	USART1_Config(115200);
	printf("Reset!\r\n");

	ret = xTaskCreate(Task1, "Task1", 128, NULL, 4, &Task1_Handle_t); //���񴴽����������񵽾���̬
	ret = xTaskCreate(Task2, "Task2", 128, NULL, 3, &Task2_Handle_t); //���񴴽����������񵽾���̬
	vTaskStartScheduler(); //����������� 

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
void Task2(void* pvParameters)
{
	while (1)
	{
		printf("Task2 Run \r\n");
		vTaskDelay(500);

	}
}

