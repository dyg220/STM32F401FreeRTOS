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


/******************************************************
*�� �� ����LCD_ShowCharX16
*�������ܣ���ʾһ���ַ�
*��    ����uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_ShowChar8X16(uint16_t x0, uint16_t y0, const uint8_t* font, uint16_t color, uint16_t bgcolor)
{
	u8 row = 0, col = 0;
	u16 pcolor = 0;
	for (row = 0; row < 16; row++)//������
	{
		for (col = 0; col < 8; col++)//������   
		{
			pcolor = (font[row] & (0x80 >> col)) ? color : bgcolor;
			LCD_Point(x0 + col, y0 + row, pcolor);
		}
	}
}


/******************************************************
*�� �� ����LCD_ShowChar16X32
*�������ܣ���ʾһ���ַ�
*��    ����uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor
*�� �� ֵ��None
*��    ע��
*******************************************************/
void LCD_ShowChar16X32(uint16_t x0, uint16_t y0, const uint8_t* font, uint16_t color, uint16_t bgcolor)
{
	u8 row = 0, col = 0;
	u16 pcolor = 0;
	u8 highbyte = 0, lowbyte = 0;
	for (row = 0; row < 32; row++)//������
	{
		highbyte = font[2 * row];
		lowbyte = font[2 * row + 1];
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









