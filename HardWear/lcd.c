#include "lcd.h"
#include "LCDfont.h"

/******************************************************
*函 数 名：LCD_Pin_Init
*函数功能：LCD引脚初始化
*参    数：None
*返 回 值：None
*备    注：
*******************************************************/
void LCD_Pin_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// 空闲状态
	LCD_RESET_H;
	LCD_CS_H;
	LCD_LEDK_OFF;
}


/******************************************************
*函 数 名：LCD_Send_Data8
*函数功能：LCD发送一个字节数据
*参    数：u8 dat 数据
*返 回 值：None
*备    注：
*******************************************************/
void LCD_Send_Data8(u8 dat)
{
	// 开启LCD通信
	LCD_CS_L;
	// 选择发送的是数据
	LCD_DC_DATA;
	// 发送命名数据
	SPI2_Transfer(dat);
	// 关闭通信
	LCD_CS_H;
}

/******************************************************
*函 数 名：LCD_Send_Data
*函数功能：LCD发送两个字节数据
*参    数：u8 dat 数据
*返 回 值：None
*备    注：
*******************************************************/
void LCD_Send_Data(u16 dat)
{
	// 开启LCD通信
	LCD_CS_L;
	// 选择发送的是数据
	LCD_DC_DATA;
	// 发送命名数据
	SPI2_Transfer(dat >> 8);   //发送高位数据
	SPI2_Transfer(dat & 0xff); //发送低位数据	
	// 关闭通信
	LCD_CS_H;
}
/******************************************************
*函 数 名：LCD_Send_Cmd
*函数功能：LCD发送一个指令
*参    数：u8 cmd 指令
*返 回 值：None
*备    注：
*******************************************************/
void LCD_Send_Cmd(u8 cmd)
{
	LCD_CS_L;
	// 选择发送的是命令
	LCD_DC_CMD;
	// 发送命名数据
	SPI2_Transfer(cmd);
	// 关闭通信
	LCD_CS_H;
}

/******************************************************
*函 数 名：LCD_Init
*函数功能：LCD初始化
*参    数：None
*返 回 值：None
*备    注：
*******************************************************/
void LCD_Init(void)
{
	LCD_Pin_Init();// 初始化GPIO
	SPI2_Init();

	/* 复位ST7789VM驱动器 */
	LCD_RESET_L;
	delay_ms(100);
	LCD_RESET_H;
	delay_ms(100);

	/* start initial sequence */
	LCD_Send_Cmd(0x36);
	LCD_Send_Data8(0x00);

	LCD_Send_Cmd(0x3A);
	LCD_Send_Data8(0x05);

	LCD_Send_Cmd(0xB2);
	LCD_Send_Data8(0x0C);
	LCD_Send_Data8(0x0C);
	LCD_Send_Data8(0x00);
	LCD_Send_Data8(0x33);
	LCD_Send_Data8(0x33);

	LCD_Send_Cmd(0xB7);
	LCD_Send_Data8(0x35);

	LCD_Send_Cmd(0xBB);
	LCD_Send_Data8(0x19);

	LCD_Send_Cmd(0xC0);
	LCD_Send_Data8(0x2C);

	LCD_Send_Cmd(0xC2);
	LCD_Send_Data8(0x01);

	LCD_Send_Cmd(0xC3);
	LCD_Send_Data8(0x12);

	LCD_Send_Cmd(0xC4);
	LCD_Send_Data8(0x20);

	LCD_Send_Cmd(0xC6);
	LCD_Send_Data8(0x0F);

	LCD_Send_Cmd(0xD0);
	LCD_Send_Data8(0xA4);
	LCD_Send_Data8(0xA1);

	LCD_Send_Cmd(0xE0);
	LCD_Send_Data8(0xD0);
	LCD_Send_Data8(0x04);
	LCD_Send_Data8(0x0D);
	LCD_Send_Data8(0x11);
	LCD_Send_Data8(0x13);
	LCD_Send_Data8(0x2B);
	LCD_Send_Data8(0x3F);
	LCD_Send_Data8(0x54);
	LCD_Send_Data8(0x4C);
	LCD_Send_Data8(0x18);
	LCD_Send_Data8(0x0D);
	LCD_Send_Data8(0x0B);
	LCD_Send_Data8(0x1F);
	LCD_Send_Data8(0x23);

	LCD_Send_Cmd(0xE1);
	LCD_Send_Data8(0xD0);
	LCD_Send_Data8(0x04);
	LCD_Send_Data8(0x0C);
	LCD_Send_Data8(0x11);
	LCD_Send_Data8(0x13);
	LCD_Send_Data8(0x2C);
	LCD_Send_Data8(0x3F);
	LCD_Send_Data8(0x44);
	LCD_Send_Data8(0x51);
	LCD_Send_Data8(0x2F);
	LCD_Send_Data8(0x1F);
	LCD_Send_Data8(0x1F);
	LCD_Send_Data8(0x20);
	LCD_Send_Data8(0x23);

	LCD_Send_Cmd(0x21);
	LCD_Send_Cmd(0x11);
	LCD_Send_Cmd(0x29);

	LCD_LEDK_ON;	// 打开背光

	LCD_Clear(WHITE);
}

