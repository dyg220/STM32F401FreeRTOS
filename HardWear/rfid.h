#ifndef _RFID_H_
#define _RFID_H_

#include "stm32f4xx.h"                  // Device header
#include "spi.h"
#include "systick.h"
#include "stdio.h"
#include "string.h"
#include "voice.h"
#include "door.h"

//RC522命令字
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

//Mifare_One卡片命令字
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

//MF522 FIFO长度定义
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte
#define MAXRLEN  18

//MFRC522寄存器地址定义
//PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		  		  0x3F

//和MF522通讯时返回的错误代码 
#define	  MI_OK             0
#define	  MI_NOTAGERR      (1)
#define   MI_ERR           (2)

#define	  SHAQU1		0X01
#define	  KUAI4			0X04
#define	  KUAI7			0X07
#define	  REGCARD		0XA1
#define	  CONSUME		0XA2
#define   READCARD		0XA3
#define   ADDMONEY		0XA4

#define RFID_CS_H 		GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET)
#define RFID_CS_L  		GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET)
#define RFID_RST_H 		GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_SET)
#define RFID_RST_L 		GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_RESET)

/***************************************库函数******************************************/
char PcdReset(void);                 //复位函数
void PcdAntennaOff(void);            //关闭天线
void PcdAntennaOn(void);             //开启天线
char M500PcdConfigISOType(u8 type);  //配置协议
char PcdRequest(u8 req_code,u8 *pTagType);  //寻卡
char PcdAnticoll(u8 *pSnr);                 //防冲撞
char PcdSelect(u8 *pSnr);                   //选卡
char PcdAuthState(u8  auth_mode,u8 addr,u8 *pKey,u8 *pSnr);  //验证密码
char PcdWrite(u8   addr,u8 *p );                             //写数据函数
char PcdRead(u8   addr,u8 *p );                              //读数据
char PcdHalt (void);                                         //睡眠
/***************************************库函数******************************************/

void RFID_Init(void);//RFID初始化
u8 PCD_distinguish_PICC(u8 *pSnr);//读卡ID
u8 WriteCardData(u8 addr,u8 *data,u8 *pSnr);//向卡中写数据
u8 ReadCardData(u8 addr,u8 *data,u8 *pSnr);//从卡中读数据

void RFID_Register(u8 *ID);
void RFID_Open_Door(void);

#endif

