#ifndef _INFRA_H_
#define _INFRA_H_

#include "sys.h"

#define INFRA1_PRES	1

#define INFRA1_RCC_CLOCKCMD             RCC_APB2PeriphClockCmd
#define INFRA1_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define INFRA1_GPIO_PIN                 GPIO_Pin_3
#define INFRA1_GPIO                     GPIOB
#define INFRA1_GPIO_PORTSOURCE          GPIO_PortSourceGPIOB
#define INFRA1_GPIO_PINSOURCE           GPIO_PinSource3 
#define INFRA1_INPUT_LEVEL              GPIO_ReadInputDataBit(INFRA1_GPIO,INFRA1_GPIO_PIN)
#define INFRA1_DOWN_LEVEL               0
#define INFRA1_RCC_CLOCKAFIO            RCC_APB2Periph_AFIO
#define INFRA1_EXITLINE                 EXTI_Line3
#define INFRA1_IRQCHANNEL               EXTI3_IRQn
#define INFRA1_IRQHANDLER               EXTI3_IRQHandler

#define INFRA2_RCC_CLOCKCMD             RCC_APB2PeriphClockCmd
#define INFRA2_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define INFRA2_RCC_CLOCKAFIO            RCC_APB2Periph_AFIO
#define INFRA2_GPIO                     GPIOB
#define INFRA2_GPIO_PIN                 GPIO_Pin_4
#define INFRA2_EXITLINE                 EXTI_Line4
#define INFRA2_GPIO_PORTSOURCE          GPIO_PortSourceGPIOB
#define INFRA2_GPIO_PINSOURCE           GPIO_PinSource4
#define INFRA2_IRQCHANNEL               EXTI4_IRQn
#define INFRA2_IRQHANDLER               EXTI4_IRQHandler
#define INFRA2_INPUT_LEVEL              GPIO_ReadInputDataBit(INFRA2_GPIO,INFRA2_GPIO_PIN)
#define INFRA2_DOWN_LEVEL               0


#define INFRA3_GPIO_PIN                 GPIO_Pin_5
#define INFRA3_GPIO                     GPIOB
#define INFRA3_DOWN_LEVEL               0
#define INFRA3_INPUT_LEVEL              GPIO_ReadInputDataBit(INFRA3_GPIO,INFRA3_GPIO_PIN)

#define INFRA4_GPIO_PIN                 GPIO_Pin_6
#define INFRA4_GPIO                     GPIOB
#define INFRA4_DOWN_LEVEL               0
#define INFRA4_INPUT_LEVEL              GPIO_ReadInputDataBit(INFRA4_GPIO,INFRA4_GPIO_PIN)

#define INFRA5_GPIO_PIN                 GPIO_Pin_7
#define INFRA5_GPIO                     GPIOB
#define INFRA5_DOWN_LEVEL               0
#define INFRA5_INPUT_LEVEL              GPIO_ReadInputDataBit(INFRA5_GPIO,INFRA5_GPIO_PIN)

void INFRA_Init();
u8 INFRA_Scan(u8 mode);
void INFRA1_EXIT_Config(void);
void INFRA2_EXIT_Config(void);
#endif




