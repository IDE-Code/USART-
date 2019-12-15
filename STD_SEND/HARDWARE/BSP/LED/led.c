#include "led.h"

void LED_GPIO_Init()
{
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(LED1_RCC_CLOCKGPIO|LED2_RCC_CLOCKGPIO |LED3_RCC_CLOCKGPIO , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(LED1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=LED2_GPIO_PIN;
	GPIO_Init(LED2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=LED3_GPIO_PIN;
	GPIO_Init(LED3_GPIO, &GPIO_InitStructure);
	
	GPIO_ResetBits(LED1_GPIO,LED1_GPIO_PIN);
	GPIO_ResetBits(LED2_GPIO,LED2_GPIO_PIN);
	GPIO_ResetBits(LED3_GPIO,LED3_GPIO_PIN);
}

void LEDx_StateSet(uint8_t LEDx,LEDState_TypeDef state)
{
  assert_param(IS_LED_TYPEDEF(LEDx));
	assert_param(IS_LED_STATE(state));
	
	if(state ==LED_OFF)
	{
		if(LEDx & LED1)            
      GPIO_ResetBits(LED1_GPIO,LED1_GPIO_PIN);/* 设置引脚输出为低电平，此时LED1灭 */
    if(LEDx & LED2)
      GPIO_ResetBits(LED2_GPIO,LED2_GPIO_PIN);/* 设置引脚输出为低电平，此时LED2灭 */
    if(LEDx & LED3)
      GPIO_ResetBits(LED3_GPIO,LED3_GPIO_PIN);/* 设置引脚输出为低电平，此时LED3灭 */	
	}
	else
	{
	  if(LEDx & LED1)
      GPIO_SetBits(LED1_GPIO,LED1_GPIO_PIN);/* 设置引脚输出为高电平，此时LED1亮 */
    if(LEDx & LED2)
      GPIO_SetBits(LED2_GPIO,LED2_GPIO_PIN);/* 设置引脚输出为高电平，此时LED2亮 */
    if(LEDx & LED3)
      GPIO_SetBits(LED3_GPIO,LED3_GPIO_PIN);/* 设置引脚输出为高电平，此时LED3亮 */ 
	}
}

