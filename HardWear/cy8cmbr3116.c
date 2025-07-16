#include "cy8cmbr3116.h"


//寄存器配置数组
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
*函 数 名：CY8CMBR3116_Init
*函数功能：CY8CMBR3116初始化
*参    数：None
*返 回 值：None
*备    注：
*******************************************************/
void CY8CMBR3116_Init(void)
{
	//IIC初始化配置
	IIC1_Init();
	//复位引脚配置
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//硬件复位
	CY8CMBR3116_RST_H;
	delay_xms(1);
	CY8CMBR3116_RST_L;
	delay_xms(1);
	CY8CMBR3116_RST_H;
	delay_xms(15);

	//寄存器初始化配置
	CY8CMBR3116_regcfg(1);
}


/******************************************************
*函 数 名：CY8CMBR3116_Start
*函数功能：CY8CMBR3116起始条件
*参    数：u8 flag   读写位  读：1   写：0
*返 回 值：None
*备    注：
*******************************************************/
u8 CY8CMBR3116_Start(u8 flag)
{
	u8 ack = 0;
	do {
		IIC1_Start();
		IIC1_SendByte(CY8CMBR3116_ADDR + flag);//发送器件地址
		ack = IIC1_RecvAck();
	} while (ack);
	return 0;
}


/******************************************************
*函 数 CY8CMBR3116_WriteData
*函数功能：CY8CMBR3116写数据
*参    数：u8 addr,u8 *WBuf,u8 len    从往哪写   写什么  写多少
*返 回 值：0成功
*备    注：
*******************************************************/
u8 CY8CMBR3116_WriteData(u8 addr, u8* WBuf, u8 len)
{
	u8 ack = 0;
	ack = CY8CMBR3116_Start(0);//发送器件地址写
	if (ack) { IIC1_Stop();	return 1; }

	IIC1_SendByte(addr);//发送寄存器地址
	delay_us(500);
	ack = IIC1_RecvAck();
	if (ack) { IIC1_Stop();	return 2; }
	delay_us(500);

	//连续写
	while (len--)
	{
		IIC1_SendByte(*WBuf);//写数据
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
*函 数 名：CY8CMBR3116_Start
*函数功能：CY8CMBR3116读数据
*参    数：u8 addr,u8 *buf,u8 len    从哪读   存放数据的数组  读多少
*返 回 值：0成功
*备    注：
*******************************************************/
u8 CY8CMBR3116_ReadData(u8 addr, u8* RBuf, u8 len)
{
	u8 ack = 0;
	ack = CY8CMBR3116_Start(0);//发送器件地址写
	if (ack) { IIC1_Stop();	return 1; }

	IIC1_SendByte(addr);//发送寄存器地址（设置数据指针）
	ack = IIC1_RecvAck();
	if (ack) { IIC1_Stop();	return 2; }
	IIC1_Stop();

	ack = CY8CMBR3116_Start(1);//发送器件地址读
	if (ack) { IIC1_Stop();	return 3; }

	while (--len)
	{
		*RBuf = IIC1_RecvByte();//读取数据
		IIC1_SendAck(ACK);
		RBuf++;
	}
	*RBuf = IIC1_RecvByte();//读取数据
	IIC1_SendAck(NO_ACK);
	IIC1_Stop();
	return 0;
}

/******************************************************
*函 数 名：CY8CMBR3116_WrCmd
*函数功能：命令寄存器操作
*参    数：u8 cmd
*返 回 值：u8    返回IIC的校验值
*备    注：
*******************************************************/
u8 CY8CMBR3116_WrCmd(u8 cmd)
{
	u8 ack = 0;
	ack = CY8CMBR3116_WriteData(CTRL_CMD, &cmd, 1);
	return ack;
}

/******************************************************
*函 数 名：CY8CMBR3116_RdCmdErr
*函数功能：读指令执行的错误状态
*参    数：None
*返 回 值：u8
*备    注：0: 指令执行成功
		   0xfd：对闪存写入失败
		   0xfe：CONFIG_CRC 所存储的配置 CRC 校验和与计算得到的配置 CRC 校验和不一致
		   0xff：指令无效
*******************************************************/
u8 CY8CMBR3116_RdCmdErr(void)
{
	u8 err_val = 0;
	CY8CMBR3116_WriteData(CTRL_CMD_ERR, &err_val, 1);
	return err_val;
}



/******************************************************
*函 数 名：CY8CMBR3116_RdCfgStatus
*函数功能：读取配置状态
*参    数：None
*返 回 值：返回值是0不是出厂配置     返回值是1是出厂配置
*备    注：
*******************************************************/
u8 CY8CMBR3116_RdCfgStatus(void)
{
	u8 sta = 0;
	CY8CMBR3116_ReadData(SYSTEM_STATUS, &sta, 1);
	return sta;
}

/******************************************************
*函 数 名：CY8CMBR3116_regcfg
*函数功能：配置CY8CMBR3116寄存器
*参    数：u8 dfg_flag     1:强制配置    0：根据情况决定是否配置
*返 回 值：返回值：0 配置成功    返回值：1 使用原配置
										 2：寄存器配置值推送失败
										 3：保存闪存失败
										 4：总体配置失败
*备    注： 配置寄存器
			发送保存指令
			保存配置信息需要300ms
			复位生效
*******************************************************/
u8 CY8CMBR3116_regcfg(u8 dfg_flag)
{
	u8 reg_sta = 0, ack = 0, reg_init_cont = 0;
ReInit:
	//读出厂配置状态寄存器
	reg_sta = CY8CMBR3116_RdCfgStatus();

	if (reg_sta == 1)
		printf("此芯片是出厂配置\r\n");
	else
		printf("此芯片不是出厂配置\r\n");

	//不需要配置
	if (dfg_flag == 0 && reg_sta == 0)
	{
		return 1;     //不配置
	}
	//配置
	for (u8 i = 0; i < 10; i++)
	{
		ack = CY8CMBR3116_WriteData(REGCFG_ADDR, (u8*)CY8CMBR3116_RegPara, 128);
		if (ack == 0) break;//成功就退出
	}
	if (ack == 0)
	{
		printf("寄存器配置数据推送成功\r\n");
	}
	else
	{
		printf("寄存器配置数据推送失败\r\n");
		return 2;  //配置数据失败
	}

	//保存到非易失器件
	for (u8 i = 0; i < 10; i++)
	{
		ack = CY8CMBR3116_WrCmd(2);
		if (ack == 0) break;//成功就退出
	}
	if (ack == 1) return 3;
	delay_ms(300);

	//读命令错误状态
	u8 err = 0;
	err = CY8CMBR3116_RdCmdErr();
	switch (err)
	{
	case 0:printf("指令执行成功\r\n"); break;
	case 0xfd:printf("闪存写入失败\r\n"); break;
	case 0xfe:printf("CRC校验失败\r\n"); break;
	case 0xff:printf("指令无效\r\n"); break;
	}
	reg_init_cont++;
	if (err != 0)
	{
		if (reg_init_cont < 10)  goto ReInit;
		printf("配置失败\r\n");
		return 4;
	}
	//软件复位
	ack = CY8CMBR3116_WrCmd(0xff);
	delay_ms(15);

	printf("总体配置成功\r\n");
	return 0;
}


/******************************************************
*函 数 名：CY8CMBR3116_Read_Key
*函数功能：读取按键状态寄存器的值并判断为某个按键
*参    数：None
*返 回 值：u8
*备    注：
*******************************************************/
u8 CY8CMBR3116_Read_Key(void)
{
	u8 ack = 0;
	u8 key_val[2] = { 0 };
	u16 key_temp = 0;
	ack = CY8CMBR3116_ReadData(BUTTON_STA, key_val, 2);
	if (ack)printf("键值读取失败\r\n");

	key_temp = (key_val[1] << 8) | key_val[0];//合成寄存器原始数据（因为寄存器是低字节优先）
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
*函 数 名：CY8CMBR3116_Key_Scan
*函数功能：按键扫描函数
*参    数：None
*返 回 值：u8
*备    注：
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
		Voice_SendCmd(0x2e);//语音播报"嘀"
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




