#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_
#include "stm32f4xx.h"

#define MOTOR_IA_High    GPIO_SetBits(GPIOB,GPIO_Pin_3);
#define MOTOR_IA_Low     GPIO_ResetBits(GPIOB,GPIO_Pin_3);
#define MOTOR_IB_High    GPIO_SetBits(GPIOB,GPIO_Pin_4);
#define MOTOR_IB_Low     GPIO_ResetBits(GPIOB,GPIO_Pin_4);

void dcMotor_Config(void);
void DCMotor_SetSpeed(int speed);


#endif
