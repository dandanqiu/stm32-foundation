#include "stm32f10x.h"
#include "delay.h"
#include <math.h>
#include "usart.h"
void usart_init(void);
void Tim3_init(void);
void Tim1_init(void);
int main(void)
{
	usart_init();
	Tim3_init();
	Tim1_init();
	while(1)
	{
		float t = GetTick() *1.0e-3f;
		float crr = 0.5*(sin(2*3.14*t)+1)*1000;
		TIM_SetCompare1(TIM3,crr);
		//清除Trigger标志位
		TIM_ClearFlag(TIM1, TIM_FLAG_Trigger);
		while(!TIM_GetFlagStatus(TIM1, TIM_FLAG_Trigger)); //等待触发事件发生
		uint16_t ccr1 = TIM_GetCapture1(TIM1); //获取捕获值
		uint16_t ccr2 = TIM_GetCapture2(TIM1); //获取捕获值
		float period =  ccr1 * 1.0e-6f *1.0e3f; //计算周期
		float duty = (float)ccr2 / (float)ccr1 * 100.0f; //计算占空比
		My_USART_Printf(USART1, "Period: %.3f ms, Duty: %.2f %%\r\n", period, duty); //打印周期和占空比
		Delay(500);
	}
}
void usart_init(void)
{
	//初始化引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //输出速率
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//初始化串口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitTypeDef USART_InitStructure = {0};
	USART_InitStructure.USART_BaudRate = 115200; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验
	USART_InitStructure.USART_Mode = USART_Mode_Tx; //发送模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE); //使能串口
}
void Tim3_init(void)
{
	//初始化时基单元
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; //预分频系数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 1000-1; //自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //重复计数器的值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3, ENABLE); //使能TIM3
	//初始化输出比较
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //选择引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //输出速率
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	TIM_OCInitTypeDef TIM_OCInitStructure = {0};
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM1模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能输出
	TIM_OCInitStructure.TIM_Pulse = 0; //比较值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); //初始化TIM3的通道1
	TIM_CCPreloadControl(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE); //使能TIM3的PWM输出
	 
}
void Tim1_init(void)
{
	//初始化时基单元
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; //预分频系数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = 65535; //自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //重复计数器的值
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	//初始化输出比较
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //选择引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_ICInitTypeDef TIM_ICInitStructure = {0};
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //选择通道1
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //直接输入
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //不分频
	TIM_ICInitStructure.TIM_ICFilter = 0; //输入滤波器
	TIM_ICInit(TIM1, &TIM_ICInitStructure); //初始化TIM1的通道1
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //选择通道2
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; //下降沿捕获
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI; //间接输入
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //不分频
	TIM_ICInitStructure.TIM_ICFilter = 0; //输入滤波器
	TIM_ICInit(TIM1, &TIM_ICInitStructure); //初始化TIM1的通道1
	//配置从模式
	TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1); //选择触发输入
	TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset); //选择从模式为复位模式
	TIM_Cmd(TIM1, ENABLE); //使能TIM1
}
