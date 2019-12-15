#ifndef _KEY_H_
#define _KEY_H_

#include "sys.h"

typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}KEYState_TypeDef;

#define KEY1_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOA
#define KEY1_GPIO_PIN                 GPIO_Pin_0
#define KEY1_GPIO                     GPIOA
#define KEY1_DOWN_LEVEL               1  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�ߵ�ƽ��������������Ϊ1 */

#define KEY2_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOA
#define KEY2_GPIO_PIN                 GPIO_Pin_1
#define KEY2_GPIO                     GPIOA
#define KEY2_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

void KEY_GPIO_Init(void);
KEYState_TypeDef KEY1_StateRead(void);
KEYState_TypeDef KEY2_StateRead(void);

#endif


