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

/****************************************直接填数组名**************************************************/
/******************************************************
*函 数 名：LCD_ShowChar8X16
*函数功能：显示一个字符
*参    数：uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：
*******************************************************/
//void LCD_ShowChar8X16(uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor)
//{
//	u8 row = 0,col = 0;
//	u16 pcolor = 0;
//	for(row = 0;row<16;row++)//遍历行
//	{
//		for(col=0;col<8;col++)//遍历列   
//		{
//			pcolor = (font[row]&(0x80>>col))?color:bgcolor;
//			LCD_Point(x0+col,y0+row,pcolor);
//		}
//	}
//}

/****************************************直接填数组名**************************************************/
/******************************************************
*函 数 名：LCD_ShowChar16X32
*函数功能：显示一个字符
*参    数：uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：
*******************************************************/
//void LCD_ShowChar16X32(uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor)
//{
//	u8 row = 0,col = 0;
//	u16 pcolor = 0;
//	u8 highbyte = 0,lowbyte = 0;
//	for(row = 0;row<32;row++)//遍历行
//	{
//		highbyte = font[2*row];
//		lowbyte = font[2*row+1];
//		for(col=0;col<16;col++)//遍历列   
//		{
//			if(col<8)//判断高字节
//				pcolor = (highbyte&(0x80>>col))?color:bgcolor;
//			else//判断低字节
//				pcolor = (lowbyte&(0x80>>(col-8)))?color:bgcolor;
//			LCD_Point(x0+col,y0+row,pcolor);
//		}
//	}
//}
/****************************************************************************************************************/


/******************************************************
*函 数 名：LCD_ShowChar8X16
*函数功能：显示一个字符
*参    数：uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：
*******************************************************/
void LCD_ShowChar8X16(uint16_t x0, uint16_t y0, char font, uint16_t color, uint16_t bgcolor)
{
	u8 row = 0, col = 0;
	u16 pcolor = 0;
	for (row = 0; row < 16; row++)//遍历行
	{
		for (col = 0; col < 8; col++)//遍历列   
		{
			pcolor = (ASCII_8X16[font - ' '][row] & (0x80 >> col)) ? color : bgcolor;
			LCD_Point(x0 + col, y0 + row, pcolor);
		}
	}
}


