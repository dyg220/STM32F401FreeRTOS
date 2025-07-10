#include "main.h"

int main()
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组
	delay_init(84);
	while (1)
	{
		LED1_FZ();
		delay_ms(500);
		LED2_FZ();
		delay_ms(500);
	}
	USART1_Config(115200);

	printf("Reset!\r\n");
	AppStartTask();


	return 0;
}
