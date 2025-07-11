#include "dc_motor.h"
#include "systick.h"

void dcMotor_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	// ʹ��AFIOʱ�� (ע�⣺STM32F4ϵ����Ҫͨ��SYSCFG���ʣ����Ǵ�ͳAFIO)
	

	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //������ʱ��3��ʱ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1; //Ԥ��Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 101 - 1; //10ms 0-100
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //ʱ�ӷ�Ƶ������
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; //ֻ���ڸ߼���ʱ��
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct); //��ʼ����ʱ��3

	TIM_InternalClockConfig(TIM2); //ѡ���ڲ�ʱ��Դ

	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWMģʽ1: CNT<CCR ��Ч��ƽ
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //���ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0; //ccr
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //���Ը�
	TIM_OC1Init(TIM2, &TIM_OCInitStruct); //ͨ��1��ʼ��
	TIM_OC2Init(TIM2, &TIM_OCInitStruct); //ͨ��1��ʼ��
	//ARR CCR
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ARRԤװ��
	TIM_CCPreloadControl(TIM2, ENABLE); //CCRԤװ��

	TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��3

}
/*
speed: 0-100	 -��ת +��ת
*/
void DCMotor_SetSpeed(int speed)
{
	if (speed > 100)
	{
		speed = 100;
	}
	if (speed < -100)
	{
		speed = -100;
	}
	if (speed >= 0)
	{
		TIM_SetCompare1(TIM2, speed); //����ͨ��1�ıȽ�ֵ
		TIM_SetCompare2(TIM2, 0); //
	}
	else
	{
		TIM_SetCompare1(TIM2, 0); //����ͨ��1�ıȽ�ֵ
		TIM_SetCompare2(TIM2, -speed); //
	}

}
