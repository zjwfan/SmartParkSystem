#include "2440addr.h"
#include "config.h"
#if OS_CRITICAL_METHOD == 3          /* Allocate storage for CPU status register */
                     OS_CPU_SR  cpu_sr;
 #endif
//#define FIN 12000000

/*------------------------------------------------------------/
ȫ�ֱ�������
-------------------------------------------------------------*/
//unsigned int FCLK,HCLK,PCLK;	 //ʱ��

/*-----------------------��������----------------------------*/
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
�������ƣ�	Main
����������	��ں���
��    �Σ�	��
�� �� ֵ��	��
-------------------------------------------------------------*/
/*
int Main(void) 
{ 
	unsigned int i;
	unsigned int *p;
	
	watchdog_disable();	 //�ر�watchdog

	clock_init(); 	//ʱ�ӳ�ʼ��
	
	led_init();	  //LED��ʼ��
	
	uart0_init(PCLK, 115200);	//UART0��ʼ��

	uart0_sendstring("Hello FL2440!\n");

	p = (unsigned int *)0x31000000;		 //SDRAM���Ե���ʼ��ַ
	SDRAM_init();	//SDRAM��ʼ��,����SDRAM���в���
	for(i=0;i<1000;i++)
	{
		*(p+i) = i;
		if(*(p+i) != i)
			uart0_sendstring("SDRAM test failed\n");
	}
	uart0_sendstring("SDRAM test finished!\n");


    enable_eint_irq(); 	   //ʹ���ⲿ�ж� 

	enable_uart_rx_irq();  //ʹ�ܴ����ж�(����������ݲ����жϷ�ʽ��ǰ��ķ��������õ��ǲ�ѯ��ʽ)

	enable_timer4_irq();   //ʹ�ܶ�ʱ��4�ж�

	ADC_init();
	RTC_time_set();

	while (1)		       // ��ѭ��
	{
	}
	return 0; 
}
*/
/*------------------------------------------------------------/
�������ƣ�	watchdog_disable
����������	�رտ��Ź�
�� �� ֵ��	��
-------------------------------------------------------------*/
void watchdog_disable(void)
{ 	
	rWTCON = 0; // set bit0 to 0 to disable timeout reset;
}



/*------------------------------------------------------------/
�������ƣ�	RTC_time_set
����������	RTCʱ�����ú���
�� �� ֵ��	��
-------------------------------------------------------------*/
void RTC_time_set(void)
{
	rRTCCON = 1 ;		//RTC read and write enable

	rBCDYEAR = 0x13 ;		//��
    rBCDMON  = 0x04 ;		//��
    rBCDDATE = 0x07 ;		//��	
	rBCDHOUR = 0x10 ;		//Сʱ
    rBCDMIN  = 0x50 ;		//��
    rBCDSEC  = 0x30 ;		//��
	
	rRTCCON &= ~1 ;		//RTC read and write disable
}

/*------------------------------------------------------------/
�������ƣ�	RTC_display
����������	RTCʱ�����ú���
�� �� ֵ��	��
-------------------------------------------------------------*/
void RTC_display(void)
{
	unsigned short year ;
	unsigned char month, day ;  //week		
	unsigned char hour, minute, second ;
	rRTCCON = 1 ;		//RTC read and write enable

	year = 0x2000+rBCDYEAR  ;	//��
	month = rBCDMON  ;		    //��
	day = rBCDDATE  ;		    //��	
//		week = BCDDAY  ;	    //����
	hour = rBCDHOUR  ;		//Сʱ
	minute = rBCDMIN  ;		//��
	second = rBCDSEC  ;		//��
		
	rRTCCON &= ~1 ;		//RTC read and write disable

	uart0_printf( "RTC time : %04x-%02x-%02x %02x:%02x:%02x\n", year, month, day, hour, minute, second );
}


 /*------------------------------------------------------------/
�������ƣ�	Eint0_ISR
����������	�ⲿ�ж�0����
��    �Σ�	��
�� �� ֵ��	��
-------------------------------------------------------------*/
void __irq Eint0_ISR(void) 
{ 
    disable_irq();

	ClearPending(BIT_EINT0);	//����ⲿ�ж�0���ж�Դ����
	uart0_sendstring("EINT0  is occurred.\n"); 
	rGPBDAT = ~(1<<5);			//LED0��

	enable_irq();
} 

