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
#define UART1_RX_LEN 0xFF 				// ����һ�ν������ݵ���󳤶�
uint8_t UART1_RX_BUF[UART1_RX_LEN];		// ���ڽ��ջ���
uint8_t UART1_RX_BYTE = 0;				// �����жϽ���һ���ֽ�
uint16_t UART1_RX_STA = 0;				// ���ڽ���״̬����15λ��ʾ���յ��˻��з�����14Ϊ��ʾ���յ��˻س�������0~7λ��ʾ���ν��յ������ݳ���

static uint32_t BaudRate = 115200;		// Ĭ�ϲ�����

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
		if ((UART1_RX_STA & 0x8000) == 0)			// ����δ���
		{
			if (UART1_RX_STA & 0x4000)				// ���յ��˻س���
			{
				if (UART1_RX_BYTE == '\n')			// ���ν��ܵ����ǻ��з�
					UART1_RX_STA |= 0x8000;			// ��ǽ��ս���
				else
					UART1_RX_STA = 0;				// ������մ���״̬��Ϊ��ʼ
			}
			else
			{
				if (UART1_RX_BYTE == '\r')			// ���ν��յ����ǻس���
					UART1_RX_STA |= 0x4000;			// ��ǽ��յ��˻س���
				else
				{
					UART1_RX_BUF[UART1_RX_STA & 0x3FFF] = UART1_RX_BYTE;	// ���ν��յ���������
					if (UART1_RX_STA++ >= UART1_RX_LEN)						// �������յ������ݸ��������ճ��ȣ�����Ϊ���մ���
						UART1_RX_STA = 0;									// ״̬��Ϊ��ʼ
				}
			}
		}
		HAL_UART_Receive_IT(&huart1, &UART1_RX_BYTE, 1);					// �ٴ�ʹ�ܽ����ж�
	}
}

/**
 * ����1���մ�����
 */
void UART1_RecvHandler(void)
{
	if (UART1_RX_STA & 0x8000)										// �������յ�
	{
		if(strstr((const char *)UART1_RX_BUF, "AT+:") != NULL)		// ���Ұ�ͷ
		{
			CmdHdr cmdhdr;
			memcpy(&cmdhdr, UART1_RX_BUF, sizeof(CmdHdr));
			//printf("angle:%f    value:%d \r\n", cmdhdr.angle, cmdhdr.value);		// ��ӡ���յ���ֵ
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
