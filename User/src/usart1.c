#include "usart1.h"

/******************************************************
*�� �� ����USART1_Config
*�������ܣ����ڳ�ʼ��
*��    ����Baud
*�� �� ֵ��None
*��    ע��
*******************************************************/
void USART1_Config(u32 Baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	USART_InitTypeDef USART_InitStruct = { 0 };
	USART_InitStruct.USART_BaudRate = Baud;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//ʹ�ܿ����ж�

	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;  		//ͨ����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;    		//�Ƿ�ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;    		//�μ����ȼ�����Ӧ��

	NVIC_Init(&NVIC_InitStruct);  //�ṹ������ĵ�ַ

	USART_Cmd(USART1, ENABLE);
}

void USART1_SendByte(u8 Byte)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == 0);
	USART_SendData(USART1, Byte);
}

u8 USART1_RecvByte(void)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == 0);
	return USART_ReceiveData(USART1);
}


USART_INFO USART1_Recv = { 0 };

extern u8 zk_flag;
//�жϷ�����
void USART1_IRQHandler(void)
{
	u8 data = 0;
	static u32 zk_addr = 0x020000;//�ֿ���ʼ��ַ
	static u32 zk_cont = 0;
	u8 clear = 0;
	//�����ж�
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//����жϱ�־λ
		data = USART_ReceiveData(USART1);
		if (USART1_Recv.index < Max_Size - 1)//��ֹ���
		{
			USART1_Recv.data[USART1_Recv.index] = data;
			USART1_Recv.index++;
		}
		if (zk_flag == 1)
		{
			W25Q64_Write_Page(zk_addr, 1, &data);
			zk_addr++;
			LEDB_ON;
			zk_cont++;
			if (zk_cont == 0x00141F58)
			{
				LEDB_OFF;
				printf("�ֿ���¼���\r\n");
				zk_flag = 0;
			}
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
		USART1_Recv.flag = 1;
		USART1_Recv.index = 0;
	}
}


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









