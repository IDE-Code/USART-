#include "timer.h"
#include "motor.h"
#include "ccd.h"


void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
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

void TIM3_IRQHandler(void)   //1us    �ж�һ��
{
  if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���TIM3�����жϷ������
	{
	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
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


/*  ȡ����ֵ����*/
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
	
	 value1_max=ADV[0];  //��̬��ֵ�㷨����ȡ������Сֵ
	 for(i=5;i<123;i++)   //���߸�ȥ��5����
	 {
			if(value1_max<=ADV[i])
			value1_max=ADV[i];
	 }
	 value1_min=ADV[0];  //��Сֵ
	 for(i=5;i<123;i++) 
	 {
			if(value1_min>=ADV[i])
			value1_min=ADV[i];
	 }
  CCD_Yuzhi=(value1_max+value1_min)/2;	  //���������������ȡ����ֵ

	for(i = 5;i<118; i++)   //Ѱ�����������
  {
		if(ADV[i]>CCD_Yuzhi&&ADV[i+1]>CCD_Yuzhi&&ADV[i+2]>CCD_Yuzhi&&ADV[i+3]<CCD_Yuzhi&&ADV[i+4]<CCD_Yuzhi&&ADV[i+5]<CCD_Yuzhi)
		{	
		  Left=i;
		  break;	
		}
  }
	for(j = 118;j>5; j--)//Ѱ���ұ�������
  {
		if(ADV[j]<CCD_Yuzhi&&ADV[j+1]<CCD_Yuzhi&&ADV[j+2]<CCD_Yuzhi&&ADV[j+3]>CCD_Yuzhi&&ADV[j+4]>CCD_Yuzhi&&ADV[j+5]>CCD_Yuzhi)
		{	
		  Right=j;
		  break;	
		}
  }
	CCD_Zhongzhi=(Right+Left)/2;//��������λ��
//	if(myabs(CCD_Zhongzhi-Last_CCD_Zhongzhi)>70)   //�������ߵ�ƫ����̫��
//	CCD_Zhongzhi=Last_CCD_Zhongzhi;    //��ȡ��һ�ε�ֵ
//	Last_CCD_Zhongzhi=CCD_Zhongzhi;  //������һ�ε�ƫ��
}