/******************************************************
*函 数 名：LCD_ShowChar16X32
*函数功能：显示一个字符
*参    数：uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：直接填字符
*******************************************************/
void LCD_ShowChar16X32(uint16_t x0, uint16_t y0, char font, uint16_t color, uint16_t bgcolor)
{
	u8 row = 0, col = 0;
	u16 pcolor = 0;
	u8 highbyte = 0, lowbyte = 0;
	for (row = 0; row < 32; row++)//遍历行
	{
		highbyte = ASCII_16X32[font - ' '][2 * row];
		lowbyte = ASCII_16X32[font - ' '][2 * row + 1];
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

/******************************************************
*函 数 名：LCD_ShowString16X32
*函数功能：显示一个字符串
*参    数：uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：直接填字符
*******************************************************/
void LCD_ShowString16X32(uint16_t x0, uint16_t y0, char* str, uint16_t color, uint16_t bgcolor)
{
	uint16_t x = x0;
	uint16_t y = y0;

	while (*str != '\0')
	{
		// 只显示可打印ASCII字符（空格到波浪号）
		if (*str >= ' ' && *str <= '~')
		{
			LCD_ShowChar16X32(x, y, *str, color, bgcolor);
			x += 16;  // 每个字符宽度16像素

			// 检查是否需要换行
			if (x > LCD_W - 16) {
				x = 0;
				y += 32;  // 32像素高度 + 行间距
			}
		}
		str++;  // 移动到下一个字符
	}
}

/******************************************************
*函 数 名：LCD_ShowImage
*函数功能：显示一个图片
*参    数：uint16_t x0, uint16_t y0,u8 wide u8 hige const uint8_t *image
*返 回 值：None
*备    注：
*******************************************************/
void LCD_ShowImage(uint16_t x0, uint16_t y0, u8 wide, u8 high, const uint8_t* image)
{
	LCD_Address_Set(x0, y0, x0 + wide - 1, y0 + high - 1);//确定显示位置
	for (u32 i = 0; i < wide * high; i++)
	{
		LCD_Send_Data((image[2 * i] << 8) | image[2 * i + 1]);
	}
}


/******************************************************
*函 数 名：LCD_ShowChinese16X16
*函数功能：显示一个汉字
*参    数：uint16_t x0, uint16_t y0, const char *ch, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：
*******************************************************/
u8 LCD_ShowChinese16X16(uint16_t x0, uint16_t y0, const char* ch, uint16_t color, uint16_t bgcolor)
{
	u16 pcolor = 0;
	// 在HZ数组中查找汉字
	for (uint8_t i = 0; i < sizeof(HZ16X16) / sizeof(HZ16X16[0]); i++)
	{
		// 比较汉字编码（GBK编码前2字节）
		if (ch[0] == HZ16X16[i][0] && ch[1] == HZ16X16[i][1])
		{
			const uint8_t* font = Chinese16X16[i]; // 获取传入字模数据
			for (uint8_t row = 0; row < 16; row++)
			{
				uint8_t highByte = font[row * 2];
				uint8_t lowByte = font[row * 2 + 1];

				for (uint8_t col = 0; col < 16; col++)//遍历列   
				{
					if (col < 8)//判断高字节
						pcolor = (highByte & (0x80 >> col)) ? color : bgcolor;
					else//判断低字节
						pcolor = (lowByte & (0x80 >> (col - 8))) ? color : bgcolor;
					LCD_Point(x0 + col, y0 + row, pcolor);
				}
			}
			return 1;//成功显示
		}
	}
	return 0;//显示失败
}

/******************************************************
*函 数 名：LCD_ShowChinese16X16s
*函数功能：显示一个汉字串
*参    数：uint16_t x0, uint16_t y0, const char *ch, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：
*******************************************************/
void LCD_ShowChinese16X16s(uint16_t x0, uint16_t y0, const char* str, uint16_t color, uint16_t bgcolor)
{
	uint16_t x = x0;
	uint16_t y = y0;
	for (uint8_t i = 0; str[i] != '\0' && str[i + 1] != '\0'; i += 2)
	{
		// 显示当前汉字
		if (LCD_ShowChinese16X16(x, y, &str[i], color, bgcolor))
		{
			x += 16; // 每个汉字宽16像素
			// 检查是否需要换行
			if (x > LCD_W - 16)
			{
				x = 0;
				y += 16;
			}
		}
	}
}


/******************************************************
*函 数 名：LCD_ShowChinese32X32
*函数功能：显示一个汉字
*参    数：uint16_t x0, uint16_t y0, const char *ch, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：
*******************************************************/
u8 LCD_ShowChinese32X32(uint16_t x0, uint16_t y0, const char* ch, uint16_t color, uint16_t bgcolor)
{
	u16 pcolor = 0;

	// 在HZ数组中查找汉字
	for (uint8_t i = 0; i < sizeof(HZ32X32) / sizeof(HZ32X32[0]); i++)
	{
		// 比较汉字编码（GBK编码前2字节）
		if (ch[0] == HZ32X32[i][0] && ch[1] == HZ32X32[i][1])
		{
			const uint8_t* font = Chinese32X32[i]; // 获取字模数据

			for (uint8_t row = 0; row < 32; row++)
			{
				// 每行4字节数据（32位）
				uint8_t byte1 = font[row * 4];     // 第1字节（前8列）
				uint8_t byte2 = font[row * 4 + 1]; // 第2字节（8-15列）
				uint8_t byte3 = font[row * 4 + 2]; // 第3字节（16-23列）
				uint8_t byte4 = font[row * 4 + 3]; // 第4字节（24-31列）

				for (uint8_t col = 0; col < 32; col++)
				{
					// 根据列位置选择对应的字节
					if (col < 8)
						pcolor = (byte1 & (0x80 >> col)) ? color : bgcolor;
					else if (col < 16)
						pcolor = (byte2 & (0x80 >> (col - 8))) ? color : bgcolor;
					else if (col < 24)
						pcolor = (byte3 & (0x80 >> (col - 16))) ? color : bgcolor;
					else
						pcolor = (byte4 & (0x80 >> (col - 24))) ? color : bgcolor;
					LCD_Point(x0 + col, y0 + row, pcolor);
				}
			}
			return 1; // 成功显示
		}
	}
	return 0; // 未找到对应汉字
}

/******************************************************
*函 数 名：LCD_ShowChinese32X32s
*函数功能：显示一个汉字串
*参    数：uint16_t x0, uint16_t y0, const char *ch, uint16_t color, uint16_t bgcolor
*返 回 值：None
*备    注：
*******************************************************/
void LCD_ShowChinese32X32s(uint16_t x0, uint16_t y0, const char* str, uint16_t color, uint16_t bgcolor)
{
	uint16_t x = x0;
	uint16_t y = y0;
	for (uint8_t i = 0; str[i] != '\0' && str[i + 1] != '\0'; i += 2)
	{
		// 显示当前汉字
		if (LCD_ShowChinese32X32(x, y, &str[i], color, bgcolor))
		{
			x += 32; // 每个汉字宽16像素
			// 检查是否需要换行
			if (x > LCD_W - 32)
			{
				x = 0;
				y += 32;
			}
		}
	}
}

/******************************************************
*函 数 名：LCD_ShowSuperString
*函数功能：显示一个字符串（汉字和英文可混合）
*参    数：uint16_t x0, uint16_t y0, const char *str, uint16_t color, uint16_t bgcolor, uint8_t fontSize
*返 回 值：None
*备    注：
*******************************************************/
void LCD_ShowSuperString(uint16_t x0, uint16_t y0, const char* str, uint16_t color, uint16_t bgcolor, uint8_t fontSize)
{
	uint16_t x = x0;
	uint16_t y = y0;
	uint8_t charWidth, charHeight, asciiWidth;

	// 设置字体尺寸参数
	if (fontSize == LCD_16X16) {
		charWidth = 16;    // 中文字符宽度
		charHeight = 16;   // 字符高度
		asciiWidth = 8;    // ASCII字符宽度
	}
	else {  // LCD_32X32
		charWidth = 32;
		charHeight = 32;
		asciiWidth = 16;
	}

	uint16_t lcdWidthLimit = LCD_W - charWidth;

	while (*str != '\0') {
		// 判断字符类型 (GBK中文最高位为1)
		if ((*str & 0x80) != 0) {
			// 处理中文字符(GBK)
			if (*(str + 1) != '\0') {
				if (fontSize == LCD_16X16) {
					if (LCD_ShowChinese16X16(x, y, str, color, bgcolor)) {
						x += charWidth;
					}
				}
				else {
					if (LCD_ShowChinese32X32(x, y, str, color, bgcolor)) {
						x += charWidth;
					}
				}
				str += 2;  // 跳过GBK编码的2字节
			}
			else {
				str++;  // 不完整的编码，跳过
			}
		}
		else {
			// 处理ASCII字符
			if (*str >= ' ' && *str <= '~') {
				if (fontSize == LCD_16X16) {
					LCD_ShowChar8X16(x, y, *str, color, bgcolor);
					x += asciiWidth;
				}
				else {
					LCD_ShowChar16X32(x, y, *str, color, bgcolor);
					x += asciiWidth;
				}
			}
			str++;
		}

		// 检查是否需要换行
		if (x > lcdWidthLimit) {
			x = 0;
			y += charHeight;
		}
	}
}



