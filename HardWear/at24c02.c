#include "at24c02.h"

/**************************************************************
*函数功能：AT24C02初始化函数
*参    数：None
*返 回 值：None
*备    注：PB6----SCL    PB7---SDA  配置输出开漏
**************************************************************/
void AT24C02_Init(void)
{
	IIC3_Init();
}

/**************************************************************
*函数功能：向AT24C02的指定地址写一个字节数据
*参    数：addr  地址（往哪写）   data 数据（写什么）
*返 回 值：0  发送一个字节成功    1  2  3发送失败
*备    注：
**************************************************************/
u8 AT24C02_WriteByte(u8 addr,u8 data)
{
	IIC3_Start();//起始条件
	if(IIC3_SendByte(Addr_Write)!=0)//发送器件地址写
	{
		IIC3_Stop();//发送停止条件
		return 1;
	}
	if(IIC3_SendByte(addr)!=0)//发送字地址（往哪写）
	{
		IIC3_Stop();//发送停止条件
		return 2;
	}
	if(IIC3_SendByte(data)!=0)//发送数据（写什么）
	{
		IIC3_Stop();//发送停止条件
		return 3;
	}
	IIC3_Stop();//发送停止条件
	
	delay_ms(5);//等待写周期
	return 0;
}


/**************************************************************
*函数功能：向AT24C02的指定地址读一个字节数据
*参    数：addr  地址（从哪读）   data 数据（保持读出的数据）
*返 回 值：0  读取一个字节成功    1  2 发送失败   3  读取失败
*备    注：
**************************************************************/
u8 AT24C02_ReadByte(u8 addr,u8*data)
{
	//1、发送起始条件
	IIC3_Start();
	
	//2、发送器件地址写
	if(IIC3_SendByte(Addr_Write)!=0)//发送器件地址写
	{
		IIC3_Stop();//发送停止条件
		return 1;
	}
	
	//3、发送字地址（从哪读）
	if(IIC3_SendByte(addr)!=0)//发送字地址（往哪读）
	{
		IIC3_Stop();//发送停止条件
		return 2;
	}
	
	//4、发送重复起始条件
	IIC3_Start();
	
	//5、发送器件地址读
	if(IIC3_SendByte(Addr_Read)!=0)//发送器件地址读
	{
		IIC3_Stop();//发送停止条件
		return 3;
	}
	//6、主机读取数据
	*data = IIC3_RecvByte(NO_ACK);
	//7、发送停止条件
	IIC3_Stop();
	
	return 0;
}


/**************************************************************
*函数功能：向AT24C02的指定地址(起始地址)读多个字节数据
*参    数：addr  地址（从哪开始读）len (长度）  buff 数据（保存读出的数据）
*返 回 值：0  读取一个字节成功    1  2 发送失败   3  读取失败
*备    注：
**************************************************************/
u8 AT24C02_ReadBytes(u8 addr,u16 len,u8*buff)
{
	//1、发送起始条件
	IIC3_Start();
	
	//2、发送器件地址写
	if(IIC3_SendByte(Addr_Write)!=0)//发送器件地址写
	{
		IIC3_Stop();//发送停止条件
		return 1;
	}
	
	//3、发送字地址（从哪读）
	if(IIC3_SendByte(addr)!=0)//发送字地址（往哪读）
	{
		IIC3_Stop();//发送停止条件
		return 2;
	}
	
	//4、发送重复起始条件
	IIC3_Start();
	
	//5、发送器件地址读
	if(IIC3_SendByte(Addr_Read)!=0)//发送器件地址读
	{
		IIC3_Stop();//发送停止条件
		return 3;
	}
	//6、读多个字节
	while(--len)
	{
		*buff = IIC3_RecvByte(ACK);
		buff++;
	}
	*buff = IIC3_RecvByte(NO_ACK);
	//7、发送停止条件
	IIC3_Stop();
	
	return 0;
}



/**************************************************************
*函数功能：向AT24C02的指定地址的当前页写数据
*参    数：addr  地址（往哪写） len(长度)  buff 数据（写什么）
*返 回 值：0  发送一个字节成功    1  2  3发送失败
*备    注：
**************************************************************/
u8 AT24C02_WritePage(u8 addr,u8 len,u8 *buff)
{
	if((addr/8) != ((addr+len-1)/8))
	{
		printf("长度超出当前页剩余\r\n");
		return 1;
	}
	
	IIC3_Start();//起始条件
	if(IIC3_SendByte(Addr_Write)!=0)//发送器件地址写
	{
		IIC3_Stop();//发送停止条件
		return 2;
	}
	if(IIC3_SendByte(addr)!=0)//发送字地址（往哪写）
	{
		IIC3_Stop();//发送停止条件
		return 3;
	}
	
	while(len--)
	{
		if(IIC3_SendByte(*buff)!=0)
		{
			IIC3_Stop();//发送停止条件
			return 4;
		}
		buff++;
	}
	
	IIC3_Stop();//发送停止条件
	
	delay_ms(5);//等待写周期
	return 0;
}

/**************************************************************
*函数功能：向AT24C02的指定地址的跨多页写
*参    数：addr  地址（往哪写） len(长度)  buff 数据（写什么）
*返 回 值：0  发送一个字节成功    1  2  3发送失败
*备    注：
**************************************************************/
u8 AT24C02_WritePages(u8 addr,u16 len,u8 *buff)
{
	u8 remain = 0;
	remain = 8-(addr%8);//当前页剩余的可写字节数
	if(len<=remain)
	{
		if(AT24C02_WritePage(addr,len,buff)!=0)
		{
			return 1;
		}
		return 0;
	}
	else
	{
		//把当前页写满
		if(AT24C02_WritePage(addr,remain,buff)!=0)
		{
			return 2;
		}
		addr = addr+remain;//存储空间的地址偏移
		buff = buff+remain;//数据地址的偏移
		len = len-remain;//剩余数据长度
		
		//剩余的数据长度大于等于8
		while(len>=8)
		{
			if(AT24C02_WritePage(addr,8,buff)!=0)
			{
				return 3;
			}
			addr = addr+8;//存储空间的地址偏移
			buff = buff+8;//数据地址的偏移
			len = len-8;//剩余数据长度
		}
		//剩余数据一定小于8，可能为0
		if(len != 0)
		{
			if(AT24C02_WritePage(addr,len,buff)!=0)
			{
				return 4;
			}
		}
	}
	return 0;
}










