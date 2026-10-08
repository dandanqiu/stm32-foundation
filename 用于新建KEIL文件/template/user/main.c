#include "stm32f10x.h"
#include "delay.h"
#include <math.h>

void App_OnBoardLED_Init(void);
void pwm_init(void);

int main(void)
{
    App_OnBoardLED_Init();
    pwm_init();

    while (1)
    {
        float t = GetTick() * 0.001f;
        float duty_ratio = 0.5f * (sinf(2.0f * 3.141592653589793f * 0.5f * t) + 1.0f);
        uint32_t duty = (uint32_t)(duty_ratio * 1000.0f);

        TIM_SetCompare1(TIM1, duty);
    }
}

void App_OnBoardLED_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void pwm_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct_B = {0};
    GPIO_InitStruct_B.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct_B.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct_B.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct_B);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_TimeBaseInitTypeDef TIM1_InitStruct = {0};
    TIM1_InitStruct.TIM_RepetitionCounter = 0;
    TIM1_InitStruct.TIM_Prescaler = 71;
    TIM1_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM1_InitStruct.TIM_Period = 999;
    TIM_TimeBaseInit(TIM1, &TIM1_InitStruct);
    TIM_ARRPreloadConfig(TIM1, ENABLE);

    TIM_OCInitTypeDef TIM1_OCInitStruct = {0};
    TIM1_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM1_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM1_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM1_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM1_OCInitStruct.TIM_Pulse = 0;
    TIM1_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &TIM1_OCInitStruct);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

