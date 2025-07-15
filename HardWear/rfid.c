#include "rfid.h"


/******************************************************
*函 数 名：RFID_Init
*函数功能：RFID初始化
*参    数：None
*返 回 值：None
*备    注：
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

	SPI3_Init();               //SPI初始化函数
	PcdReset();                //软硬复位
	PcdAntennaOff();		   //关闭天线
	PcdAntennaOn();		       //开启天线	
	M500PcdConfigISOType('A'); //A协议配置
}

/******************************************************
*函 数 名：PCD_distinguish_PICC
*函数功能：PCD识别typeA类卡
*参    数：u8 *pSnr      识别到的4字节的ID号
*返 回 值：返回值阶段错误
*备    注：	pTagType[OUT]:卡片类型代码(做输出参数)
			0x4400=  Mifare_UltraLight
			0x0400 = Mifare One (S50)
			0x0200 = Mifare One (S70)
			0x0800 = Mifare Pro(X)
			0x4403 = Mifare_DESFire
*******************************************************/
u8 PCD_distinguish_PICC(u8* pSnr)
{
	uint8_t pTagType[2] = { 0 }; //存卡的类型数组
	//1.寻卡
	if (PcdRequest(PICC_REQIDL, pTagType) != MI_OK)
	{
		return 1;
	}
	//2.防冲突
	if (PcdAnticoll(pSnr) != MI_OK)
	{
		printf("防冲突失败\r\n");
		return 2;
	}
	//3.选卡
	if (PcdSelect(pSnr) != MI_OK)
	{
		printf("选卡失败\r\n");
		return 3;
	}
	Voice_SendCmd(0x2e);//语音播报"嘀"
	printf("卡类型:0x%02x%02x\r\n", pTagType[0], pTagType[1]);
	printf("卡号:%#x %#x %#x %#x\r\n", pSnr[0], pSnr[1], pSnr[2], pSnr[3]);

	//睡眠
	PcdHalt();
	return MI_OK;
}

u8 picc_passward[6] = { 0xff,0xff,0xff,0xff,0xff,0xff };//卡密码-初始密码--空白卡

///******************************************************
//*函 数 名：WriteCardData
//*函数功能：往卡片的某块地址写数据
//*参    数：u8 addr,u8 *data,u8 *pSnr
//*返 回 值：0成功 其他失败
//*备    注：块的绝对地址0~63 注意不写到控制块中
//           要写的数据为16字节
//*******************************************************/
//u8 WriteCardData(u8 addr,u8 *data,u8 *pSnr)
//{
//	uint8_t pTagType[2] = {0}; //存卡的类型数组
//	//1.寻卡
//	if(PcdRequest(PICC_REQIDL,pTagType)!=MI_OK)
//	{
//		 return 1;
//	}
//	//2.防冲突
//	if(PcdAnticoll(pSnr)!=MI_OK)
//	{
//		return 2;
//	}
//	//3.选卡
//	if(PcdSelect(pSnr)!=MI_OK)
//	{
//		return 3;
//	}
//	Voice_SendCmd(0x2e);//语音播报"嘀"
//	//4.密码验证
//	if(PcdAuthState(PICC_AUTHENT1A,(addr/4)*4+3,picc_passward,pSnr)!=MI_OK)
//	{
//		printf("比对密码失败\r\n");
//		PcdHalt();//休眠
//		return 4;
//	}
//	//5.写数据
//	if(PcdWrite(addr,data)!=MI_OK)
//	{
//		printf("写数据失败\r\n");
//		PcdHalt();//休眠
//		return 5;
//	}
//	PcdHalt();//休眠
//	printf("数据写入成功\r\n");
//	
//	return MI_OK;
//}


