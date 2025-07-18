#include "iic.h"


/******************************************************
*函 数 名：IIC3_Init
*函数功能：IIC3初始化
*参    数：None
*返 回 值：None
*备    注：
*******************************************************/
void IIC3_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	IIC3_SCL_H;
	IIC3_SDA_H;
}


/**************************************************************
*函数功能：起始条件函数
*参    数：None
*返 回 值：None
*备    注：时钟线高电平期间，数据线产生下降沿
**************************************************************/
void IIC3_Start(void)
{
	IIC3_SCL_L;//使数据线变化作准备
	delay_us(5);
	IIC3_SDA_H;//为数据线下降沿作准备

	IIC3_SCL_H;//时钟线高电平
	delay_us(5);
	IIC3_SDA_L;//数据线下降沿
	delay_us(5);

	IIC3_SCL_L;//作为安全作用
}


/**************************************************************
*函数功能：发送一个字节函数
*参    数：None
*返 回 值：None
*备    注： 时钟线为低电平，发送方发送数据
			时钟线为高电平，接收方接收数据
**************************************************************/
u8 IIC3_SendByte(u8 Byte)
{
	u8 i, ack = 0;
	for (i = 0; i < 8; i++)
	{
		IIC3_SCL_L;//使数据线变化作准备
		delay_us(3);
		if (Byte & (0x80 >> i))
		{
			IIC3_SDA_H;
		}
		else
		{
			IIC3_SDA_L;
		}
		delay_us(2);
		IIC3_SCL_H;//为从机读数据作准备
		delay_us(5);//从机读应答信号
	}

	ack = IIC3_RecvAck();

	IIC3_SCL_L;//作为安全作用
	return ack;
}


/**************************************************************
*函数功能：接收一个字节函数
*参    数：None
*返 回 值：None
*备    注： 时钟线为低电平，发送方发送数据
			时钟线为高电平，接收方接收数据
**************************************************************/
u8 IIC3_RecvByte(u8 ack)
{
	u8 i, Byte = 0;
	IIC3_SCL_L;//使数据线变化作准备
	IIC3_SDA_H;//主机拉高数据线，数据线控制权交给从机

	for (i = 0; i < 8; i++)
	{
		IIC3_SCL_L;//为从机发送数据作准备
		delay_us(5);//从机在发送数据

		IIC3_SCL_H;//为主机读取数据作准备
		Byte = Byte << 1;
		delay_us(5);
		if (IIC3_SDA_R)
		{
			Byte |= 0x01;//接受到为数据1
		}
	}

	IIC3_SendAck(ack);
	IIC3_SCL_L;//作为安全作用
	return Byte;
}


/**************************************************************
*函数功能：发送应答/非应答函数
*参    数：None
*返 回 值：None
*备    注：应答：第九个脉冲，时钟线为高电平时数据线保持低电平
		   非应答：第九个脉冲，时钟线为高电平时数据线保持高电平
**************************************************************/
void IIC3_SendAck(u8 ack)
{
	IIC3_SCL_L;//使数据线变化作准备
	delay_us(3);
	if (ack == ACK)//应答
	{
		IIC3_SDA_L;
	}
	else//非应答
	{
		IIC3_SDA_H;
	}
	delay_us(2);

	IIC3_SCL_H;//为从机读数据作准备
	delay_us(5);//从机读应答信号
	IIC3_SCL_L;//作为安全作用
}

/**************************************************************
*函数功能：接受应答/非应答函数
*参    数：None
*返 回 值：None
*备    注：应答：第九个脉冲，时钟线为高电平时数据线保持低电平
		   非应答：第九个脉冲，时钟线为高电平时数据线保持高电平
**************************************************************/
u8 IIC3_RecvAck(void)
{
	u8 ack;
	IIC3_SCL_L;//使数据线变化作准备
	IIC3_SDA_H;//主机拉高数据线，数据线控制权交给从机

	delay_us(5);//数据线被从机改变

	IIC3_SCL_H;//为主机读创造条件
	delay_us(5);
	if (IIC3_SDA_R)
	{
		ack = NO_ACK;
	}
	else
	{
		ack = ACK;
	}


	IIC3_SCL_L;//作为安全作用

	return ack;
}




/**************************************************************
*函数功能：停止条件函数
*参    数：None
*返 回 值：None
*备    注：时钟线高电平期间，数据线产生上升沿
**************************************************************/
void IIC3_Stop(void)
{
	IIC3_SCL_L;//使数据线变化作准备
	delay_us(5);

	IIC3_SDA_L;//为数据线上升沿作准备	

	IIC3_SCL_H;//时钟线高电平
	delay_us(5);
	IIC3_SDA_H;//数据线上升沿
	delay_us(5);
}



