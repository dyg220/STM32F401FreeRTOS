#include "rfid.h"


/******************************************************
*�� �� ����RFID_Init
*�������ܣ�RFID��ʼ��
*��    ����None
*�� �� ֵ��None
*��    ע��
*******************************************************/
void RFID_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	RFID_CS_H;
	RFID_RST_H;

	SPI3_Init();               //SPI��ʼ������
	PcdReset();                //��Ӳ��λ
	PcdAntennaOff();		   //�ر�����
	PcdAntennaOn();		       //��������	
	M500PcdConfigISOType('A'); //AЭ������
}

/******************************************************
*�� �� ����PCD_distinguish_PICC
*�������ܣ�PCDʶ��typeA�࿨
*��    ����u8 *pSnr      ʶ�𵽵�4�ֽڵ�ID��
*�� �� ֵ������ֵ�׶δ���
*��    ע��	pTagType[OUT]:��Ƭ���ʹ���(���������)
			0x4400=  Mifare_UltraLight
			0x0400 = Mifare One (S50)
			0x0200 = Mifare One (S70)
			0x0800 = Mifare Pro(X)
			0x4403 = Mifare_DESFire
*******************************************************/
u8 PCD_distinguish_PICC(u8* pSnr)
{
	uint8_t pTagType[2] = { 0 }; //�濨����������
	//1.Ѱ��
	if (PcdRequest(PICC_REQIDL, pTagType) != MI_OK)
	{
		return 1;
	}
	//2.����ͻ
	if (PcdAnticoll(pSnr) != MI_OK)
	{
		printf("����ͻʧ��\r\n");
		return 2;
	}
	//3.ѡ��
	if (PcdSelect(pSnr) != MI_OK)
	{
		printf("ѡ��ʧ��\r\n");
		return 3;
	}
	Voice_SendCmd(0x2e);//��������"��"
	printf("������:0x%02x%02x\r\n", pTagType[0], pTagType[1]);
	printf("����:%#x %#x %#x %#x\r\n", pSnr[0], pSnr[1], pSnr[2], pSnr[3]);

	//˯��
	PcdHalt();
	return MI_OK;
}

u8 picc_passward[6] = { 0xff,0xff,0xff,0xff,0xff,0xff };//������-��ʼ����--�հ׿�

///******************************************************
//*�� �� ����WriteCardData
//*�������ܣ�����Ƭ��ĳ���ַд����
//*��    ����u8 addr,u8 *data,u8 *pSnr
//*�� �� ֵ��0�ɹ� ����ʧ��
//*��    ע����ľ��Ե�ַ0~63 ע�ⲻд�����ƿ���
//           Ҫд������Ϊ16�ֽ�
//*******************************************************/
//u8 WriteCardData(u8 addr,u8 *data,u8 *pSnr)
//{
//	uint8_t pTagType[2] = {0}; //�濨����������
//	//1.Ѱ��
//	if(PcdRequest(PICC_REQIDL,pTagType)!=MI_OK)
//	{
//		 return 1;
//	}
//	//2.����ͻ
//	if(PcdAnticoll(pSnr)!=MI_OK)
//	{
//		return 2;
//	}
//	//3.ѡ��
//	if(PcdSelect(pSnr)!=MI_OK)
//	{
//		return 3;
//	}
//	Voice_SendCmd(0x2e);//��������"��"
//	//4.������֤
//	if(PcdAuthState(PICC_AUTHENT1A,(addr/4)*4+3,picc_passward,pSnr)!=MI_OK)
//	{
//		printf("�ȶ�����ʧ��\r\n");
//		PcdHalt();//����
//		return 4;
//	}
//	//5.д����
//	if(PcdWrite(addr,data)!=MI_OK)
//	{
//		printf("д����ʧ��\r\n");
//		PcdHalt();//����
//		return 5;
//	}
//	PcdHalt();//����
//	printf("����д��ɹ�\r\n");
//	
//	return MI_OK;
//}


