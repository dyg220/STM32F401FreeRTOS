#include "mg200.h"


/******************************************************
*�� �� ����USART6_Config
*�������ܣ�USART6����
*��    ����Baud
*�� �� ֵ��None
*��    ע��
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
	
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);//ʹ�ܽ����ж�
	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);//ʹ�ܿ����ж�
	
	NVIC_InitTypeDef NVIC_InitStruct={0};
	NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;  		//ͨ����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;    		//�Ƿ�ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 1;   //���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;    		//�μ����ȼ�����Ӧ��
	
	NVIC_Init(&NVIC_InitStruct);  //�ṹ������ĵ�ַ
	
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

//�жϷ�����
void USART6_IRQHandler(void)
{
	u8 clear = 0;
	//�����ж�
	if(USART_GetITStatus(USART6,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);//����жϱ�־λ
		if(MG200_Recv.index<Max_Size-1)//��ֹ���
		{
			MG200_Recv.data[MG200_Recv.index]=USART_ReceiveData(USART6);
			MG200_Recv.index++;
		}
	}
	//�����ж�
	if(USART_GetITStatus(USART6,USART_IT_IDLE)==SET)
	{
		//����жϱ�־λ
		clear=USART6->SR;
		clear=USART6->DR;
		(void)clear;
		MG200_Recv.flag = 1;
		MG200_Recv.index = 0;
	}
}
/******************************************************
*�� �� ����MG200_Init
*�������ܣ�MG200ָ�Ƴ�ʼ��
*��    ����Baud
*�� �� ֵ��None
*��    ע��PWR---PC1      DETECT---PC0
*******************************************************/
void MG200_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	//��Դ����PC1
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//��������PC0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	USART6_Config(115200);
	
	MG200_PWE_ON;//�رյ͹���
}


/******************************************************
*�� �� ����MG200_Send_Cmd
*�������ܣ�MG200�������ݰ�
*��    ����cmd ָ����     parameter ����
*�� �� ֵ��None
*��    ע��
*******************************************************/
void MG200_Send_Cmd(u8 cmd,u8 parameter)
{
	u8 check_sum = 0;//У��ͱ���
	USART6_SendByte(0x6c);//��ʼ��
	USART6_SendByte(0x63);//���͵�ַ
	USART6_SendByte(0x62);//���յ�ַ
	USART6_SendByte(cmd);//ָ����
	USART6_SendByte(parameter);//����
	USART6_SendByte(0x00);//Ԥ��
	check_sum = (0x63+0x62+cmd+parameter)&0xff;
	USART6_SendByte(check_sum);
	//��ֹ�ϵ� �ᷢ�����ݸ�MCU ���⵼�±�־λ��ǰ��1
	MG200_Recv.flag = 0;
}


/******************************************************
*�� �� ����MG200_Read_Cmd
*�������ܣ�MG200�������ݰ�
*��    ����cmd        ֮ǰ���͵�ָ����    
		   *parameter ���յ����������   
		   *result    ����ִ�н��
*�� �� ֵ��0�ɹ�    ��������
*��    ע��
*******************************************************/
u8 MG200_Read_Cmd(u8 cmd,u8 *result,u8 *parameter)
{
	u8 check_sum = 0;
	while(!MG200_Recv.flag);//�ȴ��������
	MG200_Recv.flag = 0;
	
	if(MG200_Recv.data[0] != 0x6c)	return 1;
	if(MG200_Recv.data[1] != 0x62)	return 2;
	if(MG200_Recv.data[2] != 0x63)	return 3;
	if(MG200_Recv.data[3] != cmd)	return 4;
	
	*result = MG200_Recv.data[4];//��ȡ��ִ��Ч������
	*parameter = MG200_Recv.data[5];//��ȡ�����صĲ���
	//if(MG200_Recv.data[6] != 0x00)	return 5;//Ԥ��λ����һ����0x00
	
	check_sum = (0x62 + 0x63 + cmd + *result + *parameter + MG200_Recv.data[6]) & 0xff;
	if(MG200_Recv.data[7] != check_sum)	return 6;
	return 0;
}

