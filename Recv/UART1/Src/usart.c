/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#define UART1_RX_LEN 0xFF 				// 串口一次接受数据的最大长度
uint8_t UART1_RX_BUF[UART1_RX_LEN];		// 串口接收缓存
uint8_t UART1_RX_BYTE = 0;				// 串口中断接收一个字节
uint16_t UART1_RX_STA = 0;				// 串口接受状态，第15位表示接收到了换行符，第14为表示接收到了回车符，第0~7位表示本次接收到的数据长度

static uint32_t BaudRate = 115200;		// 默认波特率

/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

    huart1.Instance = USART1;
    huart1.Init.BaudRate = BaudRate;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(uartHandle->Instance==USART1)
    {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* USART1 clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

    if(uartHandle->Instance==USART1)
    {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

        /* USART1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 1000);
	return ch;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart1.Instance)
	{
		if ((UART1_RX_STA & 0x8000) == 0)			// 接收未完毕
		{
			if (UART1_RX_STA & 0x4000)				// 接收到了回车符
			{
				if (UART1_RX_BYTE == '\n')			// 本次接受到的是换行符
					UART1_RX_STA |= 0x8000;			// 标记接收结束
				else
					UART1_RX_STA = 0;				// 否则接收错误，状态置为初始
			}
			else
			{
				if (UART1_RX_BYTE == '\r')			// 本次接收到的是回车符
					UART1_RX_STA |= 0x4000;			// 标记接收到了回车符
				else
				{
					UART1_RX_BUF[UART1_RX_STA & 0x3FFF] = UART1_RX_BYTE;	// 本次接收到的是数据
					if (UART1_RX_STA++ >= UART1_RX_LEN)						// 连续接收到的数据高于最大接收长度，则视为接收错误
						UART1_RX_STA = 0;									// 状态置为初始
				}
			}
		}
		HAL_UART_Receive_IT(&huart1, &UART1_RX_BYTE, 1);					// 再次使能接收中断
	}
}

/**
 * 串口1接收处理函数
 */
void UART1_RecvHandler(void)
{
	if (UART1_RX_STA & 0x8000)										// 有数据收到
	{
		if(strstr((const char *)UART1_RX_BUF, "AT+:") != NULL)		// 查找包头
		{
			CmdHdr cmdhdr;
			memcpy(&cmdhdr, UART1_RX_BUF, sizeof(CmdHdr));
			//printf("angle:%f    value:%d \r\n", cmdhdr.angle, cmdhdr.value);		// 打印接收到的值
		}
		UART1_RX_STA = 0;
	}
}

bool UART1_Send(float angle, int value)
{
	CmdHdr cmdhdr;
	cmdhdr.hdr[0] = 'A';
	cmdhdr.hdr[1] = 'T';
	cmdhdr.hdr[2] = '+';
	cmdhdr.hdr[3] = ':';
	cmdhdr.angle = angle;
	cmdhdr.value = value;
	cmdhdr.end[0] = '\r';
	cmdhdr.end[1] = '\n';
	return HAL_UART_Transmit(&huart1, (uint8_t *)&cmdhdr, sizeof(CmdHdr), 1000) == HAL_OK ? true : false;
}

void UART1_Init(uint32_t baudrate)
{
	BaudRate = baudrate;
	MX_USART1_UART_Init();
	HAL_UART_Receive_IT(&huart1, &UART1_RX_BYTE, 1);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
