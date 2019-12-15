#include "infra.h"
#include "delay.h"
#include "led.h"

void INFRA_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  INFRA1_RCC_CLOCKCMD(INFRA1_RCC_CLOCKGPIO|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=INFRA1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(INFRA1_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=INFRA2_GPIO_PIN;
	GPIO_Init(INFRA1_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=INFRA3_GPIO_PIN;
	GPIO_Init(INFRA1_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=INFRA4_GPIO_PIN;
	GPIO_Init(INFRA1_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=INFRA5_GPIO_PIN;
	GPIO_Init(INFRA1_GPIO,&GPIO_InitStructure);
}


