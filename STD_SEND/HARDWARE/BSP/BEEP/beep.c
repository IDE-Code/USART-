#include "beep.h"

void BEEP_GPIO_Init(void) //BEEP Init
{
   GPIO_InitTypeDef GPIO_InitStructure;
	
  BEEP_RCC_CLOCKCMD(BEEP_RCC_CLOCKGPIO, ENABLE);


  GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);
  GPIO_ResetBits(BEEP_GPIO,BEEP_GPIO_PIN); 
}

void BEEP_StateSet(BEEPState_TypeDef state)
{
  assert_param(IS_BEEP_STATE(state));
	
	if(state==BEEPState_ON)
	{
	  GPIO_SetBits(BEEP_GPIO,BEEP_GPIO_PIN);  
	}
	else
	{
	  GPIO_ResetBits(BEEP_GPIO,BEEP_GPIO_PIN);
	}
}

