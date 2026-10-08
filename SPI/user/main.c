#include "stm32f10x.h"
#include "button.h"
void spi_init(void);
void spi_master_rceivetransmit(SPI_TypeDef *SPIX,const uint8_t *pdatatx,uint8_t *pdatarx,uint16_t size);
void App_W25Q64_SaveByte(uint8_t Byte);
uint8_t App_W25Q64_LoadByte(void);
void button_init(void);
Button_TypeDef button1;
void button_clicked_cb(uint8_t clicks);
void App_OnBoardLED_Init(void);
int main(void)
{
	spi_init();
	SPI_Cmd(SPI1,ENABLE);
	App_OnBoardLED_Init();
	button_init();
	uint8_t a = App_W25Q64_LoadByte();
	if(a == 0x00)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	}
	if(a == 0x01)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
	}
	App_W25Q64_SaveByte(0x12);
	a = App_W25Q64_LoadByte();
	
	while(1)
	{
		My_Button_Proc(&button1);
	}
}
void spi_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//初始化引脚
	//PA6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA7 PA5 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA15
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//对SPI进行初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitStruct);
	SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Set);
}
	void spi_master_rceivetransmit(SPI_TypeDef *SPIX,const uint8_t *pdatatx,uint8_t *pdatarx,uint16_t size)
	{
		//开启开关
		SPI_Cmd(SPIX,ENABLE);
		//先写入第一个字节
		SPI_I2S_SendData(SPIX,pdatatx[0]);
		for(uint16_t i = 0; i<size-1;i++)
		{
			while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_TXE) == RESET);
			SPI_I2S_SendData(SPIX,pdatatx[i+1]);
			while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_RXNE) == RESET);
			pdatarx[i] = SPI_I2S_ReceiveData(SPIX);
		}
		//接受最后一个数据
		while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_RXNE) == RESET);
		pdatarx[size-1] = SPI_I2S_ReceiveData(SPIX);
		SPI_Cmd(SPIX,DISABLE);
	}
	void App_W25Q64_SaveByte(uint8_t Byte)
	{
		uint8_t buffer[10];
		//写使能
		buffer[0] = 0x06;
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中
		spi_master_rceivetransmit(SPI1,buffer,buffer,1);
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
		
		//扇区擦除
		buffer[0] = 0x20;
		buffer[1] = 0x00;
		buffer[2] = 0x00;
		buffer[3] = 0x00;
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
		spi_master_rceivetransmit(SPI1,buffer,buffer,4);
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
		//等待空闲
		while(1)
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
			buffer[0] = 0x05;
			spi_master_rceivetransmit(SPI1,buffer,buffer,1);
			buffer[0] = 0xff;
			spi_master_rceivetransmit(SPI1,buffer,buffer,1);
			GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
			if((buffer[0] & 0x01) == 0)break;
		}
		//写使能
		buffer[0] = 0x06;
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中
		spi_master_rceivetransmit(SPI1,buffer,buffer,1);
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
		//编程
		GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET); // 选中
	
		buffer[0] = 0x02;
		buffer[1] = 0x00;
		buffer[2] = 0x00;
		buffer[3] = 0x00;
		buffer[4] = Byte;
		spi_master_rceivetransmit(SPI1,buffer,buffer,5);
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
		//等待空闲
		while(1)
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);
			buffer[0] = 0x05;
			spi_master_rceivetransmit(SPI1,buffer,buffer,1);
			buffer[0] = 0xff;
			spi_master_rceivetransmit(SPI1,buffer,buffer,1);
			GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);
			if((buffer[0] & 0x01) == 0)break;
		}
	}
	uint8_t App_W25Q64_LoadByte(void)
	{
		uint8_t buffer[10];
	
		GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_RESET); // 选中
	
		buffer[0] = 0x03;
		buffer[1] = 0x00;
		buffer[2] = 0x00;
		buffer[3] = 0x00;
		spi_master_rceivetransmit(SPI1,buffer,buffer,4);
		buffer[0] = 0xff;
		spi_master_rceivetransmit(SPI1,buffer,buffer,1);
		GPIO_WriteBit(GPIOA, GPIO_Pin_15, Bit_SET);
	  return buffer[0];
	}
	void button_init(void)
	{
		Button_InitTypeDef Button_InitStruct = {0};
		Button_InitStruct.GPIOx = GPIOA;
		Button_InitStruct.GPIO_Pin = GPIO_Pin_0;
		Button_InitStruct.button_clicked_cb = button_clicked_cb;
		My_Button_Init(&button1,&Button_InitStruct);
	}
			
	void button_clicked_cb(uint8_t clicks)
	{
		if(clicks == 1)
		{
			if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13) == Bit_RESET)
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
				App_W25Q64_SaveByte(0x00);
			}
			else
			{
				GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
				App_W25Q64_SaveByte(0x01);
			}
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
}	
		
			
		
				
		
