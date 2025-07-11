#include "dc_motor.h"
#include "systick.h"

void dcMotor_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	// 使能AFIO时钟 (注意：STM32F4系列需要通过SYSCFG访问，而非传统AFIO)
	

	//定义结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //开启定时器3的时钟
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1; //预分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 101 - 1; //10ms 0-100
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; //时钟分频，不用
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0; //只用于高级定时器
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct); //初始化定时器3

	TIM_InternalClockConfig(TIM2); //选择内部时钟源

	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1: CNT<CCR 有效电平
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //输出使能
	TIM_OCInitStruct.TIM_Pulse = 0; //ccr
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStruct); //通道1初始化
	TIM_OC2Init(TIM2, &TIM_OCInitStruct); //通道1初始化
	//ARR CCR
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ARR预装载
	TIM_CCPreloadControl(TIM2, ENABLE); //CCR预装载

	TIM_Cmd(TIM2, ENABLE); //使能定时器3

}
/*
speed: 0-100	 -左转 +右转
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
		TIM_SetCompare1(TIM2, speed); //设置通道1的比较值
		TIM_SetCompare2(TIM2, 0); //
	}
	else
	{
		TIM_SetCompare1(TIM2, 0); //设置通道1的比较值
		TIM_SetCompare2(TIM2, -speed); //
	}

}
