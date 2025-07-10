#include "main.h"

TaskHandle_t xTask1Handle = NULL;
void Task1(void* pvParameters); // ����������

int main()
{
	BaseType_t ret;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����
	USART1_Config(115200);
	printf("Reset!\r\n");

	ret = xTaskCreate(Task1, "Task1", 128, NULL, 3, &xTask1Handle); //���񴴽����������񵽾���̬
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