/******************************************************
*函 数 名：LCD_Address_Set
*函数功能：LCD光标设置
*参    数：u16 x1, u16 y1, u16 x2, u16 y2  起始x 起始y  结束x  结束y
*返 回 值：None
*备    注：
*******************************************************/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Send_Cmd(0x2a);//列地址设置
	LCD_Send_Data(x1);
	LCD_Send_Data(x2);
	LCD_Send_Cmd(0x2b);//行地址设置
	LCD_Send_Data(y1);
	LCD_Send_Data(y2);
	LCD_Send_Cmd(0x2c);//颜色指令
}

/******************************************************
*函 数 名：LCD_Clear
*函数功能：LCD清屏函数
*参    数：u16 color
*返 回 值：None
*备    注：
*******************************************************/
void LCD_Clear(u16 color)
{
	LCD_Address_Set(0, 0, LCD_W - 1, LCD_H - 1);
	for (u32 i = 0; i < LCD_W * LCD_H; i++)
	{
		LCD_Send_Data(color);
	}
}

/******************************************************
*函 数 名：LCD_ClearArea
*函数功能：LCD局部清屏函数
*参    数：u8 x0,u8 y0,u8 wide,u8 high,u16 color
*返 回 值：None
*备    注：
*******************************************************/
void LCD_ClearArea(u8 x0, u8 y0, u8 wide, u8 high, u16 color)
{
	LCD_Address_Set(x0, y0, x0 + wide - 1, y0 + high - 1);
	for (u32 i = 0; i < wide * high; i++)
	{
		LCD_Send_Data(color);
	}
}


/******************************************************
*函 数 名：LCD_Point
*函数功能：LCD打点
*参    数：u16 color
*返 回 值：None
*备    注：
*******************************************************/
void LCD_Point(u16 x, u16 y, u16 color)
{
	LCD_Address_Set(x, y, x, y);
	LCD_Send_Data(color);
}


/******************************************************
*函 数 名：LCD_ShowCharX16
*函数功能：显示一个字符
*参    数：uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：
*******************************************************/
void LCD_ShowChar8X16(uint16_t x0, uint16_t y0, const uint8_t* font, uint16_t color, uint16_t bgcolor)
{
	u8 row = 0, col = 0;
	u16 pcolor = 0;
	for (row = 0; row < 16; row++)//遍历行
	{
		for (col = 0; col < 8; col++)//遍历列   
		{
			pcolor = (font[row] & (0x80 >> col)) ? color : bgcolor;
			LCD_Point(x0 + col, y0 + row, pcolor);
		}
	}
}


/******************************************************
*函 数 名：LCD_ShowChar16X32
*函数功能：显示一个字符
*参    数：uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：
*******************************************************/
void LCD_ShowChar16X32(uint16_t x0, uint16_t y0, const uint8_t* font, uint16_t color, uint16_t bgcolor)
{
	u8 row = 0, col = 0;
	u16 pcolor = 0;
	u8 highbyte = 0, lowbyte = 0;
	for (row = 0; row < 32; row++)//遍历行
	{
		highbyte = font[2 * row];
		lowbyte = font[2 * row + 1];
		for (col = 0; col < 16; col++)//遍历列   
		{
			if (col < 8)//判断高字节
				pcolor = (highbyte & (0x80 >> col)) ? color : bgcolor;
			else//判断低字节
				pcolor = (lowbyte & (0x80 >> (col - 8))) ? color : bgcolor;
			LCD_Point(x0 + col, y0 + row, pcolor);
		}
	}
}