/******************************************************
*�� �� ����WriteCardData
*�������ܣ�����Ƭ��ĳ���ַд����
*��    ����u8 addr,u8 *data,u8 *pSnr
*�� �� ֵ��0�ɹ� ����ʧ��
*��    ע����ľ��Ե�ַ0~63 ע�ⲻд�����ƿ���
		   Ҫд������Ϊ16�ֽ�
*******************************************************/
u8 WriteCardData(u8 addr, u8* data, u8* pSnr)
{
	//1.ѡ��
	if (PCD_distinguish_PICC(pSnr) != MI_OK)//ѡ��
	{
		return MI_ERR;
	}
	//4.������֤
	if (PcdAuthState(PICC_AUTHENT1A, (addr / 4) * 4 + 3, picc_passward, pSnr) != MI_OK)//��֤����
	{
		printf("�ȶ�����ʧ��\r\n");
		return MI_ERR;
	}
	//5.д����
	if (PcdWrite(addr, data) != MI_OK)//д����
	{
		printf("д����ʧ��\r\n");
		return MI_ERR;
	}
	PcdHalt();//����
	printf("д��ɹ�\r\n");

	return MI_OK;

}

/******************************************************
*�� �� ����ReadCardData
*�������ܣ��ӿ�Ƭ��ĳ���ַ������
*��    ����u8 addr,u8 *data,u8 *pSnr   ���Ŀ��   ���������ݴ浽����   ��ID
*�� �� ֵ��0�ɹ� ����ʧ��
*��    ע��
*******************************************************/
u8 ReadCardData(u8 addr, u8* data, u8* pSnr)
{
	uint8_t pTagType[2] = { 0 }; //�濨����������
	//1.Ѱ��
	if (PcdRequest(PICC_REQIDL, pTagType) != MI_OK)
	{
		return 1;
	}
	//2.����ͻ
	if (PcdAnticoll(pSnr) != MI_OK)
	{
		return 2;
	}
	//3.ѡ��
	if (PcdSelect(pSnr) != MI_OK)
	{
		return 3;
	}
	Voice_SendCmd(0x2e);//��������"��"
	//4.������֤
	if (PcdAuthState(PICC_AUTHENT1A, (addr / 4) * 4 + 3, picc_passward, pSnr) != MI_OK)
	{
		printf("�ȶ�����ʧ��\r\n");
		PcdHalt();//����
		return 4;
	}
	//5.д����
	if (PcdRead(addr, data) != MI_OK)
	{
		printf("������ʧ��\r\n");
		PcdHalt();//����
		return 5;
	}
	PcdHalt();//����
	printf("��ȡ���ݣ�\r\n");
	for (u8 i = 0; i < 16; i++)
	{
		printf("0x%02x  ", *(data + i));
	}
	printf("\r\n");
	printf("�ַ�����ʽ��%s\r\n", data);

	return MI_OK;
}



/***********************************************************************************/
//��RC532�Ĵ���
//Address:�Ĵ�����ַ
u8 ReadRawRC(u8 Address)
{
	u8 ucAddr;
	u8 ucResult = 0;
	RFID_CS_L;
	/*RC522�ֲ��д��ַ����Ϊ����λMSBΪ1��Ϊ��,MSBΪ0��Ϊд,����ַ�Ǵӵ�1λ��ʼ��,��0λ��λ0,��д��ַucAddr = ((Address<<1)&0x7E);����ַucAddr = ((Address<<1)&0x7E)|0x80;*/
	ucAddr = ((Address << 1) & 0x7E) | 0x80;

	SPI3_Transfer(ucAddr);      //���Ͳ���
	ucResult = SPI3_Transfer(0);  //����
	RFID_CS_H;
	return ucResult;
}

//дRC632�Ĵ���
//Address[IN]:�Ĵ�����ַ
//value[IN]:д���ֵ
void WriteRawRC(uint8_t Address, uint8_t value)
{
	uint8_t ucAddr;

	RFID_CS_L;

	ucAddr = ((Address << 1) & 0x7E);//0111 1110

	SPI3_Transfer(ucAddr);
	SPI3_Transfer(value);
	RFID_CS_H;
}

//��Ĵ���λ
void ClearBitMask(u8 reg, u8 mask)// 0000 1000
{
	char tmp = 0x0;
	tmp = ReadRawRC(reg);
	WriteRawRC(reg, tmp & ~mask);  // clear bit mask
}

