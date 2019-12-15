#include "openmv.h"

void Openmv_Receive_Data(u16 data)
{
  static u8 openmv[18];
	static u8 state = 0;
	static u8 bit_number = 0;
	
	u8 i;
	if(state==0 && data == 0x2c)
	{
	  state =1;
		openmv[bit_number++]=data;
	}
	else if(state==1 && data == 0x18)
	{
	  state =2;
		openmv[bit_number++]=data;
	}
	else if(state==2)
	{
		openmv[bit_number++]=data;
		if(bit_number >= 17)
			state = 3;
	}
	else if(state==3)
	{
		if(data >= 58)
		{
		  state = 0;
			for(i=0;i<18;i++)
			{
			  openmv[i]=0x00;
			}
		}
	}
	else
	{
	  state = 0;
		bit_number=0;
		for(i=0;i<18;i++)
		{
			openmv[i]=0x00;
		}
	}
}

