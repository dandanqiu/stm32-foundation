#include "stm32f10x.h"
#include "delay.h"
int main(void)
{
	Delay_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;//先初始化IO引脚
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
//	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);//写1熄灭
//	
//	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);//写0点亮
	
	while(1)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);//亮
		Delay(100);//延迟100ms
		
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);//灭
		
		Delay(100);
	}
}
