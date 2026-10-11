#include "stm32f10x.h"
void adc_init(void);
void onboard_led_init(void);
int main(void)
{
	adc_init();
	onboard_led_init();
	while(1)
	{
		//清除标志位
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
		//软件启动
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		//等待转换完成
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		//获取转换结果
		uint16_t adc_value = ADC_GetConversionValue(ADC1);
		//处理adc_value为实际电压值
		float voltage = (float)adc_value *(3.3 / 4095.0);
		//用电压值判断是否点亮LED
		if(voltage > 1.5)
		{
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
		}
		else
		{
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		}

	}
}
void adc_init(void)
{
	//初始化PA0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置ADC的时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//初始化ADC1
	ADC_InitTypeDef ADC_InitStructure = {0};
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	//配置常规序列
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
}

void onboard_led_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}