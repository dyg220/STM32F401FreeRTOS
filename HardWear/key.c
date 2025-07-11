#include "key.h"
#include "systick.h"
#include "led.h"

u8 Key_Num = 0;

void Key_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = { 0 };

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}
u8 Key_Scan(void)
{
	u16 time_cnt = 0;
	u8 key_num = 0;
	if (Key1_Press)
	{
		delay_ms(30);
		if (Key1_Press)
		{
			while (Key1_Press)
			{
				time_cnt++;
				delay_ms(1);
			}
			if (time_cnt < 500)
				key_num = key1_click;
			else
				key_num = key1_long;
		}
	}
	return key_num;
}

u8 Key_GetState(void)
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
	{
		return key1_click;
	}
	return 0;
}
void Key_Tick(void)
{
	static u32 Count = 0;
	static u8 CurrState = 0, PrevState = 0;

	Count++;
	if (Count >= 20)
	{
		Count = 0;
		PrevState = CurrState;
		CurrState = Key_GetState();
		if (CurrState == 0 && PrevState != 0)
		{
			Key_Num = PrevState;
		}
	}
}
