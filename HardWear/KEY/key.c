#include "key.h"
#include "systick.h"
#include "led.h"

u8 Key_Num = 0;

void Key_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// Configure GPIOB Pin 11 as output push-pull
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_0, // Key1
		.GPIO_Mode = GPIO_Mode_IN,  // Output mode£∫  ‰»Î
		.GPIO_PuPd = GPIO_PuPd_NOPULL // ∏°ø’
		//.GPIO_Speed = GPIO_Speed_2MHz     // Œﬁ–Ë≈‰÷√
	};

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
u8 Key_Scan(void)
{
#if  Key_Mode
	if (Key_Num)
	{
		u8 Temp = 0;
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
#else
	u16 time_cnt = 0;
	u8 key_num = 0;
	if (Key1_Press)
	{
		delay_ms(20);
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
#endif
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
