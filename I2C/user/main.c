#include "stm32f10x.h"
void I2C1_Init(void);
//该函数返回0成功，-1寻址失败，-2发送数据失败
int My_I2C_SendDate(I2C_TypeDef *I2CX,uint8_t Addr,uint8_t *pdata,uint16_t size);
int My_I2C_ReceiveDate(I2C_TypeDef *I2CX,uint8_t Addr,uint8_t *pdata,uint16_t size);
void Onboard_LED_Init(void);
int main(void)
{
	I2C1_Init();
	Onboard_LED_Init();
	I2C_Cmd(I2C1,ENABLE);//开启I2C1
	uint8_t commands[] = {0x00, 0x8d, 0x14, 0xaf, 0xa5};//使显示屏亮
	My_I2C_SendDate(I2C1, 0x78, commands, 5);
	uint8_t rcvd;
	
	My_I2C_ReceiveDate(I2C1, 0x78, &rcvd, 1);
	if((rcvd & (0x01 << 6)) == 0)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	}
	while(1)
	{	
	}	
}

void I2C1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIOB_InitStruct;
	//PB6 PB7
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOB_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIOB_InitStruct);
	//对I2C进行初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE); // 施加复位信号
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE); // 释放复位信号
	
	I2C_InitTypeDef I2C_InitStruct;
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	
	I2C_Init(I2C1,&I2C_InitStruct);
}

int My_I2C_SendDate(I2C_TypeDef *I2CX,uint8_t Addr,uint8_t *pdata,uint16_t size)
{
	//判断总线是否空闲
	while(I2C_GetFlagStatus(I2CX,I2C_FLAG_BUSY) == SET);
	//发送起始位
	I2C_GenerateSTART(I2CX,ENABLE);
	while(I2C_GetFlagStatus(I2CX,I2C_FLAG_SB)==RESET);
	//寻址阶段
	I2C_ClearFlag(I2CX,I2C_FLAG_AF);
	I2C_SendData(I2CX,Addr & 0xfe);
	while(1)
	{
		if(I2C_GetFlagStatus(I2CX,I2C_FLAG_AF) == SET)
		{
			I2C_GenerateSTOP(I2CX,ENABLE);//发送停止位
			return -1;//寻址失败
		}
		if(I2C_GetFlagStatus(I2CX,I2C_FLAG_ADDR) == SET)
		break;//寻址成功
	}
	// 清除ADDR
	I2C_ReadRegister(I2CX, I2C_Register_SR1);
	I2C_ReadRegister(I2CX, I2C_Register_SR2);
	//发送数据
	for(uint16_t i=0;i<size;i++)
	{
		while(1)
		{
			if(I2C_GetFlagStatus(I2CX,I2C_FLAG_AF)==SET)
			{
				I2C_GenerateSTOP(I2CX,ENABLE);
				return -2;//数据发送失败
			}
			if(I2C_GetFlagStatus(I2CX,I2C_FLAG_TXE) == SET)
				break;
		}
			I2C_SendData(I2CX,pdata[i]);
	}
	while(1)
	{
		if(I2C_GetFlagStatus(I2CX,I2C_FLAG_AF)==SET)
			{
				I2C_GenerateSTOP(I2CX,ENABLE);
				return -2;//数据发送失败
			}
		if(I2C_GetFlagStatus(I2CX,I2C_FLAG_BTF)==SET)
			break;//数据发送完毕
	}
		I2C_GenerateSTOP(I2CX,ENABLE);
		return 0;//发送成功
}
	
int My_I2C_ReceiveDate(I2C_TypeDef *I2CX,uint8_t Addr,uint8_t *pdata,uint16_t size)
{
	//检查总线是否空闲
	while(I2C_GetFlagStatus(I2CX,I2C_FLAG_BUSY) == SET);
	//发送起始位
	I2C_GenerateSTART(I2CX,ENABLE);
	while(I2C_GetFlagStatus(I2CX,I2C_FLAG_SB) == RESET);
	//发送地址
	I2C_ClearFlag(I2CX,I2C_FLAG_AF);
	I2C_SendData(I2CX,Addr | 0x01);
	while(1)
	{
		if(I2C_GetFlagStatus(I2CX,I2C_FLAG_AF) == SET)
		{
			I2C_GenerateSTOP(I2CX,ENABLE);
			return -1;//寻址失败
		}
		if(I2C_GetFlagStatus(I2CX,I2C_FLAG_ADDR) == SET)
			break;
	}
	//清除ADDR
	I2C_ReadRegister(I2CX, I2C_Register_SR1);
	I2C_ReadRegister(I2CX, I2C_Register_SR2);
	//开启ACK,将其置为1
	I2C_AcknowledgeConfig(I2CX,ENABLE);
	for(uint16_t i = 0 ;i<size - 1;i++)
	{
		while(I2C_GetFlagStatus(I2CX,I2C_FLAG_RXNE) == RESET);
		pdata[i] = I2C_ReceiveData(I2CX);
	}
	//将ACK值为零,接受最后一个数据
	I2C_AcknowledgeConfig(I2CX,DISABLE);
	while(I2C_GetFlagStatus(I2CX,I2C_FLAG_RXNE) == RESET);
	pdata[size - 1] = I2C_ReceiveData(I2CX);
	I2C_GenerateSTOP(I2CX,ENABLE);
	return 0;//接受成功
}

void Onboard_LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
}
		
		
		
	