/******************************************************
*函 数 名：WriteCardData
*函数功能：往卡片的某块地址写数据
*参    数：u8 addr,u8 *data,u8 *pSnr
*返 回 值：0成功 其他失败
*备    注：块的绝对地址0~63 注意不写到控制块中
		   要写的数据为16字节
*******************************************************/
u8 WriteCardData(u8 addr, u8* data, u8* pSnr)
{
	//1.选卡
	if (PCD_distinguish_PICC(pSnr) != MI_OK)//选卡
	{
		return MI_ERR;
	}
	//4.密码验证
	if (PcdAuthState(PICC_AUTHENT1A, (addr / 4) * 4 + 3, picc_passward, pSnr) != MI_OK)//验证密码
	{
		printf("比对密码失败\r\n");
		return MI_ERR;
	}
	//5.写数据
	if (PcdWrite(addr, data) != MI_OK)//写数据
	{
		printf("写数据失败\r\n");
		return MI_ERR;
	}
	PcdHalt();//休眠
	printf("写入成功\r\n");

	return MI_OK;

}

/******************************************************
*函 数 名：ReadCardData
*函数功能：从卡片的某块地址读数据
*参    数：u8 addr,u8 *data,u8 *pSnr   从哪块读   读到的数据存到哪里   卡ID
*返 回 值：0成功 其他失败
*备    注：
*******************************************************/
u8 ReadCardData(u8 addr, u8* data, u8* pSnr)
{
	uint8_t pTagType[2] = { 0 }; //存卡的类型数组
	//1.寻卡
	if (PcdRequest(PICC_REQIDL, pTagType) != MI_OK)
	{
		return 1;
	}
	//2.防冲突
	if (PcdAnticoll(pSnr) != MI_OK)
	{
		return 2;
	}
	//3.选卡
	if (PcdSelect(pSnr) != MI_OK)
	{
		return 3;
	}
	Voice_SendCmd(0x2e);//语音播报"嘀"
	//4.密码验证
	if (PcdAuthState(PICC_AUTHENT1A, (addr / 4) * 4 + 3, picc_passward, pSnr) != MI_OK)
	{
		printf("比对密码失败\r\n");
		PcdHalt();//休眠
		return 4;
	}
	//5.写数据
	if (PcdRead(addr, data) != MI_OK)
	{
		printf("读数据失败\r\n");
		PcdHalt();//休眠
		return 5;
	}
	PcdHalt();//休眠
	printf("读取数据：\r\n");
	for (u8 i = 0; i < 16; i++)
	{
		printf("0x%02x  ", *(data + i));
	}
	printf("\r\n");
	printf("字符串形式：%s\r\n", data);

	return MI_OK;
}



/***********************************************************************************/
//读RC532寄存器
//Address:寄存器地址
u8 ReadRawRC(u8 Address)
{
	u8 ucAddr;
	u8 ucResult = 0;
	RFID_CS_L;
	/*RC522手册读写地址规则为第七位MSB为1则为读,MSB为0则为写,即地址是从第1位开始的,第0位需位0,则写地址ucAddr = ((Address<<1)&0x7E);读地址ucAddr = ((Address<<1)&0x7E)|0x80;*/
	ucAddr = ((Address << 1) & 0x7E) | 0x80;

	SPI3_Transfer(ucAddr);      //发送操作
	ucResult = SPI3_Transfer(0);  //接收
	RFID_CS_H;
	return ucResult;
}

//写RC632寄存器
//Address[IN]:寄存器地址
//value[IN]:写入的值
void WriteRawRC(uint8_t Address, uint8_t value)
{
	uint8_t ucAddr;

	RFID_CS_L;

	ucAddr = ((Address << 1) & 0x7E);//0111 1110

	SPI3_Transfer(ucAddr);
	SPI3_Transfer(value);
	RFID_CS_H;
}

//清寄存器位
void ClearBitMask(u8 reg, u8 mask)// 0000 1000
{
	char tmp = 0x0;
	tmp = ReadRawRC(reg);
	WriteRawRC(reg, tmp & ~mask);  // clear bit mask
}

//置RC522寄存器位
//reg[IN]:寄存器地址
//mask[IN]:置位值
void SetBitMask(u8 reg, u8 mask)
{
	char   tmp = 0x0;
	tmp = ReadRawRC(reg);
	WriteRawRC(reg, tmp | mask);  // set bit mask
}

