#include "cy8cmbr3116.h"


//�Ĵ�����������
const unsigned char CY8CMBR3116_RegPara[128] = {

	0xFFu, 0x0Fu, 0xFFu, 0x0Fu, 0x00u, 0x00u, 0x00u, 0x00u,
	0xAAu, 0xAAu, 0xAAu, 0x00u, 0x80u, 0x80u, 0x80u, 0x80u,
	0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
	0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu, 0x08u, 0x00u, 0x00u, 0x00u,
	0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x80u,
	0x05u, 0x00u, 0x00u, 0x02u, 0x00u, 0x02u, 0x00u, 0x00u,
	0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Eu, 0x1Eu, 0x00u,
	0x00u, 0x1Eu, 0x1Eu, 0x00u, 0x00u, 0x00u, 0x01u, 0x01u,
	0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
	0xFFu, 0x00u, 0x00u, 0x00u, 0x20u, 0x03u, 0x01u, 0x63u,
	0x00u, 0x37u, 0x03u, 0x00u, 0x00u, 0x0Au, 0x00u, 0x00u,
	0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
	0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
	0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
	0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
	0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xA1u, 0xD5u
};


/******************************************************
*�� �� ����CY8CMBR3116_Init
*�������ܣ�CY8CMBR3116��ʼ��
*��    ����None
*�� �� ֵ��None
*��    ע��
*******************************************************/
void CY8CMBR3116_Init(void)
{
	//IIC��ʼ������
	IIC1_Init();
	//��λ��������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//Ӳ����λ
	CY8CMBR3116_RST_H;
	delay_xms(1);
	CY8CMBR3116_RST_L;
	delay_xms(1);
	CY8CMBR3116_RST_H;
	delay_xms(15);

	//�Ĵ�����ʼ������
	CY8CMBR3116_regcfg(1);
}


/******************************************************
*�� �� ����CY8CMBR3116_Start
*�������ܣ�CY8CMBR3116��ʼ����
*��    ����u8 flag   ��дλ  ����1   д��0
*�� �� ֵ��None
*��    ע��
*******************************************************/
u8 CY8CMBR3116_Start(u8 flag)
{
	u8 ack = 0;
	do {
		IIC1_Start();
		IIC1_SendByte(CY8CMBR3116_ADDR + flag);//����������ַ
		ack = IIC1_RecvAck();
	} while (ack);
	return 0;
}


/******************************************************
*�� �� CY8CMBR3116_WriteData
*�������ܣ�CY8CMBR3116д����
*��    ����u8 addr,u8 *WBuf,u8 len    ������д   дʲô  д����
*�� �� ֵ��0�ɹ�
*��    ע��
*******************************************************/
u8 CY8CMBR3116_WriteData(u8 addr, u8* WBuf, u8 len)
{
	u8 ack = 0;
	ack = CY8CMBR3116_Start(0);//����������ַд
	if (ack) { IIC1_Stop();	return 1; }

	IIC1_SendByte(addr);//���ͼĴ�����ַ
	delay_us(500);
	ack = IIC1_RecvAck();
	if (ack) { IIC1_Stop();	return 2; }
	delay_us(500);

	//����д
	while (len--)
	{
		IIC1_SendByte(*WBuf);//д����
		delay_us(500);
		ack = IIC1_RecvAck();
		if (ack) { IIC1_Stop();	return 3; }
		delay_us(500);
		WBuf++;
	}
	IIC1_Stop();
	return 0;
}



/******************************************************
*�� �� ����CY8CMBR3116_Start
*�������ܣ�CY8CMBR3116������
*��    ����u8 addr,u8 *buf,u8 len    ���Ķ�   ������ݵ�����  ������
*�� �� ֵ��0�ɹ�
*��    ע��
*******************************************************/
u8 CY8CMBR3116_ReadData(u8 addr, u8* RBuf, u8 len)
{
	u8 ack = 0;
	ack = CY8CMBR3116_Start(0);//����������ַд
	if (ack) { IIC1_Stop();	return 1; }

	IIC1_SendByte(addr);//���ͼĴ�����ַ����������ָ�룩
	ack = IIC1_RecvAck();
	if (ack) { IIC1_Stop();	return 2; }
	IIC1_Stop();

	ack = CY8CMBR3116_Start(1);//����������ַ��
	if (ack) { IIC1_Stop();	return 3; }

	while (--len)
	{
		*RBuf = IIC1_RecvByte();//��ȡ����
		IIC1_SendAck(ACK);
		RBuf++;
	}
	*RBuf = IIC1_RecvByte();//��ȡ����
	IIC1_SendAck(NO_ACK);
	IIC1_Stop();
	return 0;
}

/******************************************************
*�� �� ����CY8CMBR3116_WrCmd
*�������ܣ�����Ĵ�������
*��    ����u8 cmd
*�� �� ֵ��u8    ����IIC��У��ֵ
*��    ע��
*******************************************************/
u8 CY8CMBR3116_WrCmd(u8 cmd)
{
	u8 ack = 0;
	ack = CY8CMBR3116_WriteData(CTRL_CMD, &cmd, 1);
	return ack;
}

/******************************************************
*�� �� ����CY8CMBR3116_RdCmdErr
*�������ܣ���ָ��ִ�еĴ���״̬
*��    ����None
*�� �� ֵ��u8
*��    ע��0: ָ��ִ�гɹ�
		   0xfd��������д��ʧ��
		   0xfe��CONFIG_CRC ���洢������ CRC У��������õ������� CRC У��Ͳ�һ��
		   0xff��ָ����Ч
*******************************************************/
u8 CY8CMBR3116_RdCmdErr(void)
{
	u8 err_val = 0;
	CY8CMBR3116_WriteData(CTRL_CMD_ERR, &err_val, 1);
	return err_val;
}



