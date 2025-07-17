#include "spi.h"

/******************************************************
*�� �� ����SPI2_Init
*�������ܣ�SPI2��ʼ��
*��    ����None
*�� �� ֵ��None
*��    ע����ST7789VM�ӿڣ�
*******************************************************/
void SPI2_Init(void)
{
	//1������GPIOBʱ�ӣ�SPI2ʱ��(APB1)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	//2����ʼ��GPIO����
	GPIO_InitTypeDef GPIO_InitStruct = {0};	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//3�������Ÿ���ΪSPI3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
						    
	//4����ʼ��SPI
	SPI_InitTypeDef SPI_InitStruct = {0};	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	  //�����ʷ�Ƶ
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;           				//ʱ����λ
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;             				//ʱ�Ӽ���
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;					 	//���ݴ�С
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;						//���ݵĴ��䷽��
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;							//����ģʽ
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;								//���ģʽ��������	
	SPI_Init(SPI2, &SPI_InitStruct);
	
	//5��ʹ��
	SPI_Cmd(SPI2, ENABLE);
}
/******************************************************
*�� �� ����SPI2_Transfer
*�������ܣ�SPI2����һ���ֽ�
*��    ����Data ����
*�� �� ֵ��None
*��    ע��
*******************************************************/
u8 SPI2_Transfer(u8 Data)
{
	//1�����͵��ֽ�����
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI2, Data);
	//2�����յ��ֽ�����
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI2);
}




/******************************************************
*�� �� ����SPI3_Init
*�������ܣ�SPI3��ʼ��
*��    ����None
*�� �� ֵ��None
*��    ע����RC522�ӿڣ�
*******************************************************/
void SPI3_Init(void)
{
	//1������GPIOCʱ�ӣ�SPI3ʱ��(APB1)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	//2����ʼ��GPIO����
	GPIO_InitTypeDef GPIO_InitStruct = {0};	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//3�������Ÿ���ΪSPI3
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
						    
	//4����ʼ��SPI
	SPI_InitTypeDef SPI_InitStruct = {0};	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//�����ʷ�Ƶ
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;           				//ʱ����λ
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;             				//ʱ�Ӽ���
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;					 	//���ݴ�С
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;						//���ݵĴ��䷽��
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;							//����ģʽ
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;								//���ģʽ��������	
	SPI_Init(SPI3, &SPI_InitStruct);
	
	//5��ʹ��
	SPI_Cmd(SPI3, ENABLE);
}
/******************************************************
*�� �� ����SPI3_Transfer
*�������ܣ�SPI3����һ���ֽ�
*��    ����Data ����
*�� �� ֵ��None
*��    ע��
*******************************************************/
u8 SPI3_Transfer(u8 Data)
{
	//1�����͵��ֽ�����
	while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI3, Data);
	//2�����յ��ֽ�����
	while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI3);
}



