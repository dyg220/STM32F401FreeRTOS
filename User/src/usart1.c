#include "usart1.h"

void USART1_Config(u32 baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // ʹ��USART1ʱ��

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;    // ѡ��PA9��PA10����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;               // ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;          // �ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             // �������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                     // ��ʼ��GPIOA
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  // ����PA9ΪUSART1_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); // ����PA10ΪUSART1_RX

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;                                      // ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // ����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;                             // ����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // ���ͺͽ���ģʽ
	USART_Init(USART1, &USART_InitStructure);                                       // ��ʼ��USART1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // ʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); // ʹ�ܿ����ж�

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         // USART1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);                           // ��ʼ��NVIC
	USART_Cmd(USART1, ENABLE);                                // ʹ��USART1
}

void USART1_SendByte(u8 Byte)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//�ȴ����ͼĴ���Ϊ��ʱ�ɷ���
	USART_SendData(USART1, Byte);
}

void USART1_SendStr(u8* str)
{
	while (*str != '\0')
	{
		USART1_SendByte(*str);
		str++;
	}
	USART1_SendByte('\0');
}

USART1_INFO USART1_Recv = { 0 };

//�жϷ�����
void USART1_IRQHandler(void)
{
	u8 clear = 0;
	//�����ж�
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//����жϱ�־λ
		if (USART1_Recv.index < Max_Size - 1)//��ֹ���
		{
			USART1_Recv.data[USART1_Recv.index] = USART_ReceiveData(USART1);
			USART1_Recv.index++;
		}
	}
	//�����ж�
	if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		//����жϱ�־λ
		clear = USART1->SR;
		clear = USART1->DR;
		(void)clear;
		USART1_Recv.data[USART1_Recv.index] = '\0';
		//USART2_SendStr((char*)USART1_Recv.data); //�����յ������ݷ��͵�USART2
		USART1_Recv.flag = 1;
		USART1_Recv.index = 0;
	}
}

//����1�����жϺ����ݴ�ӡ����
void RECV_USART1(void)
{
	if (USART1_Recv.flag == 1)
	{
		printf("%s\r\n", USART1_Recv.data);
		USART1_Recv.flag = 0;
		memset(USART1_Recv.data, 0, sizeof(USART1_Recv.data));
	}
}


/****************************************************
�������ܣ�printf�ض���
��    ����None
�� �� ֵ��None
��    ע��
*****************************************************/
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE
{
	int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x)
{
	x = x;
}
void _ttywrch(int ch)
{
	ch = ch;
}
//�ض���fputc���� 
int fputc(int ch, FILE* f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, ch);
	return ch;
}

/**********************printf support end**********************/

#endif