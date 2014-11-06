//====================================================================
// File Name : motor.c
// Function  : S3C2410  步进电机 
// Program   : 
// Date      : 2012，7. 01
// Version   : 0.0
// Writer    : lady胖胖
// History   :
//====================================================================
#include "2440addr.h"
//#include "window.h"
//#include "Def1.h"


#define MOTOR_A  (rGPFDAT|=0x01);
#define MOTOR_B  (rGPFDAT|=0x02);
#define MOTOR_C  (rGPFDAT|=0x04);
#define MOTOR_D  (rGPFDAT|=0x08);
#define MOTOR_0  (rGPFDAT&=0x00);
#define MOTOR    (rGPFCON&=0x55);
#define LEN		 170			//1  //47

extern MODEL;
//==================================================================================
void delay111(int x)
{int i,j;
 for(i=0;i<x;i++)
 for(j=0;j<x;j++)
 {
 }


}
//==================================================================================
//函数名otor_forward
//功能：电机正向转动一圈，即360°，电机每一个相转动5.625°,需要64个脉冲;
//参数：S32 q 圈数
//返回值：无
//==================================================================================

void motor_forward (int q)
{
 S32 i;
// S32 MOTOR_SAVE=rGPFCON;
 
 rGPFCON&=0x00;
 rGPFCON|=0x55;
 rGPFDAT&=0x00;
 
  if(q<0)
    q=0;
    
 for(i=0;i<64*q;i++)
  {
  
	 rGPFDAT |= 0x01;
	 delay111(LEN);
	 rGPFDAT&=0x00;
     //rGPFDAT |= 0x03;
     //delay11(LEN);
     //rGPFDAT&=0x00;
     rGPFDAT |= 0x02;
	 delay111(LEN);
     rGPFDAT&=0x00;
     //rGPFDAT |= 0x06;
     //delay11(LEN);
     //rGPFDAT&=0x00;
     rGPFDAT |= 0x04;
     delay111(LEN);
     rGPFDAT&=0x00;
     //rGPFDAT |= 0x0c;
     //delay11(LEN);
     //rGPFDAT&=0x00;
     rGPFDAT |= 0x08;
     delay111(LEN);
     rGPFDAT&=0x00;
     //rGPFDAT |= 0x09;
     //delay11(LEN);
     //rGPFDAT&=0x00;
     	 rGPFDAT |= 0x01;
	 delay111(LEN);
	 rGPFDAT&=0x00;
     //rGPFDAT |= 0x03;
     //delay11(LEN);
     //rGPFDAT&=0x00;
     rGPFDAT |= 0x02;
	 delay111(LEN);
     rGPFDAT&=0x00;
     //rGPFDAT |= 0x06;
     //delay11(LEN);
     //rGPFDAT&=0x00;
     rGPFDAT |= 0x04;
     delay111(LEN);
     rGPFDAT&=0x00;
     //rGPFDAT |= 0x0c;
     //delay11(LEN);
     //rGPFDAT&=0x00;
     rGPFDAT |= 0x08;
     delay111(LEN);
     rGPFDAT&=0x00;
     //rGPFDAT |= 0x09;
     //delay11(LEN);
     //rGPFDAT&=0x00;
     
   
  }
 // MOTOR_0;
 // rGPFCON=MOTOR_SAVE;
}

//==================================================================================
//函数名：motor_back
//功能：电机反向转动一圈360°，电机每一个相转动5.625°,需要64个脉冲;
//参数：S32 q 圈数
//返回值：无
//==================================================================================
void motor_back (int q)
{
 S32 i;
// S32 MOTOR_SAVE=rGPFCON;

 rGPFCON&=0x00;
 rGPFCON|=0x55;
 rGPFDAT&=0x00;
 
 if(q<0)
    q=0;
    

 for(i=0;i<64*q;i++)
  {

     rGPFDAT |= 0x09;
     delay111(LEN);
     rGPFDAT&=0x00;
     rGPFDAT |= 0x08;
     delay111(LEN);
     rGPFDAT&=0x00;
     rGPFDAT |= 0x0c;
     delay111(LEN);
     rGPFDAT&=0x00;
     rGPFDAT |= 0x04;
     delay111(LEN);
     rGPFDAT&=0x00;
     rGPFDAT |= 0x06;
     delay111(LEN);
     rGPFDAT&=0x00;
     rGPFDAT |= 0x02;
     delay111(LEN);
     rGPFDAT&=0x00;
	 rGPFDAT |= 0x03;
     delay111(LEN);
     rGPFDAT&=0x00;
     rGPFDAT |= 0x01;
     delay111(LEN);
     rGPFDAT&=0x00;
     


   

  }
   MOTOR_0;
 // rGPFCON=MOTOR_SAVE;
}


