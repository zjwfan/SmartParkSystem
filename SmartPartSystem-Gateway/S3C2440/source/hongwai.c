#include "2440lib.h"
#include   "config.h" 

#define EINT3close   (rINTMSK|=0x1<<3)    //屏蔽服务
#define EINT3open    (rINTMSK&=~(0x1<<3))    //开启服务

#define num 70

int Eint3_ISR(void);

U8 IRCOM[4];

/******************************************************
*函数：delay1(U32 i)                                  *
*功能: 延迟函数，当i为1000000时延迟1s,即一次延迟1us   *
******************************************************/

void delay(U32 i)
{
  U32 a,b,z;
    z=2;  //40
   for(b=z;b>0;b--)
  	for(a=i;a>0;a--);
}

void Init_Eint3(void)
{
	
	rGPFCON = 0x55aa;   //设置GPF3为外部中断
	rEXTINT1 &=0x0;
	rEXTINT1 = 1<<13; //设置EINT3为下降沿触发
		
    rSRCPND |= 1<<3;
    rINTPND |= 1<<3;

	pISR_EINT3=(int)Eint3_ISR;	
		
 	rGPFDAT=1<<3;//GPF3口初始化为1
		
	EINT3open;
	
    return;
}


int Eint3_ISR(void)
{
	U8 j,k,N=0;
	U8 msg;
    rSRCPND |= 1<<3;
    rINTPND |= 1<<3;
     
    EINT3close;    //关闭外部中断
    delay(50);  //100ms
	while ((rGPFDAT&0x8)>>3)  //如果没有信号输入则开启中断
	{ 
	    EINT3open;
	    return 0;
    } 
                           //确认IR信号出现
    while (!((rGPFDAT&0x8)>>3))          //等IR变为高电平，跳过9ms的前导低电平信号。
      {      
      delay(50);     
      }  //9ms
      
    for (j=0;j<4;j++)         //收集四组数据
    { 
        for (k=0;k<8;k++)        //每组数据有8位
        {
		   while ((rGPFDAT&0x8)>>3)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
		     {
		     delay(num);}  //4.5ms
		     
		   while (!((rGPFDAT&0x8)>>3))          //等 IR 变为高电平
		     {
		     delay(num);} 
		     		      
		   while ((rGPFDAT&0x8)>>3)           //计算IR高电平时长
		   {
		   	 delay(num);   
		     N++;           
		     if(N>=30)
			 {
			   EINT3open;
			   return;
			 }                  //0.14ms计数过长自动离开。
		    }                        //高电平计数完毕        
		    
		            
		   IRCOM[j]=IRCOM[j] >> 1;           //数据最高位补“0”
		   if (N>=8) 
		   {
		   	  IRCOM[j] = IRCOM[j] | 0x80;      //数据最高位补“1”
		   } 
		   N=0;
 		}
	}
   
   if(IRCOM[2]==~IRCOM[3])
   { 
       OSPrintf("IRCOM2=%d\n",IRCOM[2]);
       OSPrintf("IRCOM3=%d\n",IRCOM[3]);

   //    Uart_Printf("Test5\n");
	   EINT3open;
	   return; 
   }
  else
//   return IRCOM[2];
//   Uart_Printf("%d\n",IRCOM[2]);	
   {
   msg=IRCOM[2];
//   OSMboxPost(infraredMbox,&msg);
   }
   EINT3open;
    return;

}

























