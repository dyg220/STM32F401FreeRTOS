#include "door.h"

/*************************************************
 * �� �� ����Door_Init
 * �������ܣ�������ʼ��
 * ��    ����None
 * �� �� ֵ��None
 * ��    ע��MOTOR_IB---PB3   MOTOR_IA---PB4
 * **************************************************/
void Door_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	Door_Close();//Ĭ�Ϲ���
}

/*****************************************************
 * �� �� ����Door_Open
 * �������ܣ�����
 * ��    ����None
 * �� �� ֵ��None
 * ��    ע��MOTOR_IB---PB3   MOTOR_IA---PB4
 *****************************************************/
void Door_Open(void)
{
	IA_H;
	IB_L;
	delay_ms(300);
	IA_H;
	IB_H;
}


/*****************************************************
 * �� �� ����Door_Open
 * �������ܣ�����
 * ��    ����None
 * �� �� ֵ��None
 * ��    ע��MOTOR_IB---PB3   MOTOR_IA---PB4
 *****************************************************/
void Door_Close(void)
{
	IA_L;
	IB_H;
	delay_ms(300);
	IA_H;
	IB_H;
}


/*****************************************************
 * �� �� ����Door_Start
 * �������ܣ��������Զ�����
 * ��    ����None
 * �� �� ֵ��None
 * ��    ע��MOTOR_IB---PB3   MOTOR_IA---PB4
 *****************************************************/
void Door_Start(u16 nms)
{
	Door_Open();//����
	delay_ms(nms);
	Door_Close();//����
}






