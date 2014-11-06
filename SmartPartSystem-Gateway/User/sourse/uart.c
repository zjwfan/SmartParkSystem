#include "config.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 
#include <stdarg.h>

void uart1_sendbyte(unsigned char data);
extern char UartRFIDRecvData[64];			//接收缓冲区
extern OS_EVENT *UartRFIDSem;
int RecLen=0;

/* 下面的函数为串口接收中断处理程序，仅当接收到一个字符后，进入一次
    本代码没有提供串口发送中断处理程序，需要串口发送时，可以先关闭中断，
    发送完成，再打开中断 */
static void Uart1_ISR()
{
	U32 r;
	#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    	OS_CPU_SR     cpu_sr = 0;
	#endif
	
	OS_ENTER_CRITICAL();	//关中断，其实这个是通过 汇编清CORE里面程序状态寄存器中的I，F位什么的，这是最终的


	if(rUTRSTAT1 & 0x1) // 接收中断
	    {
			uart1_sendbyte(RdURXH1());	//将接收到的字符返回给对方
			if(RdURXH1() != '\r')		//如果不是接收到回车，则保持接收到的字符
			{
				UartRFIDRecvData[RecLen] = RdURXH1();
				RecLen++;
			}
			else
			{
				OSSemPost(UartRFIDSem);				//收到回车之后，将标志位over置为1，表示接收完成
				RecLen = 0;
			}
			rSUBSRCPND = BIT_SUB_RXD1;
		}
	    ClearPending(BIT_UART1);
	
	
	OS_EXIT_CRITICAL();	//开中断
}
void Uart_INT()
{
	rINTMOD &= 	~BIT_UART1;					//set as IRQ
	rSUBSRCPND = BIT_SUB_RXD1;			 	//清子中断挂起寄存器
	ClearPending(BIT_UART1);				// 清中断挂起寄存器
	
	pISR_UART1 = (U32)Uart1_ISR;
	rINTSUBMSK &= ~(BIT_SUB_RXD1);			//使能串口子接收中断
	rINTMSK &= ~(BIT_UART1); 				//使能串口总中断
}

/*------------------------------------------------------------/
函数名称：	uart0_init
功能描述：	uart0 初始化函数
返 回 值：	无
-------------------------------------------------------------*/
void uart0_init(int pclk, int baud)
{	
    // GPH0，GPH1，GPH2，GPH3: nCTS0，nRTS0，TXD0，RXD0,  bit7~bit0 10 uart func
	rGPHCON  |= (0x2) + (0x2 << 2) + (0x2 << 4) + (0x2 << 6);    
    rGPHUP   = 0x7ff;     //no pulluup

	// Reserv(0), Infra(0, dis), Parity(000, none), Stop(0, 1bit), WordLen(11, 8bit), 8N1
	rULCON0  = 0x03; 
	// (11-10, 00, PCLK), interrupt request Tx-level,Rx-pulse,	(3-2, 01 TXmode ), (1-0, 01, RX mode) polling or interrupt    
    rUCON0   = 0x245;   	

	rUFCON0  = 0x00;     // (0, 0 , disable)No FIFO
    rUMCON0  = 0x00;     // (4, 0 , disable)No flow

	// clock of UART is PCLK
   	rUBRDIV0=( (int)(pclk/16.0/baud+0.5) -1 );   //波特率,四舍五入
}

/*------------------------------------------------------------/
函数名称：	uart0_sendbyte
功能描述：	串口发送一个字符函数
返 回 值：	无
-------------------------------------------------------------*/
void uart0_sendbyte(unsigned char data)
{   
	if(data=='\n') {
		while(!(rUTRSTAT0 & 0x2));
        rUTXH0 = '\r';
    }
	while(!(rUTRSTAT0 & 0x2));   //Wait until THR is empty.
    rUTXH0 = data;  
}    
           
/*------------------------------------------------------------/
函数名称：	uart0_sendstring
功能描述：	串口发送字符串函数
返 回 值：	无
-------------------------------------------------------------*/
void uart0_sendstring(unsigned char *pt)
{
    while(*pt) {
		uart0_sendbyte(*pt++);
	}
}
/*------------------------------------------------------------/
函数名称：	uart0_printf
功能描述：	串口发送字符串函数
返 回 值：	无
-------------------------------------------------------------*/		
//If you don't use vsprintf(), the code size is reduced very much.
void uart0_printf(char *fmt,...)
{
    va_list ap;
    char str[255];

    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    uart0_sendstring(str);
    va_end(ap);
}

/*------------------------------------------------------------/
函数名称：	uart0_getbyte
功能描述：	串口接收一个字符函数（查询方式）
返 回 值：	无
-------------------------------------------------------------*/
char uart0_getbyte(void)
{   
	while(!(rUTRSTAT0 & 0x1));   //Wait until rx is ready.
    return rURXH0; 
} 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void uart1_init(int pclk, int baud)
{	
    // GPH0，GPH1，GPH2，GPH3: nCTS0，nRTS0，TXD0，RXD0,  bit7~bit0 10 uart func
	rGPHCON  |= (0x2 << 8) + (0x2 << 10) ;    
    rGPHUP   = 0x7ff;     //no pulluup

	// Reserv(0), Infra(0, dis), Parity(000, none), Stop(0, 1bit), WordLen(11, 8bit), 8N1
	rULCON1  = 0x03; 
	// (11-10, 00, PCLK), interrupt request Tx-level,Rx-pulse,	(3-2, 01 TXmode ), (1-0, 01, RX mode) polling or interrupt    
    rUCON1   = 0x245;   	

	rUFCON1  = 0x00;     // (0, 0 , disable)No FIFO
    rUMCON1  = 0x00;     // (4, 0 , disable)No flow

	// clock of UART is PCLK
   	rUBRDIV1=( (int)(pclk/16.0/baud+0.5) -1 );   //波特率,四舍五入
}

