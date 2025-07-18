#include "lcd.h"
#include "LCDfont.h"

/******************************************************
*�� �� ����LCD_Pin_Init
*�������ܣ�LCD���ų�ʼ��
*��    ����None
*�� �� ֵ��None
*��    ע��
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

	// ����״̬
	LCD_RESET_H;
	LCD_CS_H;
	LCD_LEDK_OFF;
}


/******************************************************
*�� �� ����LCD_Send_Data8
*�������ܣ�LCD����һ���ֽ�����
*��    ����u8 dat ����
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_Send_Data8(u8 dat)
{
	// ����LCDͨ��
	LCD_CS_L;
	// ѡ���͵�������
	LCD_DC_DATA;
	// ������������
	SPI2_Transfer(dat);
	// �ر�ͨ��
	LCD_CS_H;
}

/******************************************************
*�� �� ����LCD_Send_Data
*�������ܣ�LCD���������ֽ�����
*��    ����u8 dat ����
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_Send_Data(u16 dat)
{
	// ����LCDͨ��
	LCD_CS_L;
	// ѡ���͵�������
	LCD_DC_DATA;
	// ������������
	SPI2_Transfer(dat >> 8);   //���͸�λ����
	SPI2_Transfer(dat & 0xff); //���͵�λ����	
	// �ر�ͨ��
	LCD_CS_H;
}
/******************************************************
*�� �� ����LCD_Send_Cmd
*�������ܣ�LCD����һ��ָ��
*��    ����u8 cmd ָ��
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_Send_Cmd(u8 cmd)
{
	LCD_CS_L;
	// ѡ���͵�������
	LCD_DC_CMD;
	// ������������
	SPI2_Transfer(cmd);
	// �ر�ͨ��
	LCD_CS_H;
}

/******************************************************
*�� �� ����LCD_Init
*�������ܣ�LCD��ʼ��
*��    ����None
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_Init(void)
{
	LCD_Pin_Init();// ��ʼ��GPIO
	SPI2_Init();

	/* ��λST7789VM������ */
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

	LCD_LEDK_ON;	// �򿪱���

	LCD_Clear(WHITE);
}

/******************************************************
*�� �� ����LCD_Address_Set
*�������ܣ�LCD�������
*��    ����u16 x1, u16 y1, u16 x2, u16 y2  ��ʼx ��ʼy  ����x  ����y
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_Send_Cmd(0x2a);//�е�ַ����
	LCD_Send_Data(x1);
	LCD_Send_Data(x2);
	LCD_Send_Cmd(0x2b);//�е�ַ����
	LCD_Send_Data(y1);
	LCD_Send_Data(y2);
	LCD_Send_Cmd(0x2c);//��ɫָ��
}

/******************************************************
*�� �� ����LCD_Clear
*�������ܣ�LCD��������
*��    ����u16 color
*�� �� ֵ��None
*��    ע��
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
*�� �� ����LCD_ClearArea
*�������ܣ�LCD�ֲ���������
*��    ����u8 x0,u8 y0,u8 wide,u8 high,u16 color
*�� �� ֵ��None
*��    ע��
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
*�� �� ����LCD_Point
*�������ܣ�LCD���
*��    ����u16 color
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_Point(u16 x, u16 y, u16 color)
{
	LCD_Address_Set(x, y, x, y);
	LCD_Send_Data(color);
}

/****************************************ֱ����������**************************************************/
/******************************************************
*�� �� ����LCD_ShowChar8X16
*�������ܣ���ʾһ���ַ�
*��    ����uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��
*******************************************************/
//void LCD_ShowChar8X16(uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor)
//{
//	u8 row = 0,col = 0;
//	u16 pcolor = 0;
//	for(row = 0;row<16;row++)//������
//	{
//		for(col=0;col<8;col++)//������   
//		{
//			pcolor = (font[row]&(0x80>>col))?color:bgcolor;
//			LCD_Point(x0+col,y0+row,pcolor);
//		}
//	}
//}

/****************************************ֱ����������**************************************************/
/******************************************************
*�� �� ����LCD_ShowChar16X32
*�������ܣ���ʾһ���ַ�
*��    ����uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��
*******************************************************/
//void LCD_ShowChar16X32(uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor)
//{
//	u8 row = 0,col = 0;
//	u16 pcolor = 0;
//	u8 highbyte = 0,lowbyte = 0;
//	for(row = 0;row<32;row++)//������
//	{
//		highbyte = font[2*row];
//		lowbyte = font[2*row+1];
//		for(col=0;col<16;col++)//������   
//		{
//			if(col<8)//�жϸ��ֽ�
//				pcolor = (highbyte&(0x80>>col))?color:bgcolor;
//			else//�жϵ��ֽ�
//				pcolor = (lowbyte&(0x80>>(col-8)))?color:bgcolor;
//			LCD_Point(x0+col,y0+row,pcolor);
//		}
//	}
//}
/****************************************************************************************************************/