//��RC522�Ĵ���λ
//reg[IN]:�Ĵ�����ַ
//mask[IN]:��λֵ
void SetBitMask(u8 reg, u8 mask)
{
	char   tmp = 0x0;
	tmp = ReadRawRC(reg);
	WriteRawRC(reg, tmp | mask);  // set bit mask
}

/***************************************************
��������PcdReset
��  �ܣ���λ����
��  ������
����ֵ��char
��  ע��char   MI_OK  ----0   ��ʾͨ������
***************************************************/
char PcdReset(void)
{
	//���Ÿ�λ
	RFID_RST_H;
	delay_ms(2);

	RFID_RST_L;
	delay_ms(2);

	RFID_RST_H;
	delay_ms(2);

	//�����λ
	WriteRawRC(CommandReg, PCD_RESETPHASE);
	WriteRawRC(CommandReg, PCD_RESETPHASE);
	delay_ms(2);

	WriteRawRC(ModeReg, 0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
	WriteRawRC(TReloadRegL, 30);
	WriteRawRC(TReloadRegH, 0);
	WriteRawRC(TModeReg, 0x8D);
	WriteRawRC(TPrescalerReg, 0x3E);
	WriteRawRC(TxAutoReg, 0x40);          //����Ҫ  

	return MI_OK;
}

//�ر�����
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

//��������
void PcdAntennaOn(void)
{
	u8   i;
	i = ReadRawRC(TxControlReg);
	if (!(i & 0x03))
	{
		SetBitMask(TxControlReg, 0x03);
	}
}
/***************************************************
��������M500PcdConfigISOType
��  �ܣ�����RC522����ͨ��Э��
��  ����u8 type
����ֵ��char
��  ע��
***************************************************/
char M500PcdConfigISOType(u8 type)  	//ѡ��ͨ��Э��
{
	if (type == 'A')                     //ISO14443_A
	{
		ClearBitMask(Status2Reg, 0x08);
		WriteRawRC(ModeReg, 0x3D);//3F
		WriteRawRC(RxSelReg, 0x86);//84
		WriteRawRC(RFCfgReg, 0x7F);   //4F
		WriteRawRC(TReloadRegL, 30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC(TReloadRegH, 0);
		WriteRawRC(TModeReg, 0x8D);
		WriteRawRC(TPrescalerReg, 0x3E);
		delay_us(2);
		PcdAntennaOn();
	}
	else { return 1; }

	return MI_OK;
}

/***************************************************
��������PcdComMF522
��  �ܣ�ͨ��RC522��ISO14443��ͨѶ
��  ����Command[IN]:	RC522������
		pIn [IN]:		ͨ��RC522���͵���Ƭ������
		InLenByte[IN]:	�������ݵ��ֽڳ���
		pOut [OUT]:���յ��Ŀ�Ƭ��������
		*pOutLenBit[OUT]:�������ݵ�λ����
����ֵ��char
��  ע��
***************************************************/
char PcdComMF522(u8 Command, u8* pIn, u8 InLenByte, u8* pOut, u8* pOutLenBit)
{
	char   status = MI_ERR;
	u8   irqEn = 0x00;
	u8   waitFor = 0x00;
	u8   lastBits;
	u8   n;
	u16   i;
	switch (Command)
	{
	case PCD_AUTHENT:
		irqEn = 0x12;
		waitFor = 0x10;
		break;
	case PCD_TRANSCEIVE:
		irqEn = 0x77;
		waitFor = 0x30;
		break;
	default:	break;
	}

	WriteRawRC(ComIEnReg, irqEn | 0x80);
	ClearBitMask(ComIrqReg, 0x80);		//�������ж�λ
	WriteRawRC(CommandReg, PCD_IDLE);
	SetBitMask(FIFOLevelReg, 0x80);	 	//��FIFO����

	for (i = 0; i < InLenByte; i++)
	{
		WriteRawRC(FIFODataReg, pIn[i]);
	}
	WriteRawRC(CommandReg, Command);
	//   	 n = ReadRawRC(CommandReg);

	if (Command == PCD_TRANSCEIVE)
	{
		SetBitMask(BitFramingReg, 0x80);
	}	 //��ʼ����

	i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
	// 	i = 100000;
	do
	{
		n = ReadRawRC(ComIrqReg);
		i--;
	} while ((i != 0) && !(n & 0x01) && !(n & waitFor));
	ClearBitMask(BitFramingReg, 0x80);

	if (i != 0)
	{
		if (!(ReadRawRC(ErrorReg) & 0x1B))
		{
			status = MI_OK;
			if (n & irqEn & 0x01)
			{
				status = MI_NOTAGERR;
			}
			if (Command == PCD_TRANSCEIVE)
			{
				n = ReadRawRC(FIFOLevelReg);
				lastBits = ReadRawRC(ControlReg) & 0x07;
				if (lastBits)
				{
					*pOutLenBit = (n - 1) * 8 + lastBits;
				}
				else
				{
					*pOutLenBit = n * 8;
				}
				if (n == 0)
				{
					n = 1;
				}
				if (n > MAXRLEN)
				{
					n = MAXRLEN;
				}
				for (i = 0; i < n; i++)
				{
					pOut[i] = ReadRawRC(FIFODataReg);
				}
			}
		}
		else
		{
			status = MI_ERR;
		}

	}

	SetBitMask(ControlReg, 0x80);           // stop timer now
	WriteRawRC(CommandReg, PCD_IDLE);
	return status;
}

/***************************************************
��������PcdRequest
��  �ܣ�Ѱ������
��  ����u8 req_code,u8 *pTagType
����ֵ��char	�ɹ�����MI_OK
��  ע��u8 req_code
				0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�������������Ҳ���ҵ���
				0x26 = Ѱδ��������״̬�Ŀ�
		u8 *pTagType�����������ǵ�ֵ
				 ��Ƭ������
					 0x4400 = Mifare_UltraLight
					 0x0400 = Mifare_One(S50)
					 0x0200 = Mifare_One(S70)
					 0x0800 = Mifare_Pro(X)
					 0x4403 = Mifare_DESFire

***************************************************/
char PcdRequest(u8 req_code, u8* pTagType)
{
	char status;
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN];

	ClearBitMask(Status2Reg, 0x08);
	WriteRawRC(BitFramingReg, 0x07);
	SetBitMask(TxControlReg, 0x03);

	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{
		*pTagType = ucComMF522Buf[0];
		*(pTagType + 1) = ucComMF522Buf[1];
	}
	else
	{
		status = MI_ERR;
	}
	return status;
}

/***************************************************
������������ײ
��  �ܣ�PcdAnticoll
��  ����u8 *pSnr
����ֵ��char	 �ɹ�����MI_OK
��  ע��pSnr[OUT]:��Ƭ���кţ�4�ֽ�
***************************************************/
char PcdAnticoll(u8* pSnr)
{
	char status;
	u8 i, snr_check = 0;
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN];

	ClearBitMask(Status2Reg, 0x08);
	WriteRawRC(BitFramingReg, 0x00);
	ClearBitMask(CollReg, 0x80);

	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x20;

	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);

	if (status == MI_OK)
	{
		for (i = 0; i < 4; i++)
		{
			*(pSnr + i) = ucComMF522Buf[i];
			snr_check ^= ucComMF522Buf[i];
		}
		if (snr_check != ucComMF522Buf[i])
		{
			status = MI_ERR;
		}
	}

	SetBitMask(CollReg, 0x80);
	return status;
}

