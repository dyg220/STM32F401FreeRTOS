#include "mg200.h"


/******************************************************
*函 数 名：USART6_Config
*函数功能：USART6配置
*参    数：Baud
*返 回 值：None
*备    注：
*******************************************************/
void USART6_Config(u32 Baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
	
	USART_InitTypeDef USART_InitStruct = {0};
	USART_InitStruct.USART_BaudRate = Baud;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART6,&USART_InitStruct);
	
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);//使能接受中断
	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);//使能空闲中断
	
	NVIC_InitTypeDef NVIC_InitStruct={0};
	NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;  		//通道号
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;    		//是否使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;   //优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;    		//次级优先级（响应）
	
	NVIC_Init(&NVIC_InitStruct);  //结构体变量的地址
	
	USART_Cmd(USART6,ENABLE);
}

void USART6_SendByte(u8 Byte)
{
	while(USART_GetFlagStatus(USART6,USART_FLAG_TXE) == 0);
	USART_SendData(USART6,Byte);
}

u8 USART6_RecvByte(void)
{
	while(USART_GetFlagStatus(USART6,USART_FLAG_RXNE) == 0);
	return USART_ReceiveData(USART6);
}

USART_INFO MG200_Recv = {0};

//中断服务函数
void USART6_IRQHandler(void)
{
	u8 clear = 0;
	//接受中断
	if(USART_GetITStatus(USART6,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);//清除中断标志位
		if(MG200_Recv.index<Max_Size-1)//防止溢出
		{
			MG200_Recv.data[MG200_Recv.index]=USART_ReceiveData(USART6);
			MG200_Recv.index++;
		}
	}
	//空闲中断
	if(USART_GetITStatus(USART6,USART_IT_IDLE)==SET)
	{
		//清除中断标志位
		clear=USART6->SR;
		clear=USART6->DR;
		(void)clear;
		MG200_Recv.flag = 1;
		MG200_Recv.index = 0;
	}
}
/******************************************************
*函 数 名：MG200_Init
*函数功能：MG200指纹初始化
*参    数：Baud
*返 回 值：None
*备    注：PWR---PC1      DETECT---PC0
*******************************************************/
void MG200_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	//电源引脚PC1
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//触摸引脚PC0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	USART6_Config(115200);
	
	MG200_PWE_ON;//关闭低功耗
}


/******************************************************
*函 数 名：MG200_Send_Cmd
*函数功能：MG200发送数据包
*参    数：cmd 指令码     parameter 参数
*返 回 值：None
*备    注：
*******************************************************/
void MG200_Send_Cmd(u8 cmd,u8 parameter)
{
	u8 check_sum = 0;//校验和变量
	USART6_SendByte(0x6c);//起始码
	USART6_SendByte(0x63);//发送地址
	USART6_SendByte(0x62);//接收地址
	USART6_SendByte(cmd);//指令码
	USART6_SendByte(parameter);//参数
	USART6_SendByte(0x00);//预留
	check_sum = (0x63+0x62+cmd+parameter)&0xff;
	USART6_SendByte(check_sum);
	//防止上电 会发送数据给MCU 意外导致标志位提前置1
	MG200_Recv.flag = 0;
}


/******************************************************
*函 数 名：MG200_Read_Cmd
*函数功能：MG200解析数据包
*参    数：cmd        之前发送的指令码    
		   *parameter 接收到的命令参数   
		   *result    命令执行结果
*返 回 值：0成功    其他错误
*备    注：
*******************************************************/
u8 MG200_Read_Cmd(u8 cmd,u8 *result,u8 *parameter)
{
	u8 check_sum = 0;
	while(!MG200_Recv.flag);//等待接收完成
	MG200_Recv.flag = 0;
	
	if(MG200_Recv.data[0] != 0x6c)	return 1;
	if(MG200_Recv.data[1] != 0x62)	return 2;
	if(MG200_Recv.data[2] != 0x63)	return 3;
	if(MG200_Recv.data[3] != cmd)	return 4;
	
	*result = MG200_Recv.data[4];//获取到执行效果数据
	*parameter = MG200_Recv.data[5];//获取到返回的参数
	//if(MG200_Recv.data[6] != 0x00)	return 5;//预留位，不一定是0x00
	
	check_sum = (0x62 + 0x63 + cmd + *result + *parameter + MG200_Recv.data[6]) & 0xff;
	if(MG200_Recv.data[7] != check_sum)	return 6;
	return 0;
}

