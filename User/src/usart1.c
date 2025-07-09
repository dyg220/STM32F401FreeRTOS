#include "usart1.h"

void USART1_Config(u8 baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	 // ʹ��USART1ʱ��

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // ѡ��PA9��PA10����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// �ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// �������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// ��ʼ��GPIOA

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;					// ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // ����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;			// ����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		// ���ͺͽ���ģʽ
	USART_Init(USART1, &USART_InitStructure);	 // ��ʼ��USART1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // ʹ�ܽ����ж�
	USART_Cmd(USART1, ENABLE);	// ʹ��USART1

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // USART1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // ʹ���ж�
	NVIC_Init(&NVIC_InitStructure); // ��ʼ��NVIC
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // �������ж�
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); // ��������жϱ�־
		u8 data = USART_ReceiveData(USART1); // ��������
		USART_SendData(USART1, data); // ��������
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // �ȴ��������
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) // ��鷢���ж�
	{
		USART_ClearITPendingBit(USART1, USART_IT_TXE); // ��������жϱ�־

	}

}