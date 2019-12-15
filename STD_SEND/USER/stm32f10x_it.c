/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "sys.h"
extern volatile uint32_t time;
extern float pitch,roll,yaw; 		//欧拉角
extern short aacx,aacy,aacz;		//加速度传感器原始数据
extern short gyrox,gyroy,gyroz;	//陀螺仪原始数据
short temp;					//温度	
unsigned int Task_Delay[NumOfTask]={0};
extern char flag;
extern vu8 ADC[5];
extern int array[2];
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态

void KEY1_IRQHandler(void)
{
	
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		
			if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
			{
				temp=MPU_Get_Temperature();	//得到温度值
				MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
				MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
				delay_ms(10);
			}
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		//digitalToggle(GPIOC,GPIO_Pin_13)
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}


void TIM2_IRQHandler(void)
{
	uint16_t angle;
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		if(ADC[0]==1&&ADC[4]==1){flag=5;}
		if(ADC[0]==0&&ADC[1]==0&&ADC[2]==1&&ADC[3]==0&&ADC[4]==0){flag=8;}
		if(ADC[0]==0&&ADC[1]==1&&ADC[2]==0&&ADC[3]==1&&ADC[4]==0){flag=8;}
		if(ADC[0]==0&&ADC[1]==1&&ADC[2]==0&&ADC[3]==0&&ADC[4]==0){flag=7;}
		if(ADC[0]==0&&ADC[1]==1&&ADC[2]==1&&ADC[3]==0&&ADC[4]==0){flag=7;}
		if(ADC[0]==0&&ADC[1]==0&&ADC[2]==0&&ADC[3]==1&&ADC[4]==0){flag=9;}
		if(ADC[0]==0&&ADC[1]==0&&ADC[2]==1&&ADC[3]==1&&ADC[4]==0){flag=9;}
		angle=yaw+180;
		Usart_SendHalfWord( USART1, angle);
		//USART_SendData(USART1,'3');
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);    
  		digitalToggle(GPIOC,GPIO_Pin_13)
		
	}		 	
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