/*------------------------------------------------------------/
�������ƣ�	Eint2_ISR
����������	�ⲿ�ж�2����
��    �Σ�	��
�� �� ֵ��	��
-------------------------------------------------------------*/
void __irq Eint2_ISR(void) 
{ 
	disable_irq();

    ClearPending(BIT_EINT2);	//����ⲿ�ж�2���ж�Դ���� 
    uart0_sendstring("EINT2 is occurred.\n"); 
	rGPBDAT = ~(1<<6);			//LED1��

	enable_irq();
} 
/*------------------------------------------------------------/
�������ƣ�	Eint3_ISR
����������	�ⲿ�ж�3����
��    �Σ�	��
�� �� ֵ��	��
-------------------------------------------------------------*/
void __irq Eint3_ISR(void) 
{ 
	disable_irq();

	ClearPending(BIT_EINT3);	//����ⲿ�ж�3���ж�Դ����
	uart0_sendstring("EINT3  is occurred.\n"); 
	rGPBDAT = ~(1<<8);			//LED2��

	enable_irq();
} 

/*------------------------------------------------------------/
�������ƣ�	Eint4_ISR
����������	�ⲿ�ж�4����
��    �Σ�	��
�� �� ֵ��	��
-------------------------------------------------------------*/
void __irq Eint4_ISR(void) 
{   
	disable_irq();
	 	
 	rEINTPEND = (1<<4);	        //����ⲿ�жϱ�־��EINT4~EINT23��Ҫ��	

    ClearPending(BIT_EINT4_7);	//����ⲿ�ж�4���ж�Դ���� 
    uart0_sendstring("EINT4 is occurred.\n"); 
	rGPBDAT = ~(1<<10);			//LED3��

	enable_irq();
} 

/*------------------------------------------------------------/
�������ƣ�	enable_eint_irq
����������	ʹ���ⲿ�ж�
��    �Σ�	��
�� �� ֵ��	��
-------------------------------------------------------------*/
void enable_eint_irq(void) 
{ 
	rGPFCON &=~((3<<0)|(3<<4)|(3<<6)|(3<<8));
	rGPFCON |= ((2<<0)|(2<<4)|(2<<6)|(2<<8));  //Set GPF0,2,3,4 as EINT0,2,3,4
	      
	rGPFUP|=((1<<0)|(1<<2)|(1<<3)|(1<<4));    //�ض� GPF0,GPF2,GPF3,GPF4����
	 
    rEXTINT0 &= ~(7<<0|7<<8|7<<12|7<<16);     
	rEXTINT0 |= (2<<0|2<<8|2<<12|2<<16);     //EINT0,2 Falling edge triggered
	
	rEINTPEND = 0xffffff;	//����ⲿ�жϱ�־��EINT4~EINT23��Ҫ��	
		
	rSRCPND |= BIT_EINT0|BIT_EINT2|BIT_EINT3|BIT_EINT4_7; 
	rINTPND |= BIT_EINT0|BIT_EINT2|BIT_EINT3|BIT_EINT4_7; 	  //����ж�EINT0,EINT2,EINT3,EINT4��־

	rEINTMASK=~(1<<4);	   //EINT4~EINT23��Ҫ����

	rINTMSK=~(BIT_EINT0|BIT_EINT2|BIT_EINT3|BIT_EINT4_7);     //�ر��ⲿ�ж�EINT0,EINT2,EINT3,EINT4����
  
    pISR_EINT0 = (unsigned)Eint0_ISR;		//�����ж� 
    pISR_EINT2 = (unsigned)Eint2_ISR;   
    pISR_EINT3 = (unsigned)Eint3_ISR;   
    pISR_EINT4_7 = (unsigned)Eint4_ISR;   
}

 /*------------------------------------------------------------/
�������ƣ�	UART0_ISR
����������	�����жϺ���
��    �Σ�	��
�� �� ֵ��	��
-------------------------------------------------------------*/
void __irq UART0_ISR(void) 
{ 
	char ch;

    disable_irq();

	rSUBSRCPND |= 0x01;   //[0]--INT_RXD0,�����жϱ�־
    rSRCPND |= 1<<28;     //[28]--INT_UART0,���жϱ�־
	rINTPND |= 1<<28;     //[28]--INT_UART0,���жϱ�־

	if(rUTRSTAT0&0x01)	 //���յ�����
	{
		ch = rURXH0;
		uart0_sendbyte(ch);	 //�����յ������ݷ��ͳ�ȥ
		if(ch==0x55)
			//ADC_display();	 //��ʾADC

		if(ch==0xaa)
			RTC_display();	 //��ʾRTC
	}

	enable_irq();
} 


