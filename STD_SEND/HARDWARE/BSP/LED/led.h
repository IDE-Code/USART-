#ifndef _LED_H_
#define _LED_H_

#include "sys.h"


typedef enum
{
  LED_OFF =0,
	LED_ON =1,
}LEDState_TypeDef;
#define IS_LED_STATE(STATE) (((STATE)==LED_OFF)||((STATE) == LED_ON))

#define LED1 (uint8_t)0x01
#define LED2 (uint8_t)0x02
#define LED3 (uint8_t)0x04
#define IS_LED_TYPEDEF(LED)           (((LED) == LED1) || ((LED) == LED2) || ((LED) == LED3))

#define LED1_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define LED1_GPIO_PIN                 GPIO_Pin_0
#define LED1_GPIO                     GPIOB


#define LED2_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define LED2_GPIO_PIN                 GPIO_Pin_1
#define LED2_GPIO                     GPIOB


#define LED3_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define LED3_GPIO_PIN                 GPIO_Pin_10
#define LED3_GPIO                     GPIOB

			
#if 0         /* 调用标准库函数方法 */
#define LED1_ON                       GPIO_SetBits(LED1_GPIO,LED1_GPIO_PIN)
#define LED1_OFF                      GPIO_ResetBits(LED1_GPIO,LED1_GPIO_PIN)
#define LED1_TOGGLE                   {LED1_GPIO->ODR ^=LED1_GPIO_PIN;}  //输出反转

#define LED2_ON                       GPIO_SetBits(LED2_GPIO,LED2_GPIO_PIN)
#define LED2_OFF                      GPIO_ResetBits(LED2_GPIO,LED2_GPIO_PIN)
#define LED2_TOGGLE                   {LED2_GPIO->ODR ^=LED2_GPIO_PIN;}  //输出反转

#define LED3_ON                       GPIO_SetBits(LED3_GPIO,LED3_GPIO_PIN)
#define LED3_OFF                      GPIO_ResetBits(LED3_GPIO,LED3_GPIO_PIN)
#define LED3_TOGGLE                   {LED3_GPIO->ODR ^=LED3_GPIO_PIN;}  //输出反转

#else       /* 直接操作寄存器方法 */

#define LED1_ON                       {LED1_GPIO->BSRR=LED1_GPIO_PIN;}    //输出低电平
#define LED1_OFF                      {LED1_GPIO->BRR=LED1_GPIO_PIN;}   //输出高电平
#define LED1_TOGGLE                   {LED1_GPIO->ODR ^=LED1_GPIO_PIN;}  //输出反转

#define LED2_ON                       {LED2_GPIO->BSRR=LED2_GPIO_PIN;}    //输出低电平
#define LED2_OFF                      {LED2_GPIO->BRR=LED2_GPIO_PIN;}   //输出高电平
#define LED2_TOGGLE                   {LED2_GPIO->ODR ^=LED2_GPIO_PIN;}  //输出反转

#define LED3_ON                       {LED3_GPIO->BSRR=LED3_GPIO_PIN;}    //输出低电平
#define LED3_OFF                      {LED3_GPIO->BRR=LED3_GPIO_PIN;}   //输出高电平
#define LED3_TOGGLE                   {LED3_GPIO->ODR ^=LED3_GPIO_PIN;}  //输出反转

#endif


/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void LED_GPIO_Init(void);
void LEDx_StateSet(uint8_t LEDx,LEDState_TypeDef state);

#endif

