#include "motor.h"

void TIM4_PWM_Init(u16 arr,u16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PC端口
  GPIO_SetBits(GPIOA, GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);	 // 关闭电机 
 
	
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;//|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); 

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void LIMIT_16(float x,float low,float high)  //x	输入值 
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
	
	/*  方向的判断  */
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

	TIM_SetCompare3(TIM4,LLL);//	1--3右
	TIM_SetCompare4(TIM4,RRR);//   2--4
}

