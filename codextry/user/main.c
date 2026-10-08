#include "stm32f10x.h"
#include "delay.h"

void onboard_led_init(void);

int main(void)
{
	onboard_led_init(); // 初始化板载 LED
	while(1)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); // 点亮 LED
		Delay(500); // 延时 500 毫秒
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); // 熄灭 LED
		Delay(500); // 延时 500 毫秒
	}
}
void onboard_led_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能 GPIOC 时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // 选择引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // 输出速度
	GPIO_Init(GPIOC, &GPIO_InitStructure); // 初始化 GPIOC
}