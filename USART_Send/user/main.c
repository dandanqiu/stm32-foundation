#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
void My_SendBytes(USART_TypeDef *USARTX,uint8_t *pdata,uint16_t size);
void USART1_Init(void);
int main(void)
{
	Delay_Init();
	USART1_Init();
	USART_Cmd(USART1, ENABLE);//闭合总开关

	//	uint8_t data[] = {1,2,3,4,5};//发送数字
//	My_SendBytes(USART1,data,5);
	
	while(1)
	{
		printf("hello\r\n");//发送字符
		Delay(100);
	}
}

void My_SendBytes(USART_TypeDef *USARTX,uint8_t *pdata,uint16_t size)
{
	for(uint16_t i = 0;i < size;i++)
	{
		while(USART_GetFlagStatus(USARTX,USART_FLAG_TXE) == Bit_RESET);//判断为空
		USART_SendData(USARTX,pdata[i]);//写入数据
	}
	while(USART_GetFlagStatus(USARTX,USART_FLAG_TC));//判断是否发送成功
}	
	
 void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
		//	// PA9 tx
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStruct);
//	
//	// PA10 rx
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(GPIOA, &GPIO_InitStruct);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);//对USART1进行重映射
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//Pb6 tx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//Pb7 rx
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits  = USART_StopBits_1;
	USART_InitStruct.USART_Parity  = USART_Parity_No;
	
	USART_Init(USART1,&USART_InitStruct);
}	

int fputc(int ch,FILE* f)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == Bit_RESET);//等待为空
	USART_SendData(USART1,(uint8_t)ch);//发送数据
	return ch;	
}