/******************************************************
*函 数 名：CaptureAndExtract
*函数功能：指纹采集
*参    数：parameter 第几次采集   
*返 回 值：0成功   0xff通信失败  其他错误
*备    注：
*******************************************************/
u8 CaptureAndExtract(u8 parameter)
{
	u8 Result, ACKparameter;
    MG200_Send_Cmd(0x51, parameter);	// 发送提取特征量的数据包
	if(MG200_Read_Cmd(0x51, &Result, &ACKparameter)!= 0)	// 接收返回的数据包错误
	{
		printf("通信失败\r\n");
	    return 0xff;
	}
	switch (Result)
	{
		case 0x00: printf("抓取指纹图像及特征量提取成功！\r\n"); break;
		case 0xb1: printf("指纹过小(small finger)！\r\n"); break;
		case 0xb2: printf("无指纹(no finger)！\r\n"); break;
		case 0xb3: printf("指纹位置过于靠左, 需要将手指靠右边摁按(left finger)！\r\n"); break;
		case 0xb4: printf("指纹位置过于靠右, 需要将手指靠左边摁按(right finger)！\r\n"); break;
		case 0xb5: printf("指纹位置过于靠上, 需要将手指靠下边摁按(up finger)！\r\n"); break;
		case 0xb6: printf("指纹位置过于靠下, 需要将手指靠上边摁按(down finger)！\r\n"); break;
		case 0xb7: printf("手指过湿(指纹表面出汗或者水渍过多)(wet finger)！\r\n"); break;
		case 0xb8: printf("干燥手指(dry finger)！\r\n"); break;
		case 0xc0: printf("特征提取错误！\r\n"); break;
		case 0xc2: printf("特征提取错误(图像质量差)！\r\n"); break;
		default:   printf("抓取指纹图像及特征量提取错误！\r\n"); break;
	}
	return Result;
}



/******************************************************
*函 数 名：CaptureAndExtract
*函数功能：指纹注册
*参    数：ID 要注册进的ID号
*返 回 值：0成功   0xff通信失败    其他错误
*备    注：
*******************************************************/
u8 Enroll(u8 ID)
{
	u8 Result = 0, ACKparameter = 0;
	printf("开始注册指纹！\r\n");
	/*三次采集阶段*/
go0:	
	printf("第一次采集，请将手指按下\r\n");
	Voice_SendCmd(0x10);
	while(MG200_DETECT == 0);			//等待按下
	delay_ms(500);
	if(CaptureAndExtract(0x00)!= 0x00)	//提取特征量失败
	{
		printf("第一次采集失败，请重新按手指！\r\n"); printf("\r\n");
		while(MG200_DETECT == 1);		//等待松开
		goto go0;
	}
	printf("第一次采集成功，请松开手指\r\n"); printf("\r\n");
	while(MG200_DETECT == 1);			//等待松开
	
go1:	
	printf("第二次采集，请将手指按下\r\n");
	Voice_SendCmd(0x10);
	while(MG200_DETECT == 0);			//等待按下
	delay_ms(500);
	if(CaptureAndExtract(0x01)!= 0x00)	//提取特征量失败
	{
		printf("第二次采集失败，请重新按手指！\r\n"); printf("\r\n");
		while(MG200_DETECT == 1);		//等待松开
		goto go1;
	}
	printf("第二次采集成功，请松开手指\r\n"); printf("\r\n");
	while(MG200_DETECT == 1);			//等待松开
	
go2:
	printf("第三次采集，请将手指按下\r\n");
	Voice_SendCmd(0x10);
	while(MG200_DETECT == 0);			//等待按下
	delay_ms(500);
	if(CaptureAndExtract(0x02)!= 0x00)	//提取特征量失败
	{
		printf("第三次采集失败，请重新按手指！\r\n"); printf("\r\n");
		while(MG200_DETECT == 1);		//等待松开
		goto go2;
	}
	printf("第三次采集成功，请松开手指\r\n"); printf("\r\n");
	while(MG200_DETECT == 1);			//等待松开
	
	/*注册阶段*/
	delay_ms(200);
	//注册的ID号（ID范围1~100，最大用户数为100，当该参数为00h时，模块注册的ID号是内部自动分配的）
	MG200_Send_Cmd(0x7f,ID);
	
	if(MG200_Read_Cmd(0x7f, &Result, &ACKparameter) == 0xff)	//接收返回的数据包错误
	{
		printf("通信失败\r\n");
		return 0xff;
	}
	switch(Result)
	{
		case 0x00: printf("注册指纹成功！\r\n");Voice_SendCmd(0x1c); break;
		case 0x83: printf("ID 错误(ID < 0 或 ID > 最大用户数)或通信错误\r\n"); break;
		case 0x91: printf("注册失败(用户区域已满)\r\n"); break;
		case 0x93: printf("已经注册的 ID\r\n"); break;
		case 0x94: printf("指纹提取次数 < 3\r\n"); break;
	}
	return Result;
}