/*******************************************************************
* ��������     CalulateCRC()
* �������� ��  ��MF522����CRC16����
* ���ߣ�         л����
*����˵��:
* ����ֵ˵����
* �޸ļ�¼��
* ������
*******************************************************************/
void CalulateCRC(u8* pIn, u8 len, u8* pOut)
{
	u8 i, n;
	ClearBitMask(DivIrqReg, 0x04);
	WriteRawRC(CommandReg, PCD_IDLE);
	SetBitMask(FIFOLevelReg, 0x80);
	for (i = 0; i < len; i++)
	{
		WriteRawRC(FIFODataReg, *(pIn + i));
	}
	WriteRawRC(CommandReg, PCD_CALCCRC);
	i = 0xFF;
	do
	{
		n = ReadRawRC(DivIrqReg);
		i--;
	} while ((i != 0) && !(n & 0x04));
	pOut[0] = ReadRawRC(CRCResultRegL);
	pOut[1] = ReadRawRC(CRCResultRegM);
}

/***************************************************
��������PcdSelect
��  �ܣ�ѡ����Ƭ
��  ����u8 *pSnr
����ֵ��char	�ɹ�����MI_OK
��  ע��pSnr[IN]:��Ƭ���кţ�4�ֽ�
***************************************************/
char PcdSelect(u8* pSnr)
{
	char status;
	u8 i;
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN];

	ucComMF522Buf[0] = PICC_ANTICOLL1;
	ucComMF522Buf[1] = 0x70;
	ucComMF522Buf[6] = 0;
	for (i = 0; i < 4; i++)
	{
		ucComMF522Buf[i + 2] = *(pSnr + i);
		ucComMF522Buf[6] ^= *(pSnr + i);
	}
	CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);

	ClearBitMask(Status2Reg, 0x08);

	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen);

	if ((status == MI_OK) && (unLen == 0x18))
	{
		status = MI_OK;
	}
	else
	{
		status = MI_ERR;
	}
	return status;
}

