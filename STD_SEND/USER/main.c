#include "sys.h"
#include "string.h"

#define digitalToggle(p,i) {p->ODR ^=i;}
extern u16 ADV[128];
extern u8 CCD_Zhongzhi;
extern u8 CCD_Yuzhi;

extern u16  TIM2CH1_CAPTURE_STA;		    				
extern u16	TIM2CH1_CAPTURE_VAL;


extern u16  TIM1CH1_CAPTURE_STA;		    				
extern u16	TIM1CH1_CAPTURE_VAL;
char flag;
uint32_t time = 0;
extern vu16 ADC_DMA_IN[5];
vu8 ADC[5];
float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����

typedef struct
{
	char hdr[4];
	float angle;
	int value;
	unsigned char end[2];
}CmdHdr;

void uart1_send_str(const void* buf, unsigned int len)
{
	unsigned char* p = (unsigned char *)buf;
	while(len--)
	{
		while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
        USART1->DR = (u8) * p;  
		//fputc(*p, NULL);
		p++;
	}
}

void UART1_Send(float angle, int value)
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
	uart1_send_str(&cmdhdr, sizeof(CmdHdr));
}

/**
 * ����1���մ�����
 */
void UART1_RecvHandler(void)
{
	if (USART_RX_STA & 0x8000)										// �������յ�
	{
		if(strstr((const char *)USART_RX_BUF, "AT+:") != NULL)		// ���Ұ�ͷ
		{
			CmdHdr cmdhdr;
			memcpy(&cmdhdr, USART_RX_BUF, sizeof(CmdHdr));
			printf("angle:%f    value:%d \r\n", cmdhdr.angle, cmdhdr.value);
		}
		USART_RX_STA = 0;
	}
}

int main(void)
{
	float angle = 0.0;
	int   value = 0;
	
	SystemInit();        //��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	delay_init();	     //��ʱ��ʼ��
	uart_init(115200);
	
	while(1)
	{
		UART1_Send(angle, value);
		delay_ms(1000);
		angle += 0.2;
		delay_ms(1000);
		value += 2;
//		printf("------\r\n");
//		delay_ms(1000);
	}

}
