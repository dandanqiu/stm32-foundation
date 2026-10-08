#include "stm32f10x.h"
volatile uint32_t currenttime = 0;
void App_TIM3_TimeBaseInit(void);
void TIM3_IRQHandler(void);
void delaytime(uint32_t time);
void App_OnBoardLED_Init(void);
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	App_TIM3_TimeBaseInit();
	App_OnBoardLED_Init();
	while(1)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
		delaytime(50);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		delaytime(500);
	}
}
void App_TIM3_TimeBaseInit(void)
{
	//先配置时基单元
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	//配置中断标志位
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//配置NVIC
	NVIC_InitTypeDef NVIC_Initstruct = {0};
	NVIC_Initstruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_Initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Initstruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Initstruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstruct);
	TIM_Cmd(TIM3,ENABLE);
}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		currenttime++;
	}
}
void delaytime(uint32_t time)
{
	uint32_t start = currenttime;
	while((uint32_t)(currenttime - start) < time)
	{
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