/***************************************************
��������PcdAuthState
��  �ܣ���֤��Ƭ����
��  ����auth_mode[IN]: 	������֤ģʽ
				 0x60 = ��֤A��Կ
				 0x61 = ��֤B��Կ
		addr[IN]��		���ַ
		pKey[IN]��		����
		pSnr[IN]��		��Ƭ���кţ�4�ֽ�
����ֵ��char	�ɹ�����MI_OK
��  ע��
***************************************************/
char PcdAuthState(u8 auth_mode, u8 addr, u8* pKey, u8* pSnr)
{
	char status;
	u8 unLen;
	u8 ucComMF522Buf[MAXRLEN];

	ucComMF522Buf[0] = auth_mode;
	ucComMF522Buf[1] = addr;
	memcpy(&ucComMF522Buf[2], pKey, 6);
	memcpy(&ucComMF522Buf[8], pSnr, 4);

	status = PcdComMF522(PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, &unLen);
	if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
	{
		status = MI_ERR;
	}
	return status;
}

/***************************************************
��������PcdWrite
��  �ܣ�д���ݵ�M1��һ��
��  ����addr[IN]��	���ַ
		p[OUT]��	���������ݣ�16�ֽ�
����ֵ��char	�ɹ�����MI_OK
��  ע��
***************************************************/
char PcdWrite(u8 addr, u8* p)
{
	char status;
	u8 unLen;
	u8 i, ucComMF522Buf[MAXRLEN];

	ucComMF522Buf[0] = PICC_WRITE;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

	if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
	{
		status = MI_ERR;
	}

	if (status == MI_OK)
	{
		//memcpy(ucComMF522Buf, p , 16);
		for (i = 0; i < 16; i++)
		{
			ucComMF522Buf[i] = *(p + i);
		}
		CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);

		status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, &unLen);
		if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
		{
			status = MI_ERR;
		}
	}
	return status;
}

/***************************************************
��������PcdRead
��  �ܣ���ȡM1��һ������
��  ����addr[IN]�����ַ
		p [OUT]�����������ݣ�16�ֽ�
����ֵ��char	�ɹ�����MI_OK
��  ע��
***************************************************/
char PcdRead(u8 addr, u8* p)
{
	char status;
	u8 unLen;
	u8 i, ucComMF522Buf[MAXRLEN];

	ucComMF522Buf[0] = PICC_READ;
	ucComMF522Buf[1] = addr;
	CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
	if ((status == MI_OK) && (unLen == 0x90))
		//   {   memcpy(p , ucComMF522Buf, 16);   }
	{
		for (i = 0; i < 16; i++)
		{
			*(p + i) = ucComMF522Buf[i];
		}
	}
	else
	{
		status = MI_ERR;
	}

	return status;
}

//��˯�ߺ���
char PcdHalt(void)
{
	char status;
	unsigned char unLen;
	unsigned char ucComMF522Buf[MAXRLEN];
	ucComMF522Buf[0] = PICC_HALT;
	ucComMF522Buf[1] = 0;
	CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

	status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
	return MI_OK;
}
/*************************************************************************************************/







