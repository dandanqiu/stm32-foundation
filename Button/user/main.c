#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "button.h"

void usart_init(void);
void onboard_led(void);
static void on_button_clicked(uint8_t clicks);

static Button_TypeDef btn;
static Button_InitTypeDef btn_init;

int main(void)
{
	Delay_Init();
	usart_init();
	onboard_led();

	btn_init.GPIOx = GPIOA;
	btn_init.GPIO_Pin = GPIO_Pin_1;
	btn_init.button_pressed_cb = 0;
	btn_init.button_released_cb = 0;
	btn_init.button_clicked_cb = on_button_clicked;
	btn_init.button_long_pressed_cb = 0;
	btn_init.LongPressTime = 0;
	btn_init.LongPressTickInterval = 0;
	btn_init.ClickInterval = 0;
	My_Button_Init(&btn, &btn_init);

	while(1)
	{
		My_Button_Proc(&btn);
	}
}

static void on_button_clicked(uint8_t clicks)
{
	if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == RESET)
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	else
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
}
			else
			{
			}
			Delay(10);
		}
	}
}
void usart_init(void)
{
	//定义引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//定义USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
}
	void onboard_led(void)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOC,&GPIO_InitStruct);
	}
	void button_init(void)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
	}
		
	
