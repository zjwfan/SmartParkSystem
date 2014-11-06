#include "2440lib.h"
#include   "config.h" 

#define EINT3close   (rINTMSK|=0x1<<3)    //���η���
#define EINT3open    (rINTMSK&=~(0x1<<3))    //��������

#define num 70

int Eint3_ISR(void);

U8 IRCOM[4];

/******************************************************
*������delay1(U32 i)                                  *
*����: �ӳٺ�������iΪ1000000ʱ�ӳ�1s,��һ���ӳ�1us   *
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
	
	rGPFCON = 0x55aa;   //����GPF3Ϊ�ⲿ�ж�
	rEXTINT1 &=0x0;
	rEXTINT1 = 1<<13; //����EINT3Ϊ�½��ش���
		
    rSRCPND |= 1<<3;
    rINTPND |= 1<<3;

	pISR_EINT3=(int)Eint3_ISR;	
		
 	rGPFDAT=1<<3;//GPF3�ڳ�ʼ��Ϊ1
		
	EINT3open;
	
    return;
}


int Eint3_ISR(void)
{
	U8 j,k,N=0;
	U8 msg;
    rSRCPND |= 1<<3;
    rINTPND |= 1<<3;
     
    EINT3close;    //�ر��ⲿ�ж�
    delay(50);  //100ms
	while ((rGPFDAT&0x8)>>3)  //���û���ź����������ж�
	{ 
	    EINT3open;
	    return 0;
    } 
                           //ȷ��IR�źų���
    while (!((rGPFDAT&0x8)>>3))          //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�
      {      
      delay(50);     
      }  //9ms
      
    for (j=0;j<4;j++)         //�ռ���������
    { 
        for (k=0;k<8;k++)        //ÿ��������8λ
        {
		   while ((rGPFDAT&0x8)>>3)            //�� IR ��Ϊ�͵�ƽ������4.5ms��ǰ���ߵ�ƽ�źš�
		     {
		     delay(num);}  //4.5ms
		     
		   while (!((rGPFDAT&0x8)>>3))          //�� IR ��Ϊ�ߵ�ƽ
		     {
		     delay(num);} 
		     		      
		   while ((rGPFDAT&0x8)>>3)           //����IR�ߵ�ƽʱ��
		   {
		   	 delay(num);   
		     N++;           
		     if(N>=30)
			 {
			   EINT3open;
			   return;
			 }                  //0.14ms���������Զ��뿪��
		    }                        //�ߵ�ƽ�������        
		    
		            
		   IRCOM[j]=IRCOM[j] >> 1;           //�������λ����0��
		   if (N>=8) 
		   {
		   	  IRCOM[j] = IRCOM[j] | 0x80;      //�������λ����1��
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

























