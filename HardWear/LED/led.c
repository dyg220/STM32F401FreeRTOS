#include "led.h"

void LED_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9,
		.GPIO_Mode = GPIO_Mode_OUT, // Output mode
		.GPIO_OType = GPIO_OType_PP, // Push-pull output
		.GPIO_Speed = GPIO_Speed_50MHz  // Speed 50MHz
	};

	GPIO_Init(GPIOB, &GPIO_InitStructure);

	LEDALL_OFF();
}

//ȫ��
void LEDALL_ON()
{
	LED1_ON;
	LED2_ON;
}

//ȫ��
void LEDALL_OFF()
{
	LED1_OFF;
	LED2_OFF;
}

//LED1 ��ת
void LED1_FZ()
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8) == Bit_RESET) //��ȡLED1״̬
	{
		LED1_OFF;
	}
	else
	{
		LED1_ON;
	}
}

void LED2_FZ()
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9) == Bit_RESET) //��ȡLED1״̬
	{
		LED2_OFF;
	}
	else
	{
		LED2_ON;
	}
}