/******************************************************
*函 数 名：IIC1_Init
*函数功能：IIC1初始化
*参    数：None
*返 回 值：None
*备    注：
*******************************************************/
void IIC1_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	IIC1_SCL_H;
	IIC1_SDA_H;
}


/**************************************************************
*函数功能：起始条件函数
*参    数：None
*返 回 值：None
*备    注：时钟线高电平期间，数据线产生下降沿
**************************************************************/
void IIC1_Start(void)
{
	IIC1_SCL_L;//使数据线变化作准备
	delay_us(5);
	IIC1_SDA_H;//为数据线下降沿作准备

	IIC1_SCL_H;//时钟线高电平
	delay_us(5);
	IIC1_SDA_L;//数据线下降沿
	delay_us(5);

	IIC1_SCL_L;//作为安全作用
}


/**************************************************************
*函数功能：发送一个字节函数
*参    数：None
*返 回 值：None
*备    注： 时钟线为低电平，发送方发送数据
			时钟线为高电平，接收方接收数据
**************************************************************/
u8 IIC1_SendByte(u8 Byte)
{
	u8 i, ack = 0;
	for (i = 0; i < 8; i++)
	{
		IIC1_SCL_L;//使数据线变化作准备
		delay_us(3);
		if (Byte & (0x80 >> i))
		{
			IIC1_SDA_H;
		}
		else
		{
			IIC1_SDA_L;
		}
		delay_us(2);
		IIC1_SCL_H;//为从机读数据作准备
		delay_us(5);//从机读应答信号
	}

	IIC1_SCL_L;//作为安全作用
	return ack;
}


/**************************************************************
*函数功能：接收一个字节函数
*参    数：None
*返 回 值：None
*备    注： 时钟线为低电平，发送方发送数据
			时钟线为高电平，接收方接收数据
**************************************************************/
u8 IIC1_RecvByte(void)
{
	u8 i, Byte = 0;
	IIC1_SCL_L;//使数据线变化作准备
	IIC1_SDA_H;//主机拉高数据线，数据线控制权交给从机

	for (i = 0; i < 8; i++)
	{
		IIC1_SCL_L;//为从机发送数据作准备
		delay_us(5);//从机在发送数据

		IIC1_SCL_H;//为主机读取数据作准备
		Byte = Byte << 1;
		delay_us(5);
		if (IIC1_SDA_R)
		{
			Byte |= 0x01;//接受到为数据1
		}
	}
	IIC1_SCL_L;//作为安全作用
	return Byte;
}


/**************************************************************
*函数功能：发送应答/非应答函数
*参    数：None
*返 回 值：None
*备    注：应答：第九个脉冲，时钟线为高电平时数据线保持低电平
		   非应答：第九个脉冲，时钟线为高电平时数据线保持高电平
**************************************************************/
void IIC1_SendAck(u8 ack)
{
	IIC1_SCL_L;//使数据线变化作准备
	delay_us(3);
	if (ack == ACK)//应答
	{
		IIC1_SDA_L;
	}
	else//非应答
	{
		IIC1_SDA_H;
	}
	delay_us(2);

	IIC1_SCL_H;//为从机读数据作准备
	delay_us(5);//从机读应答信号
	IIC1_SCL_L;//作为安全作用
}

/**************************************************************
*函数功能：接受应答/非应答函数
*参    数：None
*返 回 值：None
*备    注：应答：第九个脉冲，时钟线为高电平时数据线保持低电平
		   非应答：第九个脉冲，时钟线为高电平时数据线保持高电平
**************************************************************/
u8 IIC1_RecvAck(void)
{
	u8 ack;
	IIC1_SCL_L;//使数据线变化作准备
	IIC1_SDA_H;//主机拉高数据线，数据线控制权交给从机

	delay_us(5);//数据线被从机改变

	IIC1_SCL_H;//为主机读创造条件
	delay_us(5);
	if (IIC1_SDA_R)
	{
		ack = NO_ACK;
	}
	else
	{
		ack = ACK;
	}


	IIC1_SCL_L;//作为安全作用

	return ack;
}




/**************************************************************
*函数功能：停止条件函数
*参    数：None
*返 回 值：None
*备    注：时钟线高电平期间，数据线产生上升沿
**************************************************************/
void IIC1_Stop(void)
{
	IIC1_SCL_L;//使数据线变化作准备
	delay_us(5);

	IIC1_SDA_L;//为数据线上升沿作准备	

	IIC1_SCL_H;//时钟线高电平
	delay_us(5);
	IIC1_SDA_H;//数据线上升沿
	delay_us(5);
}


