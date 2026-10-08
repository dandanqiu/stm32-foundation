#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
void my_usart_init(void);
void HCSR04_Init(void);
int main(void)
{
	my_usart_init();
	HCSR04_Init();
	//My_USART_Printf(USART1, "Hello, World!\r\n");
	

	while(1)
	{
		// 计数器清零
	TIM_SetCounter(TIM1, 0);
	// 清除 CC1 和 CC2
	TIM_ClearFlag(TIM1, TIM_FLAG_CC1 | TIM_FLAG_CC2);
	// 使能 TIM1
	TIM_Cmd(TIM1, ENABLE);
	// 向超声波模块发送 10us 的高电平脉冲
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	DelayUs(10);
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	//查询捕获标志位
	while(!TIM_GetFlagStatus(TIM1, TIM_FLAG_CC1));
	while(!TIM_GetFlagStatus(TIM1, TIM_FLAG_CC2));
	// 关闭 TIM1
	TIM_Cmd(TIM1, DISABLE);
	// 计算脉冲宽度
	uint16_t pulse_width = TIM_GetCapture2(TIM1) - TIM_GetCapture1(TIM1);
	// 在串口上打印距离
	float daistance = pulse_width * 1.0e-6f * 340.0f / 2.0f;
	My_USART_Printf(USART1, "Distance: %.4f m\r\n", daistance);
	Delay(500);
	}
}
void my_usart_init(void)
{
	// 初始化PA9 和 PA10
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// 初始化USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}
void HCSR04_Init(void)
{
	// 初始化 CH1 PA8 和 PA0
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// 初始化 时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter  = 0;
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	//初始化 输入捕获
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
}

