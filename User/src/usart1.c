#include "usart1.h"

void USART1_Config(u32 baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  // 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // 使能USART1时钟

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;    // 选择PA9、PA10引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;               // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;          // 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             // 推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);                     // 初始化GPIOA
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  // 设置PA9为USART1_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); // 设置PA10为USART1_RX

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;                                      // 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // 数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // 停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;                             // 无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // 发送和接收模式
	USART_Init(USART1, &USART_InitStructure);                                       // 初始化USART1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); // 使能空闲中断

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         // USART1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能中断
	NVIC_Init(&NVIC_InitStructure);                           // 初始化NVIC
	USART_Cmd(USART1, ENABLE);                                // 使能USART1
}

void USART1_SendByte(u8 Byte)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待发送寄存器为空时可发送
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

//中断服务函数
void USART1_IRQHandler(void)
{
	u8 clear = 0;
	//接受中断
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);//清除中断标志位
		if (USART1_Recv.index < Max_Size - 1)//防止溢出
		{
			USART1_Recv.data[USART1_Recv.index] = USART_ReceiveData(USART1);
			USART1_Recv.index++;
		}
	}
	//空闲中断
	if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		//清除中断标志位
		clear = USART1->SR;
		clear = USART1->DR;
		(void)clear;
		USART1_Recv.data[USART1_Recv.index] = '\0';
		//USART2_SendStr((char*)USART1_Recv.data); //将接收到的数据发送到USART2
		USART1_Recv.flag = 1;
		USART1_Recv.index = 0;
	}
}

//串口1空闲中断后将数据打印出来
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
函数功能：printf重定向
参    数：None
返 回 值：None
备    注：
*****************************************************/
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE
{
	int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x)
{
	x = x;
}
void _ttywrch(int ch)
{
	ch = ch;
}
//重定义fputc函数 
int fputc(int ch, FILE* f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, ch);
	return ch;
}

/**********************printf support end**********************/

#endif