/******************************************************
*�� �� ����LCD_ShowChar8X16
*�������ܣ���ʾһ���ַ�
*��    ����uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_ShowChar8X16(uint16_t x0, uint16_t y0, char font, uint16_t color, uint16_t bgcolor)
{
	u8 row = 0, col = 0;
	u16 pcolor = 0;
	for (row = 0; row < 16; row++)//������
	{
		for (col = 0; col < 8; col++)//������   
		{
			pcolor = (ASCII_8X16[font - ' '][row] & (0x80 >> col)) ? color : bgcolor;
			LCD_Point(x0 + col, y0 + row, pcolor);
		}
	}
}


/******************************************************
*�� �� ����LCD_ShowChar16X32
*�������ܣ���ʾһ���ַ�
*��    ����uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��ֱ�����ַ�
*******************************************************/
void LCD_ShowChar16X32(uint16_t x0, uint16_t y0, char font, uint16_t color, uint16_t bgcolor)
{
	u8 row = 0, col = 0;
	u16 pcolor = 0;
	u8 highbyte = 0, lowbyte = 0;
	for (row = 0; row < 32; row++)//������
	{
		highbyte = ASCII_16X32[font - ' '][2 * row];
		lowbyte = ASCII_16X32[font - ' '][2 * row + 1];
		for (col = 0; col < 16; col++)//������   
		{
			if (col < 8)//�жϸ��ֽ�
				pcolor = (highbyte & (0x80 >> col)) ? color : bgcolor;
			else//�жϵ��ֽ�
				pcolor = (lowbyte & (0x80 >> (col - 8))) ? color : bgcolor;
			LCD_Point(x0 + col, y0 + row, pcolor);
		}
	}
}

/******************************************************
*�� �� ����LCD_ShowString16X32
*�������ܣ���ʾһ���ַ���
*��    ����uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��ֱ�����ַ�
*******************************************************/
void LCD_ShowString16X32(uint16_t x0, uint16_t y0, char* str, uint16_t color, uint16_t bgcolor)
{
	uint16_t x = x0;
	uint16_t y = y0;

	while (*str != '\0')
	{
		// ֻ��ʾ�ɴ�ӡASCII�ַ����ո񵽲��˺ţ�
		if (*str >= ' ' && *str <= '~')
		{
			LCD_ShowChar16X32(x, y, *str, color, bgcolor);
			x += 16;  // ÿ���ַ����16����

			// ����Ƿ���Ҫ����
			if (x > LCD_W - 16) {
				x = 0;
				y += 32;  // 32���ظ߶� + �м��
			}
		}
		str++;  // �ƶ�����һ���ַ�
	}
}

/******************************************************
*�� �� ����LCD_ShowImage
*�������ܣ���ʾһ��ͼƬ
*��    ����uint16_t x0, uint16_t y0,u8 wide u8 hige const uint8_t *image
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_ShowImage(uint16_t x0, uint16_t y0, u8 wide, u8 high, const uint8_t* image)
{
	LCD_Address_Set(x0, y0, x0 + wide - 1, y0 + high - 1);//ȷ����ʾλ��
	for (u32 i = 0; i < wide * high; i++)
	{
		LCD_Send_Data((image[2 * i] << 8) | image[2 * i + 1]);
	}
}


/******************************************************
*�� �� ����LCD_ShowChinese16X16
*�������ܣ���ʾһ������
*��    ����uint16_t x0, uint16_t y0, const char *ch, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��
*******************************************************/
u8 LCD_ShowChinese16X16(uint16_t x0, uint16_t y0, const char* ch, uint16_t color, uint16_t bgcolor)
{
	u16 pcolor = 0;
	// ��HZ�����в��Һ���
	for (uint8_t i = 0; i < sizeof(HZ16X16) / sizeof(HZ16X16[0]); i++)
	{
		// �ȽϺ��ֱ��루GBK����ǰ2�ֽڣ�
		if (ch[0] == HZ16X16[i][0] && ch[1] == HZ16X16[i][1])
		{
			const uint8_t* font = Chinese16X16[i]; // ��ȡ������ģ����
			for (uint8_t row = 0; row < 16; row++)
			{
				uint8_t highByte = font[row * 2];
				uint8_t lowByte = font[row * 2 + 1];

				for (uint8_t col = 0; col < 16; col++)//������   
				{
					if (col < 8)//�жϸ��ֽ�
						pcolor = (highByte & (0x80 >> col)) ? color : bgcolor;
					else//�жϵ��ֽ�
						pcolor = (lowByte & (0x80 >> (col - 8))) ? color : bgcolor;
					LCD_Point(x0 + col, y0 + row, pcolor);
				}
			}
			return 1;//�ɹ���ʾ
		}
	}
	return 0;//��ʾʧ��
}

