#include "usart1.h"

void USART1_Config(u8 baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	 // 使能USART1时钟

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // 选择PA9、PA10引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// 推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// 初始化GPIOA

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = baud;					// 波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;			// 无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		// 发送和接收模式
	USART_Init(USART1, &USART_InitStructure);	 // 初始化USART1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能接收中断
	USART_Cmd(USART1, ENABLE);	// 使能USART1

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; // USART1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断
	NVIC_Init(&NVIC_InitStructure); // 初始化NVIC
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 检查接收中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除接收中断标志
		u8 data = USART_ReceiveData(USART1); // 接收数据
		USART_SendData(USART1, data); // 回显数据
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // 等待发送完成
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) // 检查发送中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_TXE); // 清除发送中断标志

	}

}