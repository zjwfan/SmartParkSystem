/****************************************Copyright (c)**************************************************
**                               Iky Embeddey System Institute
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:          Key.c
** Last modified Date: 2011-06-06 
** Last Version:       v1.0
** Descriptions:       按键初始化及中断服务程序
** 1X6 矩阵键盘
** 六个输入引脚：	EINT8 -----( GPG0  )
** 					EINT11 -----( GPG3  )
**					EINT13-----( GPG5  )
**					EINT14-----( GPG6 )
**					EINT15-----( GPG7 )
**					EINT19-----( GPG11 )
**------------------------------------------------------------------------------------------------------
** Modified by:        xiexy
** Modified date:      2011-06-06
** Version:            v1.0
** Descriptions:       创建
**
**------------------------------------------------------------------------------------------------------
** Modified by:     
** Modified date:     
** Version:           
** Descriptions:      
**
********************************************************************************************************/
#include "config.h" 

extern OS_EVENT *InfoQ;						/* 声明指向消息队列的指针 					*/

/*****************************************************************************
** Function name: Key_ISR
** Descriptions:  按键中断服务函数，函数扫描键盘，确认当前有哪个键按下了，并将按键值
**				  经消息邮箱发送出去
** Input:         无 
** Output:        无
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
	static INT16U Key[7];					/* 该数组用于保存按键的编号 				*/

	OS_ENTER_CRITICAL();					/* 关闭中断，以防止扫描键盘时被其他中断打断 */
	
	DelayNS( 80 ) ;							/* 延时以便去抖动 							*/	

	if(      (rGPGDAT&(1<< 0)) == 0 )		/* 按下了key1键 							*/
	{	
		Key[0] = 1;
		OSQPost(InfoQ, (void *)&Key[0]);	/* 向消息队列发送按键值						*/
	}
	else if( (rGPGDAT&(1<< 3)) == 0 )		/* 按下了key2键 							*/
	{
		Key[1] = 2;
		OSQPost(InfoQ, (void *)&Key[1]);
	}
	else if( (rGPGDAT&(1<< 5)) == 0 )		/* 按下了key3键 							*/
	{
		Key[2] = 3;
		OSQPost(InfoQ, (void *)&Key[2]);
	}
	else if( (rGPGDAT&(1<< 6)) == 0 )		/* 按下了key4键 							*/
	{
		Key[3] = 4;
		OSQPost(InfoQ, (void *)&Key[3]);
	}
	else if( (rGPGDAT&(1<< 7)) == 0 )		/* 按下了key5键 							*/
	{
		Key[4] = 5;
		OSQPost(InfoQ, (void *)&Key[4]);
	}
	else if( (rGPGDAT&(1<<11)) == 0 )		/* 按下了key6键 							*/
	{
		Key[5] = 6;
		OSQPost(InfoQ, (void *)&Key[5]);
	}
	else
	{
		Key[6] = 0xff;
	}
	
	if(rINTPND==BIT_EINT8_23) 				/* 清除中断标志位							*/
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
** Descriptions:  按键初始化函数
**				  
** Input:         无 
** Output:        无
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
