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
float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据

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
		while((USART1->SR&0X40)==0){}//循环发送,直到发送完毕   
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
 * 串口1接收处理函数
 */
void UART1_RecvHandler(void)
{
	if (USART_RX_STA & 0x8000)										// 有数据收到
	{
		if(strstr((const char *)USART_RX_BUF, "AT+:") != NULL)		// 查找包头
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
	
	SystemInit();        //初始化RCC 设置系统主频为72MHZ
	delay_init();	     //延时初始化
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