/******************************************************
*�� �� ����CaptureAndExtract
*�������ܣ�ָ�Ʋɼ�
*��    ����parameter �ڼ��βɼ�   
*�� �� ֵ��0�ɹ�   0xffͨ��ʧ��  ��������
*��    ע��
*******************************************************/
u8 CaptureAndExtract(u8 parameter)
{
	u8 Result, ACKparameter;
    MG200_Send_Cmd(0x51, parameter);	// ������ȡ�����������ݰ�
	if(MG200_Read_Cmd(0x51, &Result, &ACKparameter)!= 0)	// ���շ��ص����ݰ�����
	{
		printf("ͨ��ʧ��\r\n");
	    return 0xff;
	}
	switch (Result)
	{
		case 0x00: printf("ץȡָ��ͼ����������ȡ�ɹ���\r\n"); break;
		case 0xb1: printf("ָ�ƹ�С(small finger)��\r\n"); break;
		case 0xb2: printf("��ָ��(no finger)��\r\n"); break;
		case 0xb3: printf("ָ��λ�ù��ڿ���, ��Ҫ����ָ���ұ�����(left finger)��\r\n"); break;
		case 0xb4: printf("ָ��λ�ù��ڿ���, ��Ҫ����ָ���������(right finger)��\r\n"); break;
		case 0xb5: printf("ָ��λ�ù��ڿ���, ��Ҫ����ָ���±�����(up finger)��\r\n"); break;
		case 0xb6: printf("ָ��λ�ù��ڿ���, ��Ҫ����ָ���ϱ�����(down finger)��\r\n"); break;
		case 0xb7: printf("��ָ��ʪ(ָ�Ʊ����������ˮ�չ���)(wet finger)��\r\n"); break;
		case 0xb8: printf("������ָ(dry finger)��\r\n"); break;
		case 0xc0: printf("������ȡ����\r\n"); break;
		case 0xc2: printf("������ȡ����(ͼ��������)��\r\n"); break;
		default:   printf("ץȡָ��ͼ����������ȡ����\r\n"); break;
	}
	return Result;
}



/******************************************************
*�� �� ����CaptureAndExtract
*�������ܣ�ָ��ע��
*��    ����ID Ҫע�����ID��
*�� �� ֵ��0�ɹ�   0xffͨ��ʧ��    ��������
*��    ע��
*******************************************************/
u8 Enroll(u8 ID)
{
	u8 Result = 0, ACKparameter = 0;
	printf("��ʼע��ָ�ƣ�\r\n");
	/*���βɼ��׶�*/
go0:	
	printf("��һ�βɼ����뽫��ָ����\r\n");
	Voice_SendCmd(0x10);
	while(MG200_DETECT == 0);			//�ȴ�����
	delay_ms(500);
	if(CaptureAndExtract(0x00)!= 0x00)	//��ȡ������ʧ��
	{
		printf("��һ�βɼ�ʧ�ܣ������°���ָ��\r\n"); printf("\r\n");
		while(MG200_DETECT == 1);		//�ȴ��ɿ�
		goto go0;
	}
	printf("��һ�βɼ��ɹ������ɿ���ָ\r\n"); printf("\r\n");
	while(MG200_DETECT == 1);			//�ȴ��ɿ�
	
go1:	
	printf("�ڶ��βɼ����뽫��ָ����\r\n");
	Voice_SendCmd(0x10);
	while(MG200_DETECT == 0);			//�ȴ�����
	delay_ms(500);
	if(CaptureAndExtract(0x01)!= 0x00)	//��ȡ������ʧ��
	{
		printf("�ڶ��βɼ�ʧ�ܣ������°���ָ��\r\n"); printf("\r\n");
		while(MG200_DETECT == 1);		//�ȴ��ɿ�
		goto go1;
	}
	printf("�ڶ��βɼ��ɹ������ɿ���ָ\r\n"); printf("\r\n");
	while(MG200_DETECT == 1);			//�ȴ��ɿ�
	
go2:
	printf("�����βɼ����뽫��ָ����\r\n");
	Voice_SendCmd(0x10);
	while(MG200_DETECT == 0);			//�ȴ�����
	delay_ms(500);
	if(CaptureAndExtract(0x02)!= 0x00)	//��ȡ������ʧ��
	{
		printf("�����βɼ�ʧ�ܣ������°���ָ��\r\n"); printf("\r\n");
		while(MG200_DETECT == 1);		//�ȴ��ɿ�
		goto go2;
	}
	printf("�����βɼ��ɹ������ɿ���ָ\r\n"); printf("\r\n");
	while(MG200_DETECT == 1);			//�ȴ��ɿ�
	
	/*ע��׶�*/
	delay_ms(200);
	//ע���ID�ţ�ID��Χ1~100������û���Ϊ100�����ò���Ϊ00hʱ��ģ��ע���ID�����ڲ��Զ�����ģ�
	MG200_Send_Cmd(0x7f,ID);
	
	if(MG200_Read_Cmd(0x7f, &Result, &ACKparameter) == 0xff)	//���շ��ص����ݰ�����
	{
		printf("ͨ��ʧ��\r\n");
		return 0xff;
	}
	switch(Result)
	{
		case 0x00: printf("ע��ָ�Ƴɹ���\r\n");Voice_SendCmd(0x1c); break;
		case 0x83: printf("ID ����(ID < 0 �� ID > ����û���)��ͨ�Ŵ���\r\n"); break;
		case 0x91: printf("ע��ʧ��(�û���������)\r\n"); break;
		case 0x93: printf("�Ѿ�ע��� ID\r\n"); break;
		case 0x94: printf("ָ����ȡ���� < 3\r\n"); break;
	}
	return Result;
}


