#include "key.h"

/*************************************************
 * 函 数 名：Key_Init
 * 函数功能：按键初始化
 * 参    数：None
 * 返 回 值：None
 * 备    注：KEY1----PA0   KEY2---PC13
 * **************************************************/
void Key_Init(void)
{

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}


uint8_t Key_Scan(void)
{
    u8 key_num = 0;
	if(Key1_Press)
	{
		delay_ms(30);
		if(Key1_Press)
		{
			while(Key1_Press);
            key_num = 1;
		}
	}
	if(Key2_Press)
	{
		delay_ms(30);
		if(Key2_Press)
		{
			while(Key2_Press);
            key_num = 2;
		}
	}
	return key_num;
}



