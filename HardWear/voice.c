#include "voice.h"

/*****************************************************
 * 函 数 名：Voice_Init
 * 函数功能：语音初始化
 * 参    数：None
 * 返 回 值：None
 * 备    注：DATA---PC5      BUSY---PC4
 * **************************************************/
void Voice_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	VOICE_DATA_H;
	delay_xms(200);
}

/*****************************************************
 * 函 数 名：Voice_Start
 * 函数功能：起始条件
 * 参    数：None
 * 返 回 值：None
 * 备    注：DATA_L    5ms
 * **************************************************/
void Voice_Start(void)
{
	VOICE_DATA_L;
	delay_ms(5);
}


/*****************************************************
 * 函 数 名：Voice_SendBit
 * 函数功能：发送码位
 * 参    数：None
 * 返 回 值：None
 * 备    注：
 * **************************************************/
void Voice_SendBit(u8 Bit)
{
	if (Bit == 1)
	{
		VOICE_DATA_H;
		delay_us(1200);
		VOICE_DATA_L;
		delay_us(400);
	}
	else if (Bit == 0)
	{
		VOICE_DATA_H;
		delay_us(400);
		VOICE_DATA_L;
		delay_us(1200);
	}
}

/*****************************************************
 * 函 数 名：Voice_SendBit
 * 函数功能：发送命令
 * 参    数：None
 * 返 回 值：None
 * 备    注：
 * **************************************************/
void Voice_SendCmd(u8 cmd)
{
	u8 i = 0;
	u16 TimeCnt = 0;
	Voice_Start();//起始条件
	for (i = 0; i < 8; i++)
	{
		if (cmd & 0x01)
		{
			Voice_SendBit(1);
		}
		else
		{
			Voice_SendBit(0);
		}
		cmd = cmd >> 1;//右移一位
	}
	VOICE_DATA_H;//拉高数据线
	while (VOICE_BUSY)
	{
		delay_us(1);
		TimeCnt++;
		if (TimeCnt > 2000) break;
	}
}





