#ifndef _LCD_H_
#define _LCD_H_

#include "stm32f4xx.h"                  // Device header
#include "spi.h"
#include "systick.h"

extern const unsigned char gImage_pic[115200];
extern const unsigned char ASCII_16X32[][64];
extern const unsigned char ASCII_8X16[][16];

#define LCD_16X16 	0
#define LCD_32X32 	1

//#define USE_HORIZONTAL 0  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

#define LCD_W 240
#define LCD_H 240


#define LCD_CS_H	   GPIO_SetBits(GPIOB,GPIO_Pin_12) 
#define LCD_CS_L       GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define LCD_RESET_H    GPIO_SetBits(GPIOB,GPIO_Pin_10) 
#define LCD_RESET_L    GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define LCD_DC_DATA    GPIO_SetBits(GPIOB,GPIO_Pin_14) 
#define LCD_DC_CMD     GPIO_ResetBits(GPIOB,GPIO_Pin_14)

#define LCD_LEDK_ON    GPIO_SetBits(GPIOB,GPIO_Pin_1) 
#define LCD_LEDK_OFF   GPIO_ResetBits(GPIOB,GPIO_Pin_1)


void LCD_Init(void);
void LCD_Clear(u16 color);
//void LCD_ShowChar8X16(uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor);
void LCD_ShowChar8X16(uint16_t x0, uint16_t y0, char font, uint16_t color, uint16_t bgcolor);
//void LCD_ShowChar16X32(uint16_t x0, uint16_t y0, const uint8_t *font, uint16_t color, uint16_t bgcolor);

void LCD_ShowChar16X32(uint16_t x0, uint16_t y0, char font, uint16_t color, uint16_t bgcolor);
void LCD_ShowString16X32(uint16_t x0, uint16_t y0, char* str, uint16_t color, uint16_t bgcolor);

u8 LCD_ShowChinese16X16(uint16_t x0, uint16_t y0, const char* ch, uint16_t color, uint16_t bgcolor);
void LCD_ShowChinese16X16s(uint16_t x0, uint16_t y0, const char* str, uint16_t color, uint16_t bgcolor);

u8 LCD_ShowChinese32X32(uint16_t x0, uint16_t y0, const char* ch, uint16_t color, uint16_t bgcolor);
void LCD_ShowChinese32X32s(uint16_t x0, uint16_t y0, const char* str, uint16_t color, uint16_t bgcolor);

void LCD_ShowImage(uint16_t x0, uint16_t y0, u8 wide, u8 high, const uint8_t* image);
void LCD_ShowSuperString(uint16_t x0, uint16_t y0, const char* str, uint16_t color, uint16_t bgcolor, uint8_t fontSize);

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 		 	 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)




#endif

