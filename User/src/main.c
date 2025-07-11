#include "main.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组
	delay_init(84);
	USART1_Config(115200);
	printf("Reset!\r\n");
	LED_Init();
	Key_Init();
	//AppStartTask();
	return 0;
}