/******************************************************
*函 数 名：Match1N
*函数功能：指纹对比
*参    数：ID 对比出的ID号
*返 回 值：0对比成功   0xff通信失败    其他错误
*备    注：
*******************************************************/
u8 Match1N(u8 *ID)
{
	u8 Result = 0xfe;
	/*采集*/
//	printf("请放下手指\r\n");
	if(MG200_DETECT)
	{
		CaptureAndExtract(0x00); //采集
	
		/*对比*/
		MG200_Send_Cmd(0x71,0x00);//发送对比指令
		if(MG200_Read_Cmd(0x71,&Result,ID)!=0)//解析返回的数据包
		{
			printf("通信失败\r\n");
			return 0xff;
		}
		if(Result == 0x00)
		{
			printf("对比成功\r\n");
			Voice_SendCmd(0x12);//播报"欢迎回家"
			Door_Start(2000);
		}
		else
		{
			Voice_SendCmd(0x13);//播报"开门失败"
			printf("对比失败\r\n");
		}
	}
	return Result;
}


/******************************************************
*函 数 名：EraseOne
*函数功能：删除某一个指纹
*参    数：ID 删除的指纹ID
*返 回 值：0 删除成功   0xff通信失败    其他错误
*备    注：
*******************************************************/
u8 EraseOne(u8 ID)
{
	u8 Result = 0,parameter = 0;
	
	MG200_Send_Cmd(0x73,ID);//发送删除指令
	
	if(MG200_Read_Cmd(0x73,&Result,&parameter)!=0)//解析返回的数据包
	{
		printf("通信失败\r\n");
		return 0xff;
	}
	switch (Result)
	{
		case 0X00: printf("删除成功！\r\n"); Voice_SendCmd(0x1c);break;
		case 0X83: printf("参数错误(ID ≤ 0 或者 ID > 最大用户数)\r\n"); break;
		case 0X90: printf("未注册的用户\r\n"); break;
		case 0XFF: printf("写入 ROM 错误\r\n"); break;
	}
	return Result;
}


/******************************************************
*函 数 名：EraseAll
*函数功能：删除某一个指纹
*参    数：None
*返 回 值：0 删除成功   0xff通信失败    其他错误
*备    注：
*******************************************************/
u8 EraseAll(void)
{
	u8 Result = 0,parameter = 0;
	MG200_Send_Cmd(0x54,0x00);//发送删除指令
	if(MG200_Read_Cmd(0x54,&Result,&parameter)!=0)//解析返回的数据包
	{
		printf("通信失败\r\n");
		return 0xff;
	}
	switch(Result)
	{
		case 0X00: printf("删除全部用户信息成功\r\n"); Voice_SendCmd(0x1c);break;
		case 0X90: printf("删除失败(注册的用户数为 0 的时候)\r\n"); break;
		default:   printf("删除失败\r\n"); break;
	}
	return Result;
}



