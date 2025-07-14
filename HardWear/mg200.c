#include "mg200.h"


/******************************************************
*函 数 名：USART6_Config
*函数功能：USART6配置
*参    数：Baud
*返 回 值：None
*备    注：
*******************************************************/
void USART6_Config(u32 Baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	USART_InitTypeDef USART_InitStruct = { 0 };
	USART_InitStruct.USART_BaudRate = Baud;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART6, &USART_InitStruct);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//使能接受中断
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);//使能空闲中断

	NVIC_InitTypeDef NVIC_InitStruct = { 0 };
	NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;  		//通道号
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;    		//是否使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;    		//次级优先级（响应）

	NVIC_Init(&NVIC_InitStruct);  //结构体变量的地址

	USART_Cmd(USART6, ENABLE);
}

void USART6_SendByte(u8 Byte)
{
	while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == 0);
	USART_SendData(USART6, Byte);
}

u8 USART6_RecvByte(void)
{
	while (USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == 0);
	return USART_ReceiveData(USART6);
}

#define Max_Size   256

typedef struct {
	u8 data[Max_Size];
	u16 index;
	u8 flag;
}USART_INFO;

USART_INFO MG200_Recv = { 0 };


//中断服务函数
void USART6_IRQHandler(void)
{
	u8 clear = 0;
	//接受中断
	if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);//清除中断标志位
		if (MG200_Recv.index < Max_Size - 1)//防止溢出
		{
			MG200_Recv.data[MG200_Recv.index] = USART_ReceiveData(USART6);
			MG200_Recv.index++;
		}
	}
	//空闲中断
	if (USART_GetITStatus(USART6, USART_IT_IDLE) == SET)
	{
		//清除中断标志位
		clear = USART6->SR;
		clear = USART6->DR;
		(void)clear;
		MG200_Recv.flag = 1;
		MG200_Recv.index = 0;
	}
}

/******************************************************
*函 数 名：MG200_Send_Cmd
*函数功能：MG200发送数据包
*参    数：cmd 指令码     parameter 参数
*返 回 值：None
*备    注：
*******************************************************/
void MG200_Send_Cmd(u8 cmd, u8 parameter)
{


}

/******************************************************
*函 数 名：MG200_Read_Cmd
*函数功能：MG200解析数据包
*参    数：cmd        之前发送的指令码
		   *parameter 接收到的命令参数
		   *result    命令执行结果
*返 回 值：0成功    其他错误
*备    注：
*******************************************************/
u8 MG200_Read_Cmd(u8 cmd, u8* result, u8* parameter)
{

}