/******************************************************
*�� �� ����Match1N
*�������ܣ�ָ�ƶԱ�
*��    ����ID �Աȳ���ID��
*�� �� ֵ��0�Աȳɹ�   0xffͨ��ʧ��    ��������
*��    ע��
*******************************************************/
u8 Match1N(u8 *ID)
{
	u8 Result = 0xfe;
	/*�ɼ�*/
//	printf("�������ָ\r\n");
	if(MG200_DETECT)
	{
		CaptureAndExtract(0x00); //�ɼ�
	
		/*�Ա�*/
		MG200_Send_Cmd(0x71,0x00);//���ͶԱ�ָ��
		if(MG200_Read_Cmd(0x71,&Result,ID)!=0)//�������ص����ݰ�
		{
			printf("ͨ��ʧ��\r\n");
			return 0xff;
		}
		if(Result == 0x00)
		{
			printf("�Աȳɹ�\r\n");
			Voice_SendCmd(0x12);//����"��ӭ�ؼ�"
			Door_Start(2000);
		}
		else
		{
			Voice_SendCmd(0x13);//����"����ʧ��"
			printf("�Ա�ʧ��\r\n");
		}
	}
	return Result;
}


/******************************************************
*�� �� ����EraseOne
*�������ܣ�ɾ��ĳһ��ָ��
*��    ����ID ɾ����ָ��ID
*�� �� ֵ��0 ɾ���ɹ�   0xffͨ��ʧ��    ��������
*��    ע��
*******************************************************/
u8 EraseOne(u8 ID)
{
	u8 Result = 0,parameter = 0;
	
	MG200_Send_Cmd(0x73,ID);//����ɾ��ָ��
	
	if(MG200_Read_Cmd(0x73,&Result,&parameter)!=0)//�������ص����ݰ�
	{
		printf("ͨ��ʧ��\r\n");
		return 0xff;
	}
	switch (Result)
	{
		case 0X00: printf("ɾ���ɹ���\r\n"); Voice_SendCmd(0x1c);break;
		case 0X83: printf("��������(ID �� 0 ���� ID > ����û���)\r\n"); break;
		case 0X90: printf("δע����û�\r\n"); break;
		case 0XFF: printf("д�� ROM ����\r\n"); break;
	}
	return Result;
}


/******************************************************
*�� �� ����EraseAll
*�������ܣ�ɾ��ĳһ��ָ��
*��    ����None
*�� �� ֵ��0 ɾ���ɹ�   0xffͨ��ʧ��    ��������
*��    ע��
*******************************************************/
u8 EraseAll(void)
{
	u8 Result = 0,parameter = 0;
	MG200_Send_Cmd(0x54,0x00);//����ɾ��ָ��
	if(MG200_Read_Cmd(0x54,&Result,&parameter)!=0)//�������ص����ݰ�
	{
		printf("ͨ��ʧ��\r\n");
		return 0xff;
	}
	switch(Result)
	{
		case 0X00: printf("ɾ��ȫ���û���Ϣ�ɹ�\r\n"); Voice_SendCmd(0x1c);break;
		case 0X90: printf("ɾ��ʧ��(ע����û���Ϊ 0 ��ʱ��)\r\n"); break;
		default:   printf("ɾ��ʧ��\r\n"); break;
	}
	return Result;
}



