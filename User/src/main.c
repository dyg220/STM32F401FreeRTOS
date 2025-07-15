#include "main.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����
	delay_init(84);
	USART1_Config(115200);

	LED_Init();
	Key_Init();
	Door_Init();
	MG200_Init();
	RFID_Init();
	Voice_Init();
	Voice_SendCmd(0xef);
	printf("Reset!\r\n");
	AppStartTask();
	return 0;
}
