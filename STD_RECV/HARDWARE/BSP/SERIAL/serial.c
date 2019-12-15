#include "serial.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "adc.h"

/*
���� aa    
���� ab    ��̨���� ai
ǰ�� ac    ����     aj
���� ad    ����     ak
��ת ae    ��ɨ     ag
��ת af    ��ɨ     ah
*/
extern u16 ADV[128];
extern u8 CCD_Zhongzhi;
extern u8 CCD_Yuzhi;
void ShoHelp_Serial(void)
{
  printf("\n\r              -�����˵�   \r");
	printf("\n\r              -���� aa    \r");
	printf("\n\r              -���� ab    \r");
	printf("\n\r              -ǰ�� ac    \r");
	printf("\n\r              -���� ad    \r");
	printf("\n\r              -��̨���� ai\r");
	printf("\n\r              -����     aj\r");
	printf("\n\r              -����     ak\r");
	printf("\n\r              -��ɨ     ag\r");
	printf("\n\r              -��ɨ     ah\r");
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
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);//�ȴ����ͽ���
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
//			if(times%30==0) LED1_TOGGLE;//��˸LED,��ʾϵͳ��������.
//			delay_ms(10);   
//		}
	}	
}




