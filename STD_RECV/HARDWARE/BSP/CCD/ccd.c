#include "ccd.h"
#include "adc.h"
#include "usart.h"
#include "delay.h"


#define TSL_SI    PBout(0)   //SI  
#define TSL_CLK   PBout(1)   //CLK 


u16 ADV[128]={0};

u8 SciBuf[200];

//void Test_CCD()
//{
//	u16 k=0;
//	u8 now_time=delay_ms(k);
//	
//  while(1)
//	{
//		k++;
//	  if(delay_ms(k)-now_time >= 50)
//		{
//		  now_time=delay_ms(k);
//		}
//	}
//}

/*CCD_INIT*/
void CCD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     //2M
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	Adc_Init();
}

/* CCD 采集*/
void RD_TSL(void)
{
	u8 i;
	
	TSL_SI=1;
	delay_us(1);
	TSL_CLK=1;
	delay_us(1);
	TSL_SI=0;
	delay_us(1);
	for(i=0;i<128;i++)
	{
	  delay_us(1);
	  TSL_CLK=1;
		delay_us(1);
		ADV[i]=Get_Adc(ADC_Channel_0);
		TSL_CLK=0;
	}
	delay_us(1);
	TSL_CLK=1;
	delay_us(1);
	TSL_CLK=0;
}
///*
// * FUNCTION NAME: void sendToPc(void) *
//* CREATE DATE : 20170707 *
//* CREATED BY : XJU *
//* FUNCTION :  将从CCD中读取的数据按照上位机要求的通信协议存入
//* MODIFY DATE : NONE *
//* INPUT : void *
//* OUTPUT : NONE *
//* RETURN : NONE *
//*/
//void slove_data(void)
//{
//  int i,j;
//	RD_TSL();
//	SciBuf[0]=0;
//	SciBuf[1]=128+4;
//	SciBuf[2]=0;
//	SciBuf[3]=0;
//	SciBuf[4]=0;
//	SciBuf[5]=0;
//	for(i=0;i<128;i++)
//		SciBuf[6+i]=ADV[i];
//}
///*
//* FUNCTION NAME: void sendToPc(void) *
//* CREATE DATE : 20170707 *
//* CREATED BY : XJU *
//* FUNCTION : 将 待发送的信息通过串口发送至上位机
//* MODIFY DATE : NONE *
//* INPUT : void *
//* OUTPUT : NONE *
//* RETURN : NONE *
//*/
//char binToHex_low(u8 num);
//char binToHex_high(u8 num);

//void sendToPc(void)
//{
//  int i;
//	slove_data();
//	printf("*");
//	printf("LD");
//	for(i=2;i<123;i++)
//	{
//	  printf("%c",binToHex_high(SciBuf[i])); //????????4????16??
//		printf("%c",binToHex_low(SciBuf[i]));  //?????????????16??
//	}
//	printf("00");
//	printf("#");
//	printf("\n\r \r");
//}

//char binToHex_low(u8 num)
//{
//  u8 low_four;
//	low_four=num&0x0f;
//	 if(low_four==0)
//		 return('0');
//	 else if(low_four==1)
//		  return('1');
//	 else if(low_four==2)
//		  return('2');
//	 else if(low_four==3)
//		  return('3');
//	 else if(low_four==4)
//		  return('4');
//	 else if(low_four==5)
//		  return('5');
//	 else if(low_four==6)
//		  return('6');
//	 else if(low_four==7)
//		  return('7');
//	 else if(low_four==8)
//		  return('8');
//	 else if(low_four==9)
//		  return('9');
//	 else if(low_four==10)
//		  return('A');
//	 else if(low_four==11)
//		  return('B');
//	 else if(low_four==12)
//		  return('C');
//	 else if(low_four==13)
//		  return('D');
//	 else if(low_four==14)
//		  return('E');
//	 else if(low_four==15)
//		  return('F');
//}

//char binToHex_high(u8 num)
//{
//  u8 high_four;
//	high_four=(num>>4)&0x0f;
//	if(high_four==0)
//			return('0');
//				else if(high_four==1)
//					return('1');
//					else if(high_four==2)
//							return('2');
//							else if(high_four==3)
//								return('3');
//								else if(high_four==4)
//								return('4');
//									else if(high_four==5)
//									return('5');
//										else if(high_four==6)
//											return('6');
//											else if(high_four==7)
//											return('7');
//												else if(high_four==8)
//												return('8');
//													else if(high_four==9)
//														return('9');
//														else if(high_four==10)
//															return('A');
//															else if(high_four==11)
//																return('B');
//																else if(high_four==12)
//																	return('C');
//																	else if(high_four==13)
//																		return('D');
//																		else if(high_four==14)
//																			return('E');
//																			else if(high_four==15)
//																				return('F');
//}