/******************************************************
*�� �� ����LCD_ShowChinese16X16s
*�������ܣ���ʾһ�����ִ�
*��    ����uint16_t x0, uint16_t y0, const char *ch, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_ShowChinese16X16s(uint16_t x0, uint16_t y0, const char* str, uint16_t color, uint16_t bgcolor)
{
	uint16_t x = x0;
	uint16_t y = y0;
	for (uint8_t i = 0; str[i] != '\0' && str[i + 1] != '\0'; i += 2)
	{
		// ��ʾ��ǰ����
		if (LCD_ShowChinese16X16(x, y, &str[i], color, bgcolor))
		{
			x += 16; // ÿ�����ֿ�16����
			// ����Ƿ���Ҫ����
			if (x > LCD_W - 16)
			{
				x = 0;
				y += 16;
			}
		}
	}
}


/******************************************************
*�� �� ����LCD_ShowChinese32X32
*�������ܣ���ʾһ������
*��    ����uint16_t x0, uint16_t y0, const char *ch, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��
*******************************************************/
u8 LCD_ShowChinese32X32(uint16_t x0, uint16_t y0, const char* ch, uint16_t color, uint16_t bgcolor)
{
	u16 pcolor = 0;

	// ��HZ�����в��Һ���
	for (uint8_t i = 0; i < sizeof(HZ32X32) / sizeof(HZ32X32[0]); i++)
	{
		// �ȽϺ��ֱ��루GBK����ǰ2�ֽڣ�
		if (ch[0] == HZ32X32[i][0] && ch[1] == HZ32X32[i][1])
		{
			const uint8_t* font = Chinese32X32[i]; // ��ȡ��ģ����

			for (uint8_t row = 0; row < 32; row++)
			{
				// ÿ��4�ֽ����ݣ�32λ��
				uint8_t byte1 = font[row * 4];     // ��1�ֽڣ�ǰ8�У�
				uint8_t byte2 = font[row * 4 + 1]; // ��2�ֽڣ�8-15�У�
				uint8_t byte3 = font[row * 4 + 2]; // ��3�ֽڣ�16-23�У�
				uint8_t byte4 = font[row * 4 + 3]; // ��4�ֽڣ�24-31�У�

				for (uint8_t col = 0; col < 32; col++)
				{
					// ������λ��ѡ���Ӧ���ֽ�
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
			return 1; // �ɹ���ʾ
		}
	}
	return 0; // δ�ҵ���Ӧ����
}

/******************************************************
*�� �� ����LCD_ShowChinese32X32s
*�������ܣ���ʾһ�����ִ�
*��    ����uint16_t x0, uint16_t y0, const char *ch, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_ShowChinese32X32s(uint16_t x0, uint16_t y0, const char* str, uint16_t color, uint16_t bgcolor)
{
	uint16_t x = x0;
	uint16_t y = y0;
	for (uint8_t i = 0; str[i] != '\0' && str[i + 1] != '\0'; i += 2)
	{
		// ��ʾ��ǰ����
		if (LCD_ShowChinese32X32(x, y, &str[i], color, bgcolor))
		{
			x += 32; // ÿ�����ֿ�16����
			// ����Ƿ���Ҫ����
			if (x > LCD_W - 32)
			{
				x = 0;
				y += 32;
			}
		}
	}
}

/******************************************************
*�� �� ����LCD_ShowSuperString
*�������ܣ���ʾһ���ַ��������ֺ�Ӣ�Ŀɻ�ϣ�
*��    ����uint16_t x0, uint16_t y0, const char *str, uint16_t color, uint16_t bgcolor, uint8_t fontSize
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_ShowSuperString(uint16_t x0, uint16_t y0, const char* str, uint16_t color, uint16_t bgcolor, uint8_t fontSize)
{
	uint16_t x = x0;
	uint16_t y = y0;
	uint8_t charWidth, charHeight, asciiWidth;

	// ��������ߴ����
	if (fontSize == LCD_16X16) {
		charWidth = 16;    // �����ַ����
		charHeight = 16;   // �ַ��߶�
		asciiWidth = 8;    // ASCII�ַ����
	}
	else {  // LCD_32X32
		charWidth = 32;
		charHeight = 32;
		asciiWidth = 16;
	}

	uint16_t lcdWidthLimit = LCD_W - charWidth;

	while (*str != '\0') {
		// �ж��ַ����� (GBK�������λΪ1)
		if ((*str & 0x80) != 0) {
			// ���������ַ�(GBK)
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
				str += 2;  // ����GBK�����2�ֽ�
			}
			else {
				str++;  // �������ı��룬����
			}
		}
		else {
			// ����ASCII�ַ�
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

		// ����Ƿ���Ҫ����
		if (x > lcdWidthLimit) {
			x = 0;
			y += charHeight;
		}
	}
}



