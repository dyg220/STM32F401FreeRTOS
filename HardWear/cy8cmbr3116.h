#ifndef _CY8CMBR3116_H_
#define _CY8CMBR3116_H_

#include "stm32f4xx.h"                  // Device header
#include "iic.h"
#include "stdio.h"
#include "voice.h"
#include "string.h"
#include "door.h"

typedef struct{
	u8 password[10];//���뻺����
	u8 index;//����λ��
}CY_INFO;

#define CY8CMBR3116_ADDR	(0x37<<1)   //������ַ�ճ���дλ

#define CY8CMBR3116_RST_H	(GPIO_SetBits(GPIOA,GPIO_Pin_1))
#define CY8CMBR3116_RST_L	(GPIO_ResetBits(GPIOA,GPIO_Pin_1))

#define BUTTON_STA			0xAA		//����״̬�Ĵ�����ַ
#define SYSTEM_STATUS		0x8a		//0: ��Ĭ�ϵĳ�������	1: Ĭ�ϵĳ�������
#define CTRL_CMD 			0x86			
#define CTRL_CMD_STATUS		0x88		//0: û���κδ���		1: �����˴���
#define CTRL_CMD_ERR		0x89		
//0: ָ��ִ�гɹ�   0xfd��������д��ʧ��  0xfe��CONFIG_CRC ���洢������ CRC У��������õ������� CRC У��Ͳ�һ��  0xff��ָ����Ч
#define REGCFG_ADDR			0x00 		//���üĴ�����ʼ


u8 CY8CMBR3116_regcfg(u8 dfg_flag);
void CY8CMBR3116_Init(void);
u8 CY8CMBR3116_Read_Key(void);
u8 CY8CMBR3116_Key_Scan(void);
void CY8CMBR3116_Open_Door(void);

#endif

