#include "stm32f10x.h"
void on_board_init(void);
void systeamclock_init(void);

int main(void)
{
	systeamclock_init();
	on_board_init();
	while(1)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		for(uint32_t i = 0;i<666666;i++);//延迟五百毫秒
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
		for(uint32_t i = 0;i<666666;i++);//延迟五百毫秒
	}
}
void on_board_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
}
void systeamclock_init(void)
{
	//开启缓冲区并等待两个周期
	FLASH_PrefetchBufferCmd(ENABLE); 
	FLASH_SetLatency(FLASH_Latency_2);
	//开启HSE
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	
	//配置并启动锁相环
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//选择HSE x9
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//配置AHB APB1 APB2分频器
	RCC_HCLKConfig(RCC_SYSCLK_Div1); //HCLK = SYSCLK/1
	RCC_PCLK1Config(RCC_HCLK_Div2); //PCLK1 = HCLK/2
	RCC_PCLK2Config(RCC_HCLK_Div1);//PCLK = HCLK/1
	//切换sysclk的来源
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
}



