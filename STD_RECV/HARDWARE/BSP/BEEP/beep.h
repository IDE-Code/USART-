#ifndef _BEEP_H_
#define _BEEP_H_

#include "sys.h"

typedef enum
{
  BEEPState_OFF =0,
	BEEPState_ON,
}BEEPState_TypeDef;
#define IS_BEEP_STATE(STATE)           (((STATE) == BEEPState_OFF) || ((STATE) == BEEPState_ON))

// B11
#define BEEP_RCC_CLOCKCMD             RCC_APB2PeriphClockCmd
#define BEEP_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define BEEP_GPIO_PIN                 GPIO_Pin_11
#define BEEP_GPIO                     GPIOB

#if 0         /* 调用标准库函数方法 */

#define BEEP_ON                       GPIO_SetBits(BEEP_GPIO,BEEP_GPIO_PIN)
#define BEEP_OFF                      GPIO_ResetBits(BEEP_GPIO,BEEP_GPIO_PIN)
#define BEEP_TOGGLE                   {BEEP_GPIO->ODR ^=BEEP_GPIO_PIN;}  //输出反转

#else       /* 直接操作寄存器方法 */

#define BEEP_ON                       {BEEP_GPIO->BSRR=BEEP_GPIO_PIN;}    //输出高电平
#define BEEP_OFF                      {BEEP_GPIO->BRR=BEEP_GPIO_PIN;}   //输出低电平
#define BEEP_TOGGLE                   {BEEP_GPIO->ODR ^=BEEP_GPIO_PIN;}  //输出反转

#endif
void BEEP_GPIO_Init(void);
void BEEP_StateSet(BEEPState_TypeDef state);

#endif



