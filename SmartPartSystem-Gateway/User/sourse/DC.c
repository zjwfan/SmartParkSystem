//====================================================================
// File Name : motor.c
// Function  : S3C2410  ֱ�����
// Program   : 
// Date      : 2013��7.07
// Version   : 0.0
// Writer    : �̷�
// History   :
//====================================================================
#include "2440addr.h"

//�������ͬʱת��
/*
#define MOTOR_A  (rGPFDAT|=0x14);//�ڶ����Ŀ���1
#define MOTOR_B  (rGPFDAT|=0x28);//��һ��������1
#define MOTOR_C  (rGPFDAT|=0x24);//�ڶ��������1
#define MOTOR_D  (rGPFDAT|=0x18);//�������Ŀ���1

//#define LEN		 50			//1  //47

#define MOTOR_SAVE rGPFDAT = 0x00;
extern MODEL;
//==================================================================================
void delay11(int x)
{int i,j;
 for(i=0;i<x;i++)
 for(j=0;j<x;j++)
 {
 }


}
//==================================================================================
//�������m ֱ�������ת
//���ܣ�
//������S32 q Ȧ��
//����ֵ����
//==================================================================================

void motor_forward_1 (int q)
{
 S32 i;
// S32 MOTOR_SAVE=rGPFCON;
 
 rGPFCON&=0x00;
 rGPFCON|=0x550;
 rGPFDAT&=0x00;
 
  if(q<0)
    q=0;
    
 for(i=0;i<64*q;i++)
  {
  
	 MOTOR_A;
	 delay11(LEN);
	 rGPFDAT&=0x00;
     
  }
 // MOTOR_0;
   rGPFDAT=MOTOR_SAVE;
}

//==================================================================================
//��������motor_back_1
//���ܣ�
//������
//����ֵ����
//==================================================================================
void motor_back_1 (int q)
{
 S32 i;
 //S32 MOTOR_SAVE=rGPFCON;

 rGPFCON&=0x00;
 rGPFCON|=0x550;
 rGPFDAT&=0x00;
 
 if(q<0)
    q=0;
    

 for(i=0;i<64*q;i++)
  {

     MOTOR_B;
     delay11(LEN);
     rGPFDAT&=0x00;
     
  }

  rGPFDAT=MOTOR_SAVE;
}
*/
//==================================================================================
//��������motor_turnleft_1
//���ܣ�
//������
//����ֵ����
//==================================================================================
/*
void motor_turnleft_1(int q)
{
	S32 i;
	rGPFCON&=0x00;
    rGPFCON|=0x550;
    rGPFDAT&=0x00;
    
    if(q<0)
    	q=0;
    	
  for(i=0;i<64*q;i++)
  {
     MOTOR_C;
     delay11(LEN);
     rGPFDAT&=0x00;
     
  
  }
  rGPFDAT=MOTOR_SAVE;
    
}

//==================================================================================
//��������motor_turnright_1
//���ܣ�
//������
//����ֵ����
//==================================================================================

void motor_turnright_1(int q)
{
	S32 i;
	rGPFCON&=0x00;
    rGPFCON|=0x550;
    rGPFDAT&=0x00;
    
    if(q<0)
    	q=0;
    	
  for(i=0;i<64*q;i++)
  {
     MOTOR_D;
     delay11(LEN);
     rGPFDAT&=0x00;
     
  
  }
  rGPFDAT=MOTOR_SAVE;
    
}

*/