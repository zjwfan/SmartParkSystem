#include "2440addr.h"
#include "config.h"
#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
                     OS_CPU_SR  cpu_sr;
 #endif
//#define FIN 12000000

/*------------------------------------------------------------/
全局变量声明
-------------------------------------------------------------*/
//unsigned int FCLK,HCLK,PCLK;	 //时钟

/*-----------------------函数声明----------------------------*/
void watchdog_disable(void);


void RTC_time_set(void);
void RTC_display(void);

extern void uart0_init(int pclk, int baud);
extern void uart0_sendbyte(unsigned char data);
extern void uart0_sendstring(unsigned char *pt);
extern void uart0_printf(char *fmt,...);

void enable_uart_rx_irq(void);

void enable_eint_irq(void);

void enable_timer4_irq(void);

void enable_irq(void);
void disable_irq(void);


/*------------------------------------------------------------/
函数名称：	Main
功能描述：	入口函数
传    参：	无
返 回 值：	无
-------------------------------------------------------------*/
/*
int Main(void) 
{ 
	unsigned int i;
	unsigned int *p;
	
	watchdog_disable();	 //关闭watchdog

	clock_init(); 	//时钟初始化
	
	led_init();	  //LED初始化
	
	uart0_init(PCLK, 115200);	//UART0初始化

	uart0_sendstring("Hello FL2440!\n");

	p = (unsigned int *)0x31000000;		 //SDRAM测试的起始地址
	SDRAM_init();	//SDRAM初始化,并对SDRAM进行测试
	for(i=0;i<1000;i++)
	{
		*(p+i) = i;
		if(*(p+i) != i)
			uart0_sendstring("SDRAM test failed\n");
	}
	uart0_sendstring("SDRAM test finished!\n");


    enable_eint_irq(); 	   //使能外部中断 

	enable_uart_rx_irq();  //使能串口中断(这里接收数据采用中断方式，前面的发送数据用的是查询方式)

	enable_timer4_irq();   //使能定时器4中断

	ADC_init();
	RTC_time_set();

	while (1)		       // 死循环
	{
	}
	return 0; 
}
*/
/*------------------------------------------------------------/
函数名称：	watchdog_disable
功能描述：	关闭看门狗
返 回 值：	无
-------------------------------------------------------------*/
void watchdog_disable(void)
{ 	
	rWTCON = 0; // set bit0 to 0 to disable timeout reset;
}



/*------------------------------------------------------------/
函数名称：	RTC_time_set
功能描述：	RTC时间设置函数
返 回 值：	无
-------------------------------------------------------------*/
void RTC_time_set(void)
{
	rRTCCON = 1 ;		//RTC read and write enable

	rBCDYEAR = 0x13 ;		//年
    rBCDMON  = 0x04 ;		//月
    rBCDDATE = 0x07 ;		//日	
	rBCDHOUR = 0x10 ;		//小时
    rBCDMIN  = 0x50 ;		//分
    rBCDSEC  = 0x30 ;		//秒
	
	rRTCCON &= ~1 ;		//RTC read and write disable
}

/*------------------------------------------------------------/
函数名称：	RTC_display
功能描述：	RTC时间设置函数
返 回 值：	无
-------------------------------------------------------------*/
void RTC_display(void)
{
	unsigned short year ;
	unsigned char month, day ;  //week		
	unsigned char hour, minute, second ;
	rRTCCON = 1 ;		//RTC read and write enable

	year = 0x2000+rBCDYEAR  ;	//年
	month = rBCDMON  ;		    //月
	day = rBCDDATE  ;		    //日	
//		week = BCDDAY  ;	    //星期
	hour = rBCDHOUR  ;		//小时
	minute = rBCDMIN  ;		//分
	second = rBCDSEC  ;		//秒
		
	rRTCCON &= ~1 ;		//RTC read and write disable

	uart0_printf( "RTC time : %04x-%02x-%02x %02x:%02x:%02x\n", year, month, day, hour, minute, second );
}


 /*------------------------------------------------------------/
函数名称：	Eint0_ISR
功能描述：	外部中断0函数
传    参：	无
返 回 值：	无
-------------------------------------------------------------*/
void __irq Eint0_ISR(void) 
{ 
    disable_irq();

	ClearPending(BIT_EINT0);	//清除外部中断0的中断源请求
	uart0_sendstring("EINT0  is occurred.\n"); 
	rGPBDAT = ~(1<<5);			//LED0亮

	enable_irq();
} 

