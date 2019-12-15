#include "serial.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "adc.h"

/*
加速 aa    
减速 ab    云台置中 ai
前进 ac    上仰     aj
后退 ad    下仰     ak
左转 ae    左扫     ag
右转 af    右扫     ah
*/
extern u16 ADV[128];
extern u8 CCD_Zhongzhi;
extern u8 CCD_Yuzhi;
void ShoHelp_Serial(void)
{
  printf("\n\r              -帮助菜单   \r");
	printf("\n\r              -加速 aa    \r");
	printf("\n\r              -减速 ab    \r");
	printf("\n\r              -前进 ac    \r");
	printf("\n\r              -后退 ad    \r");
	printf("\n\r              -云台置中 ai\r");
	printf("\n\r              -上仰     aj\r");
	printf("\n\r              -下仰     ak\r");
	printf("\n\r              -左扫     ag\r");
	printf("\n\r              -右扫     ah\r");
}
void Show_CCD(void)   
{
	printf("\n\r yuzhi = %d \n\n\n\n Zhongzhi = %d \r",CCD_Yuzhi,CCD_Zhongzhi);
}

void Serial_Message()
{
  u8 len,t;
  //ShoHelp_Serial();
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);//等待发送结束
				if(USART_RX_BUF[0]=='r')
				{
					LED3_TOGGLE;
					printf(" red \r");
				}
				if(USART_RX_BUF[0]=='g')
				{
					LED3_TOGGLE;
					printf(" green \r");
				}
				if(USART_RX_BUF[0]=='b')
				{
					LED3_TOGGLE;
					printf(" bule \r");
				}
			}
			USART_RX_STA=0;
		}
//		else
//		{
//			times++;
//			if(times%30==0) LED1_TOGGLE;//闪烁LED,提示系统正在运行.
//			delay_ms(10);   
//		}
	}	
}




