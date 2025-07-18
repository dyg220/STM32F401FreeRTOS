#include "main.h"


u8 Key_PassWord[8]={'1','2','3','4'};


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
	W25Q64_Init();
	W25Q64_Zk_Updata();   //×Ö¿âÉÕÂ¼
	LCD_Init();
	Voice_Init();
	Voice_SendCmd(0xef);
	printf("Reset!\r\n");
	
//	LCD_ShowImage(0,0,240,240,gImage_pic);
	LCD_ShowString_FromFlash(0,0,RED,WHITE,LCD_32X32,"Hello!¹þ¹þ");
	AppStartTask();//ÈÎÎñ
	
	return 0;
}