/*------------------------------------------------------------/
函数名称：	Eint2_ISR
功能描述：	外部中断2函数
传    参：	无
返 回 值：	无
-------------------------------------------------------------*/
void __irq Eint2_ISR(void) 
{ 
	disable_irq();

    ClearPending(BIT_EINT2);	//清除外部中断2的中断源请求 
    uart0_sendstring("EINT2 is occurred.\n"); 
	rGPBDAT = ~(1<<6);			//LED1亮

	enable_irq();
} 
/*------------------------------------------------------------/
函数名称：	Eint3_ISR
功能描述：	外部中断3函数
传    参：	无
返 回 值：	无
-------------------------------------------------------------*/
void __irq Eint3_ISR(void) 
{ 
	disable_irq();

	ClearPending(BIT_EINT3);	//清除外部中断3的中断源请求
	uart0_sendstring("EINT3  is occurred.\n"); 
	rGPBDAT = ~(1<<8);			//LED2亮

	enable_irq();
} 

/*------------------------------------------------------------/
函数名称：	Eint4_ISR
功能描述：	外部中断4函数
传    参：	无
返 回 值：	无
-------------------------------------------------------------*/
void __irq Eint4_ISR(void) 
{   
	disable_irq();
	 	
 	rEINTPEND = (1<<4);	        //清除外部中断标志（EINT4~EINT23需要）	

    ClearPending(BIT_EINT4_7);	//清除外部中断4的中断源请求 
    uart0_sendstring("EINT4 is occurred.\n"); 
	rGPBDAT = ~(1<<10);			//LED3亮

	enable_irq();
} 

/*------------------------------------------------------------/
函数名称：	enable_eint_irq
功能描述：	使能外部中断
传    参：	无
返 回 值：	无
-------------------------------------------------------------*/
void enable_eint_irq(void) 
{ 
	rGPFCON &=~((3<<0)|(3<<4)|(3<<6)|(3<<8));
	rGPFCON |= ((2<<0)|(2<<4)|(2<<6)|(2<<8));  //Set GPF0,2,3,4 as EINT0,2,3,4
	      
	rGPFUP|=((1<<0)|(1<<2)|(1<<3)|(1<<4));    //关断 GPF0,GPF2,GPF3,GPF4上拉
	 
    rEXTINT0 &= ~(7<<0|7<<8|7<<12|7<<16);     
	rEXTINT0 |= (2<<0|2<<8|2<<12|2<<16);     //EINT0,2 Falling edge triggered
	
	rEINTPEND = 0xffffff;	//清除外部中断标志（EINT4~EINT23需要）	
		
	rSRCPND |= BIT_EINT0|BIT_EINT2|BIT_EINT3|BIT_EINT4_7; 
	rINTPND |= BIT_EINT0|BIT_EINT2|BIT_EINT3|BIT_EINT4_7; 	  //清除中断EINT0,EINT2,EINT3,EINT4标志

	rEINTMASK=~(1<<4);	   //EINT4~EINT23需要设置

	rINTMSK=~(BIT_EINT0|BIT_EINT2|BIT_EINT3|BIT_EINT4_7);     //关闭外部中断EINT0,EINT2,EINT3,EINT4屏蔽
  
    pISR_EINT0 = (unsigned)Eint0_ISR;		//挂载中断 
    pISR_EINT2 = (unsigned)Eint2_ISR;   
    pISR_EINT3 = (unsigned)Eint3_ISR;   
    pISR_EINT4_7 = (unsigned)Eint4_ISR;   
}

 /*------------------------------------------------------------/
函数名称：	UART0_ISR
功能描述：	串口中断函数
传    参：	无
返 回 值：	无
-------------------------------------------------------------*/
void __irq UART0_ISR(void) 
{ 
	char ch;

    disable_irq();

	rSUBSRCPND |= 0x01;   //[0]--INT_RXD0,清子中断标志
    rSRCPND |= 1<<28;     //[28]--INT_UART0,清中断标志
	rINTPND |= 1<<28;     //[28]--INT_UART0,清中断标志

	if(rUTRSTAT0&0x01)	 //接收到数据
	{
		ch = rURXH0;
		uart0_sendbyte(ch);	 //将接收到的数据发送出去
		if(ch==0x55)
			//ADC_display();	 //显示ADC

		if(ch==0xaa)
			RTC_display();	 //显示RTC
	}

	enable_irq();
} 


