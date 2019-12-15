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

#if 0         /* ���ñ�׼�⺯������ */

#define BEEP_ON                       GPIO_SetBits(BEEP_GPIO,BEEP_GPIO_PIN)
#define BEEP_OFF                      GPIO_ResetBits(BEEP_GPIO,BEEP_GPIO_PIN)
#define BEEP_TOGGLE                   {BEEP_GPIO->ODR ^=BEEP_GPIO_PIN;}  //�����ת

#else       /* ֱ�Ӳ����Ĵ������� */

#define BEEP_ON                       {BEEP_GPIO->BSRR=BEEP_GPIO_PIN;}    //����ߵ�ƽ
#define BEEP_OFF                      {BEEP_GPIO->BRR=BEEP_GPIO_PIN;}   //����͵�ƽ
#define BEEP_TOGGLE                   {BEEP_GPIO->ODR ^=BEEP_GPIO_PIN;}  //�����ת

#endif
void BEEP_GPIO_Init(void);
void BEEP_StateSet(BEEPState_TypeDef state);

#endif



