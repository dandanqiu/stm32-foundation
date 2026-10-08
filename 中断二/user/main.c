#include "stm32f10x.h"
void App_OnBoardLED_Init(void);
void button_init(void);
int main(void)
{
	App_OnBoardLED_Init();
	button_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	while(1)
	{
	}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		EXTI_ClearFlag(EXTI_Line5);
	}
	if(EXTI_GetFlagStatus(EXTI_Line6) == SET)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
		EXTI_ClearFlag(EXTI_Line6);
	}
}
		
void App_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
}
	
void button_init(void)
{
	//初始化引脚
	//PA6 PA7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_EventOutputConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
	GPIO_EventOutputConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
	//初始化EXTI
	EXTI_InitTypeDef EXTI_InitSruct = {0};
	EXTI_InitSruct.EXTI_Line = EXTI_Line5 | EXTI_Line6;
	EXTI_InitSruct.EXTI_LineCmd = ENABLE;
	EXTI_InitSruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitSruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitSruct);
	//配置中断 
	NVIC_InitTypeDef NVIC_InitStruct = {0};
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
	