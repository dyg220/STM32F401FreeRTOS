#include "w25q64.h"

/**************************************************************
*�� �� ����W25Q64_Init
*�������ܣ�W25Q64��ʼ��
*��    ����None
*�� �� ֵ��None
*��    ע��CS---PB14
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
	Write_Status(0x00);//������п�д����
}

/**************************************************************
*�������ܣ�дʹ�ܺ���
*��    ����None
*�� �� ֵ��None
*��    ע��0x06  "ҳ���" "��������" "��������" "оƬ����" "д����״̬�Ĵ���"
**************************************************************/
void Write_Enable(void)
{
	W25Q64_CS_L;
	SPI1_Transfer(0x06);
	W25Q64_CS_H;
}

/**************************************************************
*�������ܣ���״̬�Ĵ�������
*��    ����None
*�� �� ֵ��״̬�Ĵ�����ֵ
*��    ע��0x05  
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
*�������ܣ�д����״̬�Ĵ�������
*��    �����Ĵ���������Ϣ
*�� �� ֵ��None
*��    ע��0x01  
**************************************************************/
void Write_Status(u8 data)
{
	//дʹ��
	Write_Enable();
	W25Q64_CS_L;
	SPI1_Transfer(0x01);
	SPI1_Transfer(data);
	W25Q64_CS_H;
	while(Read_Status()&(1<<0));//�ȴ�BUSY��0

}

/**************************************************************
*�������ܣ�����������
*��    ����addr  ���Ķ�  u32 len ���೤  u8*buf ������������ݵĿռ�
*�� �� ֵ��None
*��    ע��0x03
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
*�������ܣ�ҳд����
*��    ����addr  ����д  u32 len   u8*buf ������������ݵĿռ�
*�� �� ֵ��None
*��    ע��0x02
**************************************************************/
void W25Q64_Write_Page(u32 addr,u32 len, u8*buf)
{
	if((addr/256) != ((addr+len-1)/256))
	{
		printf("���ȳ�����ǰҳʣ��\r\n");
		return ;
	}
	//дʹ��
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
	while(Read_Status()&(1<<0));//�ȴ�BUSY��0
}

/**************************************************************
*�������ܣ���ҳд����
*��    ����addr  ����д  u32 len   u8*buf ������������ݵĿռ�
*�� �� ֵ��None
*��    ע��0x02
**************************************************************/
void W25Q64_Write_Pages(u32 addr,u32 len, u8*buf)
{
	u32 remain = 0;
	remain = 256-(addr%256);//��ǰҳʣ��Ŀ�д�ֽ���
	if(len<=remain)
	{
		W25Q64_Write_Page(addr,len,buf);
		return ;
	}
	else
	{
		//�ѵ�ǰҳд��
		W25Q64_Write_Page(addr,remain,buf);
		addr = addr+remain;//�洢�ռ�ĵ�ַƫ��
		buf = buf+remain;//���ݵ�ַ��ƫ��
		len = len-remain;//ʣ�����ݳ���

		//ʣ������ݳ��ȴ��ڵ���8
		while(len>=256)
		{
			W25Q64_Write_Page(addr,256,buf);
			addr = addr+256;//�洢�ռ�ĵ�ַƫ��
			buf = buf+256;//���ݵ�ַ��ƫ��
			len = len-256;//ʣ�����ݳ���
		}
		//ʣ������һ��С��8������Ϊ0
		if(len != 0)
		{
			W25Q64_Write_Page(addr,len,buf);
		}
	}
}

/**************************************************************
*�������ܣ�������������
*��    ����addr ���Ŀ�ʼ����  (4096 Byte)
*�� �� ֵ��None
*��    ע��0x20
**************************************************************/
void W25Q64_Sector_Erase(u32 addr)
{
	//дʹ��
	Write_Enable();
	W25Q64_CS_L;
	SPI1_Transfer(0x20);
	SPI1_Transfer(addr>>16);
	SPI1_Transfer(addr>>8);
	SPI1_Transfer(addr);
	W25Q64_CS_H;
	while(Read_Status()&(1<<0));//�ȴ�BUSY��0
}



/**************************************************************
*�������ܣ�������������
*��    ����addr ���Ŀ�ʼ����  
*�� �� ֵ��None
*��    ע��0xd8
**************************************************************/
void W25Q64_block_Erase(u32 addr)
{
	//дʹ��
	Write_Enable();
	W25Q64_CS_L;
	SPI1_Transfer(0xd8);
	SPI1_Transfer(addr>>16);
	SPI1_Transfer(addr>>8);
	SPI1_Transfer(addr);
	W25Q64_CS_H;
	while(Read_Status()&(1<<0));//�ȴ�BUSY��0
}



/***************************************************
��������W25Q64_Erase_Blockn
��  �ܣ������
��  ����u32 addr,u8 num
����ֵ����
��  ע��addr:��һ���ַ
		num:�������ٿ�
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
*�������ܣ�оƬ��������
*��    ����None
*�� �� ֵ��None
*��    ע��0xC7
**************************************************************/
void W25Q64_Chip_Erase(void)
{
	//дʹ��
	Write_Enable();
	W25Q64_CS_L;
	SPI1_Transfer(0xC7);
	W25Q64_CS_H;
	while(Read_Status()&(1<<0));//�ȴ�BUSY��0
}


u8 zk_flag = 0;

/***************************************************
��������W25Q64_Zk_Updata
��  �ܣ��ֿ����
��  ������
����ֵ����
��  ע��
***************************************************/
void W25Q64_Zk_Updata(void)
{
	u8 keynum=0;
	keynum = Key_Scan();
	if(keynum == 2)   						//��¼�ֿ�Ĵ�����־
	{
		printf("��ʼ�����ռ�,���Ժ�....\r\n");
		//0x00000~x7fffff
		//�ӵڶ��鿪ʼ��¼
		W25Q64_Erase_Blockn(0x020000,29);
		printf("�����ռ����,�뿪ʼ��¼�ֿ�\r\n");
		zk_flag=1;             	//��ʼ�����ֿ��־
	}
}