/***************************************************
函数名：PcdReset
功  能：复位函数
参  数：无
返回值：char
备  注：char   MI_OK  ----0   表示通信正常
***************************************************/
char PcdReset(void)
{
	//引脚复位
	RFID_RST_H;
	delay_ms(2);

	RFID_RST_L;
	delay_ms(2);

	RFID_RST_H;
	delay_ms(2);

	//软件复位
	WriteRawRC(CommandReg, PCD_RESETPHASE);
	WriteRawRC(CommandReg, PCD_RESETPHASE);
	delay_ms(2);

	WriteRawRC(ModeReg, 0x3D);            //和Mifare卡通讯，CRC初始值0x6363
	WriteRawRC(TReloadRegL, 30);
	WriteRawRC(TReloadRegH, 0);
	WriteRawRC(TModeReg, 0x8D);
	WriteRawRC(TPrescalerReg, 0x3E);
	WriteRawRC(TxAutoReg, 0x40);          //必须要  

	return MI_OK;
}

//关闭天线
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

//开启天线
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
函数名：M500PcdConfigISOType
功  能：设置RC522国际通信协议
参  数：u8 type
返回值：char
备  注：
***************************************************/
char M500PcdConfigISOType(u8 type)  	//选择通信协议
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
函数名：PcdComMF522
功  能：通过RC522和ISO14443卡通讯
参  数：Command[IN]:	RC522命令字
		pIn [IN]:		通过RC522发送到卡片的数据
		InLenByte[IN]:	发送数据的字节长度
		pOut [OUT]:接收到的卡片返回数据
		*pOutLenBit[OUT]:返回数据的位长度
返回值：char
备  注：
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
	ClearBitMask(ComIrqReg, 0x80);		//清所有中断位
	WriteRawRC(CommandReg, PCD_IDLE);
	SetBitMask(FIFOLevelReg, 0x80);	 	//清FIFO缓存

	for (i = 0; i < InLenByte; i++)
	{
		WriteRawRC(FIFODataReg, pIn[i]);
	}
	WriteRawRC(CommandReg, Command);
	//   	 n = ReadRawRC(CommandReg);

	if (Command == PCD_TRANSCEIVE)
	{
		SetBitMask(BitFramingReg, 0x80);
	}	 //开始传送

	i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
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
函数名：PcdRequest
功  能：寻卡函数
参  数：u8 req_code,u8 *pTagType
返回值：char	成功返回MI_OK
备  注：u8 req_code
				0x52 = 寻感应区内所有符合14443A标准的卡（进入了休眠也能找到）
				0x26 = 寻未进入休眠状态的卡
		u8 *pTagType：函数给我们的值
				 卡片的类型
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
函数名：防冲撞
功  能：PcdAnticoll
参  数：u8 *pSnr
返回值：char	 成功返回MI_OK
备  注：pSnr[OUT]:卡片序列号，4字节
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
* 函数名：     CalulateCRC()
* 功能描述 ：  用MF522计算CRC16函数
* 作者：         谢国卿
*参数说明:
* 返回值说明：
* 修改记录：
* 其他：
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
函数名：PcdSelect
功  能：选定卡片
参  数：u8 *pSnr
返回值：char	成功返回MI_OK
备  注：pSnr[IN]:卡片序列号，4字节
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
函数名：PcdAuthState
功  能：验证卡片密码
参  数：auth_mode[IN]: 	密码验证模式
				 0x60 = 验证A密钥
				 0x61 = 验证B密钥
		addr[IN]：		块地址
		pKey[IN]：		密码
		pSnr[IN]：		卡片序列号，4字节
返回值：char	成功返回MI_OK
备  注：
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
函数名：PcdWrite
功  能：写数据到M1卡一块
参  数：addr[IN]：	块地址
		p[OUT]：	读出的数据，16字节
返回值：char	成功返回MI_OK
备  注：
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
函数名：PcdRead
功  能：读取M1卡一块数据
参  数：addr[IN]：块地址
		p [OUT]：读出的数据，16字节
返回值：char	成功返回MI_OK
备  注：
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

//卡睡眠函数
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







