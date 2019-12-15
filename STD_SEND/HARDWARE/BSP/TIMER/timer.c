#include "timer.h"
#include "motor.h"
#include "ccd.h"


void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}


u16 ledchange_count;
u16 ADC_count;
float Moter_RVal,Moter_LVal;

u16 test_count;
u16 flag;

extern u16 ADV[128];
u8 CCD_Zhongzhi;
u8 CCD_Yuzhi;

extern u8 count_flag;

void  Find_CCD_Zhongzhi(void);

void TIM3_IRQHandler(void)   //1us    中断一次
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查TIM3更新中断发生与否
	{
	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		
		ledchange_count++;
		ADC_count++;
		if(ADC_count==10000)
		{
		  if(count_flag==0)
				count_flag=1;
		}
			
		if(ADC_count==20000)
		{
		  count_flag=2;
			ADC_count=0;
		}
//		flag=~flag;
//		if(ledchange_count==10000)
//		{
//			ledchange_count=0;
//		  if(flag==0)
//			{
//			  flag=1;
//			}
//			else if(flag==1)
//				flag=3;
//		}
//		if(flag) RD_TSL();
//		  Find_CCD_Zhongzhi();
	}
}


/*  取绝对值函数*/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

void  Find_CCD_Zhongzhi(void)
{ 
	 static u8 i,j,Left,Right,Last_CCD_Zhongzhi;
	 static u16 value1_max,value1_min;
	
	 value1_max=ADV[0];  //动态阈值算法，读取最大和最小值
	 for(i=5;i<123;i++)   //两边各去掉5个点
	 {
			if(value1_max<=ADV[i])
			value1_max=ADV[i];
	 }
	 value1_min=ADV[0];  //最小值
	 for(i=5;i<123;i++) 
	 {
			if(value1_min>=ADV[i])
			value1_min=ADV[i];
	 }
  CCD_Yuzhi=(value1_max+value1_min)/2;	  //计算出本次中线提取的阈值

	for(i = 5;i<118; i++)   //寻找左边跳变沿
  {
		if(ADV[i]>CCD_Yuzhi&&ADV[i+1]>CCD_Yuzhi&&ADV[i+2]>CCD_Yuzhi&&ADV[i+3]<CCD_Yuzhi&&ADV[i+4]<CCD_Yuzhi&&ADV[i+5]<CCD_Yuzhi)
		{	
		  Left=i;
		  break;	
		}
  }
	for(j = 118;j>5; j--)//寻找右边跳变沿
  {
		if(ADV[j]<CCD_Yuzhi&&ADV[j+1]<CCD_Yuzhi&&ADV[j+2]<CCD_Yuzhi&&ADV[j+3]>CCD_Yuzhi&&ADV[j+4]>CCD_Yuzhi&&ADV[j+5]>CCD_Yuzhi)
		{	
		  Right=j;
		  break;	
		}
  }
	CCD_Zhongzhi=(Right+Left)/2;//计算中线位置
//	if(myabs(CCD_Zhongzhi-Last_CCD_Zhongzhi)>70)   //计算中线的偏差，如果太大
//	CCD_Zhongzhi=Last_CCD_Zhongzhi;    //则取上一次的值
//	Last_CCD_Zhongzhi=CCD_Zhongzhi;  //保存上一次的偏差
}