/******************************************************
*�� �� ����CY8CMBR3116_RdCfgStatus
*�������ܣ���ȡ����״̬
*��    ����None
*�� �� ֵ������ֵ��0���ǳ�������     ����ֵ��1�ǳ�������
*��    ע��
*******************************************************/
u8 CY8CMBR3116_RdCfgStatus(void)
{
	u8 sta = 0;
	CY8CMBR3116_ReadData(SYSTEM_STATUS, &sta, 1);
	return sta;
}

/******************************************************
*�� �� ����CY8CMBR3116_regcfg
*�������ܣ�����CY8CMBR3116�Ĵ���
*��    ����u8 dfg_flag     1:ǿ������    0��������������Ƿ�����
*�� �� ֵ������ֵ��0 ���óɹ�    ����ֵ��1 ʹ��ԭ����
										 2���Ĵ�������ֵ����ʧ��
										 3����������ʧ��
										 4����������ʧ��
*��    ע�� ���üĴ���
			���ͱ���ָ��
			����������Ϣ��Ҫ300ms
			��λ��Ч
*******************************************************/
u8 CY8CMBR3116_regcfg(u8 dfg_flag)
{
	u8 reg_sta = 0, ack = 0, reg_init_cont = 0;
ReInit:
	//����������״̬�Ĵ���
	reg_sta = CY8CMBR3116_RdCfgStatus();

	if (reg_sta == 1)
		printf("��оƬ�ǳ�������\r\n");
	else
		printf("��оƬ���ǳ�������\r\n");

	//����Ҫ����
	if (dfg_flag == 0 && reg_sta == 0)
	{
		return 1;     //������
	}
	//����
	for (u8 i = 0; i < 10; i++)
	{
		ack = CY8CMBR3116_WriteData(REGCFG_ADDR, (u8*)CY8CMBR3116_RegPara, 128);
		if (ack == 0) break;//�ɹ����˳�
	}
	if (ack == 0)
	{
		printf("�Ĵ��������������ͳɹ�\r\n");
	}
	else
	{
		printf("�Ĵ���������������ʧ��\r\n");
		return 2;  //��������ʧ��
	}

	//���浽����ʧ����
	for (u8 i = 0; i < 10; i++)
	{
		ack = CY8CMBR3116_WrCmd(2);
		if (ack == 0) break;//�ɹ����˳�
	}
	if (ack == 1) return 3;
	delay_ms(300);

	//���������״̬
	u8 err = 0;
	err = CY8CMBR3116_RdCmdErr();
	switch (err)
	{
	case 0:printf("ָ��ִ�гɹ�\r\n"); break;
	case 0xfd:printf("����д��ʧ��\r\n"); break;
	case 0xfe:printf("CRCУ��ʧ��\r\n"); break;
	case 0xff:printf("ָ����Ч\r\n"); break;
	}
	reg_init_cont++;
	if (err != 0)
	{
		if (reg_init_cont < 10)  goto ReInit;
		printf("����ʧ��\r\n");
		return 4;
	}
	//�����λ
	ack = CY8CMBR3116_WrCmd(0xff);
	delay_ms(15);

	printf("�������óɹ�\r\n");
	return 0;
}


/******************************************************
*�� �� ����CY8CMBR3116_Read_Key
*�������ܣ���ȡ����״̬�Ĵ�����ֵ���ж�Ϊĳ������
*��    ����None
*�� �� ֵ��u8
*��    ע��
*******************************************************/
u8 CY8CMBR3116_Read_Key(void)
{
	u8 ack = 0;
	u8 key_val[2] = { 0 };
	u16 key_temp = 0;
	ack = CY8CMBR3116_ReadData(BUTTON_STA, key_val, 2);
	if (ack)printf("��ֵ��ȡʧ��\r\n");

	key_temp = (key_val[1] << 8) | key_val[0];//�ϳɼĴ���ԭʼ���ݣ���Ϊ�Ĵ����ǵ��ֽ����ȣ�
	switch (key_temp)
	{
	case 0x0008: return '1';
	case 0x0010: return '2';
	case 0x0100: return '3';
	case 0x0004: return '4';
	case 0x0020: return '5';
	case 0x0200: return '6';
	case 0x0001: return '7';
	case 0x0080: return '8';
	case 0x0800: return '9';
	case 0x0002: return '*';
	case 0x0040: return '0';
	case 0x0400: return '#';
	default:return 0xff;
	}
}

/******************************************************
*�� �� ����CY8CMBR3116_Key_Scan
*�������ܣ�����ɨ�躯��
*��    ����None
*�� �� ֵ��u8
*��    ע��
*******************************************************/
u8 CY8CMBR3116_Key_Scan(void)
{
	static u8 CY8CMBR3116_flag = 1;
	u8 CY8CMBR3116_val = 0;
	u8 CY8CMBR3116_key = 0xff;
	CY8CMBR3116_val = CY8CMBR3116_Read_Key();
	if (CY8CMBR3116_val != 0xff && CY8CMBR3116_flag)
	{
		CY8CMBR3116_key = CY8CMBR3116_val;
		CY8CMBR3116_flag = 0;
		Voice_SendCmd(0x2e);//��������"��"
	}
	if (CY8CMBR3116_val == 0xff)
	{
		CY8CMBR3116_flag = 1;
	}
	return CY8CMBR3116_key;
}

//CY_INFO CY_password = {0};

//void CY8CMBR3116_Open_Door(void)
//{
//	u8 keynum = 0;
//	keynum = CY8CMBR3116_Key_Scan();
//	if(keynum != 0xff)
//	{
//		if(keynum == '#')
//		{
//			CY_password.password[CY_password.index] = '\0';
//		}
//	}
//}




