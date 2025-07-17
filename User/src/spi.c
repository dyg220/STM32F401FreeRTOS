#include "spi.h"

/******************************************************
*函 数 名：SPI2_Init
*函数功能：SPI2初始化
*参    数：None
*返 回 值：None
*备    注：（ST7789VM接口）
*******************************************************/
void SPI2_Init(void)
{
	//1、开启GPIOB时钟，SPI2时钟(APB1)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	//2、初始化GPIO引脚
	GPIO_InitTypeDef GPIO_InitStruct = {0};	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//3、将引脚复用为SPI3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
						    
	//4、初始化SPI
	SPI_InitTypeDef SPI_InitStruct = {0};	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	  //波特率分频
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;           				//时钟相位
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;             				//时钟极性
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;					 	//数据大小
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;						//数据的传输方向
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;							//主机模式
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;								//软件模式，不启用	
	SPI_Init(SPI2, &SPI_InitStruct);
	
	//5、使能
	SPI_Cmd(SPI2, ENABLE);
}
/******************************************************
*函 数 名：SPI2_Transfer
*函数功能：SPI2传输一个字节
*参    数：Data 数据
*返 回 值：None
*备    注：
*******************************************************/
u8 SPI2_Transfer(u8 Data)
{
	//1、发送单字节数据
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI2, Data);
	//2、接收单字节数据
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI2);
}




/******************************************************
*函 数 名：SPI3_Init
*函数功能：SPI3初始化
*参    数：None
*返 回 值：None
*备    注：（RC522接口）
*******************************************************/
void SPI3_Init(void)
{
	//1、开启GPIOC时钟，SPI3时钟(APB1)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	//2、初始化GPIO引脚
	GPIO_InitTypeDef GPIO_InitStruct = {0};	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//3、将引脚复用为SPI3
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
						    
	//4、初始化SPI
	SPI_InitTypeDef SPI_InitStruct = {0};	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//波特率分频
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;           				//时钟相位
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;             				//时钟极性
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;					 	//数据大小
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;						//数据的传输方向
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;							//主机模式
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;								//软件模式，不启用	
	SPI_Init(SPI3, &SPI_InitStruct);
	
	//5、使能
	SPI_Cmd(SPI3, ENABLE);
}
/******************************************************
*函 数 名：SPI3_Transfer
*函数功能：SPI3传输一个字节
*参    数：Data 数据
*返 回 值：None
*备    注：
*******************************************************/
u8 SPI3_Transfer(u8 Data)
{
	//1、发送单字节数据
	while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI3, Data);
	//2、接收单字节数据
	while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI3);
}