/*------------------------------------------------------------/
函数名称：	enable_uart0_rx_irq
功能描述：	使能串口接收中断
返 回 值：	无
-------------------------------------------------------------*/
void enable_uart_rx_irq(void)
{
    rSRCPND |= 1<<28;     //[28]--INT_UART0,清中断标志
	rSUBSRCPND |= 0x01;   //[0]--INT_RXD0,清子中断标志
	rINTPND |= 1<<28;     //[28]--INT_UART0,清中断标志

	rINTMSK &= ~(1<<28);	 //打开UART0接收中断屏蔽
	rINTSUBMSK &= ~(1<<0);

    pISR_UART0 = (unsigned)UART0_ISR;		//挂载中断 
}
 /*------------------------------------------------------------/
函数名称：	Timer4_ISR
功能描述：	定时器4中断函数
传    参：	无
返 回 值：	无
-------------------------------------------------------------*/
void __irq Timer4_ISR(void) 
{ 
	static int count;
    disable_irq();

	count++;
	count = count % 4;

    rSRCPND |= 1<<14;     //[14]--INT_TIMER4,清中断标志
	rINTPND |= 1<<14;     //[28]--INT_TIMER4,清中断标志

	//每隔2秒蜂鸣器响一次，持续时间为0.5秒
	if (count==0)
		rGPBDAT |= 0x01;      //蜂鸣器响
	else if (count==1)
		rGPBDAT &= 0xfffe;    //蜂鸣器不响

	enable_irq();
} 


/*------------------------------------------------------------/
函数名称：	enable_timer4_irq
功能描述：	使能定时器4中断
返 回 值：	无
-------------------------------------------------------------*/
void enable_timer4_irq(void)
{
    rSRCPND |= 1<<14;     //[14]--INT_TIMER4,清中断标志
	rINTPND |= 1<<14;     //[28]--INT_TIMER4,清中断标志

	rINTMSK &= ~(1<<14);	 //打开Timer4中断屏蔽

	rTCFG0 &= 0xFF00FF;
	rTCFG0 |= 0xf900;       // prescaler等于249

	rTCFG1 &= ~0xF0000;    
	rTCFG1 |= 0x10000;      //divider等于4，则设置定时器4的时钟频率为25kHz
    
	rTCNTB4 = 12500;        //让定时器4每隔0.5秒中断一次

    rTCON &= ~0xF00000;
	rTCON |= 0x700000;	   //auto reload; update TCNTB4; start for timer4
	rTCON &= ~0x200000 ;    //定时器4开始工作


    pISR_TIMER4 = (unsigned)Timer4_ISR;		//挂载中断 
}

/*------------------------------------------------------------/
函数名称：	
功能描述：	打开中断
返 回 值：	无
-------------------------------------------------------------*/
void enable_irq(void)
{
	
    
    OS_ENTER_CRITICAL(); 
/*	register int R0;
	__asm
	{
		mrs R0, cpsr; 
		bic R0, R0, #0x80;
		msr	cpsr, R0;
	};	
*/	
}

/*------------------------------------------------------------/
函数名称：	disable_irq
功能描述：	关闭中断
返 回 值：	无
-------------------------------------------------------------*/
void disable_irq(void)
{
	OS_EXIT_CRITICAL();
/*	register int R0;
	__asm
	{
		mrs R0, cpsr; 
		orr R0, R0, #0x80;
		msr	cpsr, R0;
	};
*/	
}



 