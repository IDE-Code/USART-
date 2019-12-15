#include "motor.h"

void TIM4_PWM_Init(u16 arr,u16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PC�˿�
  GPIO_SetBits(GPIOA, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	 // �رյ�� 
 
	
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;//|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); 

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void LIMIT_16(float x,float low,float high)  //x	����ֵ 
{
	if(low>high)x=0;
	if(x<low)x=low;
	else if(x>high)x=high;
}

extern float Moter_RVal,Moter_LVal;
float RRR,LLL;

void PWM_Control(void)
{
  LIMIT_16(Moter_LVal,-7000,7000);   //zuo
	LIMIT_16(Moter_RVal,-7000,7000);     //you
	RRR=Moter_RVal;
	LLL=Moter_LVal;
	
	/*  ������ж�  */
	if(Moter_RVal<0)  
	{
		RRR=-RRR;
	  //RRR=-RRR/7;
		GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_6);
		GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);
	}
	if(Moter_LVal<0)
	{
		LLL=-LLL;
		//LLL = -LLL/7;
		GPIO_ResetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_7);
		GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_6);
	}
	if(Moter_RVal>0 && Moter_LVal>0)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_7);
		GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6);
	}

	TIM_SetCompare3(TIM4,LLL);//	1--3��
	TIM_SetCompare4(TIM4,RRR);//   2--4
}