/*------------------------------------------------------------/
�������ƣ�	enable_uart0_rx_irq
����������	ʹ�ܴ��ڽ����ж�
�� �� ֵ��	��
-------------------------------------------------------------*/
void enable_uart_rx_irq(void)
{
    rSRCPND |= 1<<28;     //[28]--INT_UART0,���жϱ�־
	rSUBSRCPND |= 0x01;   //[0]--INT_RXD0,�����жϱ�־
	rINTPND |= 1<<28;     //[28]--INT_UART0,���жϱ�־

	rINTMSK &= ~(1<<28);	 //��UART0�����ж�����
	rINTSUBMSK &= ~(1<<0);

    pISR_UART0 = (unsigned)UART0_ISR;		//�����ж� 
}
 /*------------------------------------------------------------/
�������ƣ�	Timer4_ISR
����������	��ʱ��4�жϺ���
��    �Σ�	��
�� �� ֵ��	��
-------------------------------------------------------------*/
void __irq Timer4_ISR(void) 
{ 
	static int count;
    disable_irq();

	count++;
	count = count % 4;

    rSRCPND |= 1<<14;     //[14]--INT_TIMER4,���жϱ�־
	rINTPND |= 1<<14;     //[28]--INT_TIMER4,���жϱ�־

	//ÿ��2���������һ�Σ�����ʱ��Ϊ0.5��
	if (count==0)
		rGPBDAT |= 0x01;      //��������
	else if (count==1)
		rGPBDAT &= 0xfffe;    //����������

	enable_irq();
} 


/*------------------------------------------------------------/
�������ƣ�	enable_timer4_irq
����������	ʹ�ܶ�ʱ��4�ж�
�� �� ֵ��	��
-------------------------------------------------------------*/
void enable_timer4_irq(void)
{
    rSRCPND |= 1<<14;     //[14]--INT_TIMER4,���жϱ�־
	rINTPND |= 1<<14;     //[28]--INT_TIMER4,���жϱ�־

	rINTMSK &= ~(1<<14);	 //��Timer4�ж�����

	rTCFG0 &= 0xFF00FF;
	rTCFG0 |= 0xf900;       // prescaler����249

	rTCFG1 &= ~0xF0000;    
	rTCFG1 |= 0x10000;      //divider����4�������ö�ʱ��4��ʱ��Ƶ��Ϊ25kHz
    
	rTCNTB4 = 12500;        //�ö�ʱ��4ÿ��0.5���ж�һ��

    rTCON &= ~0xF00000;
	rTCON |= 0x700000;	   //auto reload; update TCNTB4; start for timer4
	rTCON &= ~0x200000 ;    //��ʱ��4��ʼ����


    pISR_TIMER4 = (unsigned)Timer4_ISR;		//�����ж� 
}

/*------------------------------------------------------------/
�������ƣ�	
����������	���ж�
�� �� ֵ��	��
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
�������ƣ�	disable_irq
����������	�ر��ж�
�� �� ֵ��	��
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



 