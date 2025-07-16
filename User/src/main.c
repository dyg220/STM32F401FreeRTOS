#include "main.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(84);
	LED_Init();
	Key_Init();
	Door_Init();
	USART1_Config(115200);
	MG200_Init();
	RFID_Init();
	AT24C02_Init();
	CY8CMBR3116_Init();
	Voice_Init();
	Voice_SendCmd(0xef);

	printf("Reset!\r\n");

	AppStartTask();//хннЯ

	return 0;
}





