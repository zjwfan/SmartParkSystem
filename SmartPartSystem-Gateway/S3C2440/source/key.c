/****************************************Copyright (c)**************************************************
**                               Iky Embeddey System Institute
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:          Key.c
** Last modified Date: 2011-06-06 
** Last Version:       v1.0
** Descriptions:       ������ʼ�����жϷ������
** 1X6 �������
** �����������ţ�	EINT8 -----( GPG0  )
** 					EINT11 -----( GPG3  )
**					EINT13-----( GPG5  )
**					EINT14-----( GPG6 )
**					EINT15-----( GPG7 )
**					EINT19-----( GPG11 )
**------------------------------------------------------------------------------------------------------
** Modified by:        xiexy
** Modified date:      2011-06-06
** Version:            v1.0
** Descriptions:       ����
**
**------------------------------------------------------------------------------------------------------
** Modified by:     
** Modified date:     
** Version:           
** Descriptions:      
**
********************************************************************************************************/
#include "config.h" 

extern OS_EVENT *InfoQ;						/* ����ָ����Ϣ���е�ָ�� 					*/

/*****************************************************************************
** Function name: Key_ISR
** Descriptions:  �����жϷ�����������ɨ����̣�ȷ�ϵ�ǰ���ĸ��������ˣ���������ֵ
**				  ����Ϣ���䷢�ͳ�ȥ
** Input:         �� 
** Output:        ��
** Created by:    xiexy
** Created Date:  2011-06-06
**----------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**----------------------------------------------------------------------------------
******************************************************************************/
void Key_ISR(void)
{
	#if OS_CRITICAL_METHOD == 3             /* Allocate storage for CPU status register */
    OS_CPU_SR     cpu_sr = 0;
	#endif
	static INT16U Key[7];					/* ���������ڱ��水���ı�� 				*/

	OS_ENTER_CRITICAL();					/* �ر��жϣ��Է�ֹɨ�����ʱ�������жϴ�� */
	
	DelayNS( 80 ) ;							/* ��ʱ�Ա�ȥ���� 							*/	

	if(      (rGPGDAT&(1<< 0)) == 0 )		/* ������key1�� 							*/
	{	
		Key[0] = 1;
		OSQPost(InfoQ, (void *)&Key[0]);	/* ����Ϣ���з��Ͱ���ֵ						*/
	}
	else if( (rGPGDAT&(1<< 3)) == 0 )		/* ������key2�� 							*/
	{
		Key[1] = 2;
		OSQPost(InfoQ, (void *)&Key[1]);
	}
	else if( (rGPGDAT&(1<< 5)) == 0 )		/* ������key3�� 							*/
	{
		Key[2] = 3;
		OSQPost(InfoQ, (void *)&Key[2]);
	}
	else if( (rGPGDAT&(1<< 6)) == 0 )		/* ������key4�� 							*/
	{
		Key[3] = 4;
		OSQPost(InfoQ, (void *)&Key[3]);
	}
	else if( (rGPGDAT&(1<< 7)) == 0 )		/* ������key5�� 							*/
	{
		Key[4] = 5;
		OSQPost(InfoQ, (void *)&Key[4]);
	}
	else if( (rGPGDAT&(1<<11)) == 0 )		/* ������key6�� 							*/
	{
		Key[5] = 6;
		OSQPost(InfoQ, (void *)&Key[5]);
	}
	else
	{
		Key[6] = 0xff;
	}
	
	if(rINTPND==BIT_EINT8_23) 				/* ����жϱ�־λ							*/
	{
		ClearPending(BIT_EINT8_23);
		if(rEINTPEND&(1<<8)) 
		{
		//Uart_Printf("eint11\n");
			rEINTPEND |= 1<< 8;
		}
		if(rEINTPEND&(1<<11))
		{
		//Uart_Printf("eint11\n");
			rEINTPEND |= 1<< 11;
		}
		if(rEINTPEND&(1<<13)) 
		{
		//Uart_Printf("eint11\n");
			rEINTPEND |= 1<< 13;
		}
		if(rEINTPEND&(1<<14)) 
		{
		//Uart_Printf("eint11\n");
			rEINTPEND |= 1<< 14;
		}
		if(rEINTPEND&(1<<15)) 
		{
		//Uart_Printf("eint11\n");
			rEINTPEND |= 1<< 15;
		}
		if(rEINTPEND&(1<<19)) 
		{
		//	Uart_Printf("eint19\n");		
			rEINTPEND |= 1<< 19;
		}
	}
	
	OS_EXIT_CRITICAL();
}

/*****************************************************************************
** Function name: Key_Init
** Descriptions:  ������ʼ������
**				  
** Input:         �� 
** Output:        ��
** Created by:    xiexy
** Created Date:  2011-06-06
**----------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**----------------------------------------------------------------------------------
******************************************************************************/
void Key_Init(void)
{
	
	rGPGCON = rGPGCON & (~((3<<22)|(3<<6)|(3<<0)|(3<<10)|(3<<12)|(3<<14))) |
						 ((2<<22)|(2<<6)|(2<<0)|(2<<10)|(2<<12)|(2<<14)) ;		//GPG11,3 set EINT
	
	rEXTINT1 &= ~(7|(7<<0));	
	rEXTINT1 |= (0|(0<<0));	//set eint8 falling edge int
	
	rEXTINT1 &= ~(7<<12);
	rEXTINT1 |= (0<<12);	//set eint11 falling edge int
	
	rEXTINT1 &= ~(7<<20);
	rEXTINT1 |= (0<<20);	//set eint13 falling edge int
	
	rEXTINT1 &= ~(7<<24);
	rEXTINT1 |= (0<<24);	//set eint14 falling edge int
	
	rEXTINT1 &= ~(7<<28);
	rEXTINT1 |= (0<<28);	//set eint15 falling edge int
		
	rEXTINT2 &= ~(0xf<<12);
	rEXTINT2 |= (0<<12);	//set eint19 falling edge int
	

	rEINTPEND |= (1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19);		//clear eint 11,19
	rEINTMASK &= ~((1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19));		//enable eint11,19
	ClearPending(BIT_EINT0|BIT_EINT2|BIT_EINT8_23);
	
	pISR_EINT0 = pISR_EINT2 = pISR_EINT8_23 = (U32)Key_ISR;
	
	rINTMSK &= ~(BIT_EINT0|BIT_EINT2|BIT_EINT8_23);
}
