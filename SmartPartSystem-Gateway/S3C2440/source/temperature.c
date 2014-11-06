#include "temperature.h"

/*使用的是GPG0口*/

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
*函数：delay1(U32 i)                                  *
*功能: 延迟函数，当i为1000000时延迟1s,即一次延迟1us   *
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
*函数：ReadTemperature(void) 			               *						
*功能：读取DS18B20温度，返回一个温度值				   *					   
********************************************************/
int ReadTemperature(void)
{
	U8 a=0;
	U8 b=0;
	U16 t=0;
	float tt=0;
		Init_DS18B20();
		WriteOneChar(0xCC); // 跳过读序号列号的操作
		WriteOneChar(0x44); // 启动温度转换
		Init_DS18B20();
		WriteOneChar(0xCC); //跳过读序号列号的操作
		WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
		a=ReadOneChar();
		b=ReadOneChar();		
		t=b;
		t<<=8;
		t=t|a; 
		tt=t*0.0625; //将温度的高位与低位合并
		t= tt*10; //结果温度
		return(t);
}
/*******************************************************
*函数：void num() 			                           *						
*功能：对温度值进行数据分离				               *					   
********************************************************/
U32 get_t(void)
{   U32 p;
	
    p=ReadTemperature(); //读取当前温度
//	shu[0]=p%10;//小数位
 //   shu[1]=(p/10)%10;//个位
 //   shu[2]=(p/100)%10;//十位
    
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


