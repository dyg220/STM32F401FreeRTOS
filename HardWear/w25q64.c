#include "w25q64.h"

/**************************************************************
*函 数 名：W25Q64_Init
*函数功能：W25Q64初始化
*参    数：None
*返 回 值：None
*备    注：CS---PB14
**************************************************************/
void W25Q64_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	SPI1_Init();
	
	W25Q64_CS_H;
	Write_Status(0x00);//解除所有块写保护
}

/**************************************************************
*函数功能：写使能函数
*参    数：None
*返 回 值：None
*备    注：0x06  "页编程" "扇区擦除" "块区擦除" "芯片擦除" "写控制状态寄存器"
**************************************************************/
void Write_Enable(void)
{
	W25Q64_CS_L;
	SPI1_Transfer(0x06);
	W25Q64_CS_H;
}

/**************************************************************
*函数功能：读状态寄存器函数
*参    数：None
*返 回 值：状态寄存器的值
*备    注：0x05  
**************************************************************/
u8 Read_Status(void)
{
	u8 sta = 0;
	W25Q64_CS_L;
	SPI1_Transfer(0x05);
	sta = SPI1_Transfer(0xff);
	W25Q64_CS_H;
	return sta;
}


/**************************************************************
*函数功能：写控制状态寄存器函数
*参    数：寄存器配置信息
*返 回 值：None
*备    注：0x01  
**************************************************************/
void Write_Status(u8 data)
{
	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI1_Transfer(0x01);
	SPI1_Transfer(data);
	W25Q64_CS_H;
	while(Read_Status()&(1<<0));//等待BUSY变0

}

/**************************************************************
*函数功能：连续读函数
*参    数：addr  从哪读  u32 len 读多长  u8*buf 保存读出的数据的空间
*返 回 值：None
*备    注：0x03
**************************************************************/
void W25Q64_Read_Bytes(u32 addr,u32 len, u8*buf)
{
	W25Q64_CS_L;
	SPI1_Transfer(0x03);
	SPI1_Transfer(addr>>16);
	SPI1_Transfer(addr>>8);
	SPI1_Transfer(addr);
	while(len--)
	{
		*buf = SPI1_Transfer(0xff);
		buf++;
	}
	W25Q64_CS_H;
}

/**************************************************************
*函数功能：页写函数
*参    数：addr  往哪写  u32 len   u8*buf 保存读出的数据的空间
*返 回 值：None
*备    注：0x02
**************************************************************/
void W25Q64_Write_Page(u32 addr,u32 len, u8*buf)
{
	if((addr/256) != ((addr+len-1)/256))
	{
		printf("长度超出当前页剩余\r\n");
		return ;
	}
	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI1_Transfer(0x02);
	SPI1_Transfer(addr>>16);
	SPI1_Transfer(addr>>8);
	SPI1_Transfer(addr);
	while(len--)
	{
		SPI1_Transfer(*buf);
		buf++;
	}
	W25Q64_CS_H;
	while(Read_Status()&(1<<0));//等待BUSY变0
}

/**************************************************************
*函数功能：跨页写函数
*参    数：addr  往哪写  u32 len   u8*buf 保存读出的数据的空间
*返 回 值：None
*备    注：0x02
**************************************************************/
void W25Q64_Write_Pages(u32 addr,u32 len, u8*buf)
{
	u32 remain = 0;
	remain = 256-(addr%256);//当前页剩余的可写字节数
	if(len<=remain)
	{
		W25Q64_Write_Page(addr,len,buf);
		return ;
	}
	else
	{
		//把当前页写满
		W25Q64_Write_Page(addr,remain,buf);
		addr = addr+remain;//存储空间的地址偏移
		buf = buf+remain;//数据地址的偏移
		len = len-remain;//剩余数据长度

		//剩余的数据长度大于等于8
		while(len>=256)
		{
			W25Q64_Write_Page(addr,256,buf);
			addr = addr+256;//存储空间的地址偏移
			buf = buf+256;//数据地址的偏移
			len = len-256;//剩余数据长度
		}
		//剩余数据一定小于8，可能为0
		if(len != 0)
		{
			W25Q64_Write_Page(addr,len,buf);
		}
	}
}

/**************************************************************
*函数功能：扇区擦除函数
*参    数：addr 从哪开始擦除  (4096 Byte)
*返 回 值：None
*备    注：0x20
**************************************************************/
void W25Q64_Sector_Erase(u32 addr)
{
	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI1_Transfer(0x20);
	SPI1_Transfer(addr>>16);
	SPI1_Transfer(addr>>8);
	SPI1_Transfer(addr);
	W25Q64_CS_H;
	while(Read_Status()&(1<<0));//等待BUSY变0
}



/**************************************************************
*函数功能：块区擦除函数
*参    数：addr 从哪开始擦除  
*返 回 值：None
*备    注：0xd8
**************************************************************/
void W25Q64_block_Erase(u32 addr)
{
	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI1_Transfer(0xd8);
	SPI1_Transfer(addr>>16);
	SPI1_Transfer(addr>>8);
	SPI1_Transfer(addr);
	W25Q64_CS_H;
	while(Read_Status()&(1<<0));//等待BUSY变0
}



/***************************************************
函数名：W25Q64_Erase_Blockn
功  能：块擦除
参  数：u32 addr,u8 num
返回值：无
备  注：addr:第一块地址
		num:擦除多少块
***************************************************/
void W25Q64_Erase_Blockn(u32 addr,u8 num)
{
	u8 i=0;
	while(num--)
	{
		W25Q64_block_Erase(addr);
		i++;
		addr+=65536;
	}
}


/**************************************************************
*函数功能：芯片擦除函数
*参    数：None
*返 回 值：None
*备    注：0xC7
**************************************************************/
void W25Q64_Chip_Erase(void)
{
	//写使能
	Write_Enable();
	W25Q64_CS_L;
	SPI1_Transfer(0xC7);
	W25Q64_CS_H;
	while(Read_Status()&(1<<0));//等待BUSY变0
}


u8 zk_flag = 0;

/***************************************************
函数名：W25Q64_Zk_Updata
功  能：字库更新
参  数：无
返回值：无
备  注：
***************************************************/
void W25Q64_Zk_Updata(void)
{
	u8 keynum=0;
	keynum = Key_Scan();
	if(keynum == 2)   						//烧录字库的触发标志
	{
		printf("开始擦除空间,请稍后....\r\n");
		//0x00000~x7fffff
		//从第二块开始烧录
		W25Q64_Erase_Blockn(0x020000,29);
		printf("擦除空间完成,请开始烧录字库\r\n");
		zk_flag=1;             	//开始接收字库标志
	}
}