/*------------------------------------------------------------/
函数名称：	uart0_sendbyte
功能描述：	串口发送一个字符函数
返 回 值：	无
-------------------------------------------------------------*/
void uart1_sendbyte(unsigned char data)
{   
	if(data=='\n') {
		while(!(rUTRSTAT1 & 0x2));
        rUTXH1 = '\r';
    }
	while(!(rUTRSTAT1 & 0x2));   //Wait until THR is empty.
    rUTXH1 = data;  
}    
           
/*------------------------------------------------------------/
函数名称：	uart0_sendstring
功能描述：	串口发送字符串函数
返 回 值：	无
-------------------------------------------------------------*/
void uart1_sendstring(unsigned char *pt)
{
    while(*pt) {
		uart1_sendbyte(*pt++);
	}
}
/*------------------------------------------------------------/
函数名称：	uart0_printf
功能描述：	串口发送字符串函数
返 回 值：	无
-------------------------------------------------------------*/		
//If you don't use vsprintf(), the code size is reduced very much.
void uart1_printf(char *fmt,...)
{
    va_list ap;
    char str[255];

    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    uart1_sendstring(str);
    va_end(ap);
}

/*------------------------------------------------------------/
函数名称：	uart0_getbyte
功能描述：	串口接收一个字符函数（查询方式）
返 回 值：	无
-------------------------------------------------------------*/
char uart1_getbyte(void)
{   
	while(!(rUTRSTAT1 & 0x1))
	OSTimeDly(2);   //Wait until rx is ready.
   	 return rURXH1; 
	/*
	//while(!(rUTRSTAT1 & 0x1));   //Wait until rx is ready.
	char cha=0x06;
	if(rUTRSTAT1 & 0x1)  //Wait until rx is ready.
	{
    		
    		if(rURXH1 != cha)
    		{
    			return rURXH1;
    		}
    		
    	}
   // return 1;
   */ 
} 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void uart2_init(int pclk, int baud)
{	
    // GPH0，GPH1，GPH2，GPH3: nCTS0，nRTS0，TXD0，RXD0,  bit7~bit0 10 uart func
	//rGPHCON  |= (0x2) + (0x2 << 2) + (0x2 << 4) + (0x2 << 6);  
	rGPHCON  |=  (0x2 << 12) + (0x2 << 14) ;   
    rGPHUP   = 0x7ff;     //no pulluup

	// Reserv(0), Infra(0, dis), Parity(000, none), Stop(0, 1bit), WordLen(11, 8bit), 8N1
	rULCON2  = 0x03; 
	// (11-10, 00, PCLK), interrupt request Tx-level,Rx-pulse,	(3-2, 01 TXmode ), (1-0, 01, RX mode) polling or interrupt    
    rUCON2   = 0x245;   	

	rUFCON2  = 0x00;     // (0, 0 , disable)No FIFO
    rUMCON2  = 0x00;     // (4, 0 , disable)No flow

	// clock of UART is PCLK
   	rUBRDIV2=( (int)(pclk/16.0/baud+0.5) -1 );   //波特率,四舍五入
}

/*------------------------------------------------------------/
函数名称：	uart0_sendbyte
功能描述：	串口发送一个字符函数
返 回 值：	无
-------------------------------------------------------------*/
void uart2_sendbyte(unsigned char data)
{   
	if(data=='\n') {
		while(!(rUTRSTAT2 & 0x2));
        rUTXH2 = '\r';
    }
	while(!(rUTRSTAT2& 0x2));   //Wait until THR is empty.
    rUTXH2 = data;  
}    
           
/*------------------------------------------------------------/
函数名称：	uart0_sendstring
功能描述：	串口发送字符串函数
返 回 值：	无
-------------------------------------------------------------*/
void uart2_sendstring(unsigned char *pt)
{
    while(*pt) {
		uart2_sendbyte(*pt++);
	}
}
/*------------------------------------------------------------/
函数名称：	uart0_printf
功能描述：	串口发送字符串函数
返 回 值：	无
-------------------------------------------------------------*/		
//If you don't use vsprintf(), the code size is reduced very much.
void uart2_printf(char *fmt,...)
{
    va_list ap;
    char str[255];

    va_start(ap,fmt);
    vsprintf(str,fmt,ap);
    uart2_sendstring(str);
    va_end(ap);
}

/*------------------------------------------------------------/
函数名称：	uart0_getbyte
功能描述：	串口接收一个字符函数（查询方式）
返 回 值：	无
-------------------------------------------------------------*/
char uart2_getbyte(void)
{   
	while(!(rUTRSTAT2 & 0x1));   //Wait until rx is ready.
    return rURXH2; 
} 
