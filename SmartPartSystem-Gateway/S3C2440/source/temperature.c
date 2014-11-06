#include "temperature.h"

/*ʹ�õ���GPG0��*/

#define DQset1     (rGPGDAT|=1)
#define DQset0     (rGPGDAT&=~1)
#define DQ         (rGPGDAT&0x1)
#define DQ_INPUT   (rGPGCON&=~3)
//#define DQ_OUTPUT  (rGPGCON&=~3)  
/*
rGPGCON&=~3;
rGPGCON|=1;
*/
//U8 shu[3];
/******************************************************
*������delay1(U32 i)                                  *
*����: �ӳٺ�������iΪ1000000ʱ�ӳ�1s,��һ���ӳ�1us   *
******************************************************/
void delay1(U32 i)
{
 // U32 a,b,z;
//    z=40;
 //   for(b=z;b>0;b--)
    U32 a;
  	for(a=i;a>0;a--);
}


 void Init_DS18B20(void)
 {
 	do
 	{
 		//DQ_OUTPUT;
 		rGPGCON&=~3;
        rGPGCON|=1;

 		DQset1;
 		delay1(1);
 		DQset0;
 		delay1(600);
 		DQset1;
 		delay1(60);
 	}while(DQ==0);
 }
 

 int ReadOneChar(void)
 {
 	U8 i,temp=0;
 	for(i=0;i<8;i++)
 	{
 		temp>>=1;
 	//	DQ_OUTPUT;
 		rGPGCON&=~3;
        rGPGCON|=1;

 		DQset0;
 		delay1(1);
 		DQset1;
 		DQ_INPUT;
 		delay1(10);
 		if(DQ)
 			temp=(temp|0x80);
 		delay1(65);
 	//	DQ_OUTPUT;
 	    rGPGCON&=~3;
        rGPGCON|=1;

 		DQset1; 	
 	}
 	return(temp);
 }


void WriteOneChar(U8 dat)
{
	U8 i;
//	DQ_OUTPUT;
    rGPGCON&=~3;
    rGPGCON|=1;

	for(i=0;i<8;i++)
	{
		DQset0;
		delay1(1);
		if(dat&0x01)
		DQset1;
		delay1(65);
		DQset1;
		dat=dat>>1;
	}
}
/*******************************************************
*������ReadTemperature(void) 			               *						
*���ܣ���ȡDS18B20�¶ȣ�����һ���¶�ֵ				   *					   
********************************************************/
int ReadTemperature(void)
{
	U8 a=0;
	U8 b=0;
	U16 t=0;
	float tt=0;
		Init_DS18B20();
		WriteOneChar(0xCC); // ����������кŵĲ���
		WriteOneChar(0x44); // �����¶�ת��
		Init_DS18B20();
		WriteOneChar(0xCC); //����������кŵĲ���
		WriteOneChar(0xBE); //��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
		a=ReadOneChar();
		b=ReadOneChar();		
		t=b;
		t<<=8;
		t=t|a; 
		tt=t*0.0625; //���¶ȵĸ�λ���λ�ϲ�
		t= tt*10; //����¶�
		return(t);
}
/*******************************************************
*������void num() 			                           *						
*���ܣ����¶�ֵ�������ݷ���				               *					   
********************************************************/
U32 get_t(void)
{   U32 p;
	
    p=ReadTemperature(); //��ȡ��ǰ�¶�
//	shu[0]=p%10;//С��λ
 //   shu[1]=(p/10)%10;//��λ
 //   shu[2]=(p/100)%10;//ʮλ
    
//    w=shu[2]*100+shu[1]*10+shu[0]*1;
    return p;
    
    	    
	
	
}
/*
void Main()
{
 while(1)
  {
    num();
	Uart_Printf("\n%d%d.%d\n",shu[2],shu[1],shu[0]);
  }
}*/
/*void DS18B20_Task()
{
    get_t();
	Uart_Printf("\n%d%d.%d\n",shu[2],shu[1],shu[0]);
}*/


