#include "voice.h"

/*****************************************************
 * �� �� ����Voice_Init
 * �������ܣ�������ʼ��
 * ��    ����None
 * �� �� ֵ��None
 * ��    ע��DATA---PC5      BUSY---PC4
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
 * �� �� ����Voice_Start
 * �������ܣ���ʼ����
 * ��    ����None
 * �� �� ֵ��None
 * ��    ע��DATA_L    5ms
 * **************************************************/
void Voice_Start(void)
{
	VOICE_DATA_L;
	delay_ms(5);
}


/*****************************************************
 * �� �� ����Voice_SendBit
 * �������ܣ�������λ
 * ��    ����None
 * �� �� ֵ��None
 * ��    ע��
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
 * �� �� ����Voice_SendBit
 * �������ܣ���������
 * ��    ����None
 * �� �� ֵ��None
 * ��    ע��
 * **************************************************/
void Voice_SendCmd(u8 cmd)
{
	u8 i = 0;
	u16 TimeCnt = 0;
	Voice_Start();//��ʼ����
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
		cmd = cmd >> 1;//����һλ
	}
	VOICE_DATA_H;//����������
	while (VOICE_BUSY)
	{
		delay_us(1);
		TimeCnt++;
		if (TimeCnt > 2000) break;
	}
}





