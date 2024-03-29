/**************************************************************
 NAME: option.h
 DESC: To measuure the USB download speed, the WDT is used.
       To measure up to large time, The WDT interrupt is used.
 HISTORY:
 Feb.20.2002:Shin, On Pil: Programming start
 Mar.25.2002:purnnamu: S3C2400X profile.c is ported for S3C2440X.
 Jan.E.2004:DonGo: Modified for S3C2440a.
 **************************************************************/
 
#ifndef __OPTION_H__
#define __OPTION_H__

#ifdef __cplusplus
	extern "C" {
#endif

//--by Customer--

//#define LCD_N35
//#define LCD_L80
//#define LCD_T35
#define LCD_W35
//#define LCD_X35
//#define LCD_A70
//#define LCD_VGA1024768

//--end of by Customer--

#if defined(LCD_N35) + defined(LCD_L80) + defined(LCD_T35) + defined(LCD_A70) + defined(LCD_VGA1024768) + defined(LCD_X35) + defined(LCD_W35) != 1
#error Must define only one LCD type
#endif
	 
#if defined(LCD_N35)

#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_PIXCLOCK 4

#define LCD_RIGHT_MARGIN 36
#define LCD_LEFT_MARGIN 19
#define LCD_HSYNC_LEN 5

#define LCD_UPPER_MARGIN 1
#define LCD_LOWER_MARGIN 5
#define LCD_VSYNC_LEN 1

#elif defined(LCD_L80)
#define LCD_WIDTH 640
#define LCD_HEIGHT 480
#define LCD_PIXCLOCK 2

#define LCD_RIGHT_MARGIN 67 
#define LCD_LEFT_MARGIN 40
#define LCD_HSYNC_LEN 31

#define LCD_UPPER_MARGIN 25
#define LCD_LOWER_MARGIN 5
#define LCD_VSYNC_LEN 1


#elif defined(LCD_T35)
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_PIXCLOCK 4

#define LCD_RIGHT_MARGIN 25
#define LCD_LEFT_MARGIN 0
#define LCD_HSYNC_LEN 4

#define LCD_UPPER_MARGIN 1
#define LCD_LOWER_MARGIN 4
#define LCD_VSYNC_LEN 1
#define LCD_CON5 ( (1 << 11)| (1<<0) | (1 << 8) | (1 << 6) | (1 << 9) | ( 1<< 10))

#elif defined(LCD_W35)
#define LCD_PIXCLOCK 4

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

#define LCD_RIGHT_MARGIN 0x44
#define LCD_LEFT_MARGIN 0x04
#define LCD_HSYNC_LEN 0x01

#define LCD_UPPER_MARGIN 10
#define LCD_LOWER_MARGIN 4
#define LCD_VSYNC_LEN 1

#define LCD_CON5 ((1<<11) | (1<<8) | (1<<9) | (1<<0) ) 

#elif defined(LCD_X35)
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define LCD_PIXCLOCK 4

#define LCD_RIGHT_MARGIN 25
#define LCD_LEFT_MARGIN 0
#define LCD_HSYNC_LEN 4

#define LCD_UPPER_MARGIN 0
#define LCD_LOWER_MARGIN 4
#define LCD_VSYNC_LEN 9
#define LCD_CON5 ( (1 << 11)| (1<<0) | (1 << 8) | (1 << 6) | (1 << 9) | ( 1<< 10))

#elif defined(LCD_A70)
#define LCD_WIDTH 800
#define LCD_HEIGHT 480
#define LCD_PIXCLOCK 2

#define LCD_RIGHT_MARGIN 67
#define LCD_LEFT_MARGIN 40
#define LCD_HSYNC_LEN 31

#define LCD_UPPER_MARGIN 25
#define LCD_LOWER_MARGIN 5
#define LCD_VSYNC_LEN 1

#elif defined(LCD_VGA1024768)
#define LCD_WIDTH 1024
#define LCD_HEIGHT 768
#define LCD_PIXCLOCK 2

#define LCD_RIGHT_MARGIN 15
#define LCD_LEFT_MARGIN 199
#define LCD_HSYNC_LEN 15

#define LCD_UPPER_MARGIN 1
#define LCD_LOWER_MARGIN 1
#define LCD_VSYNC_LEN 1
#define LCD_CON5 ( (1 << 11)| (1<<0) )

#endif



#include "Def1.h"
#ifndef ADS10
#define ADS10 			TRUE
#endif
#define USE_MAIN		FALSE  // change USE_MAIN value in option.a together  
#define SEMIHOSTING		FALSE

//USE_MAIN & SEMIHOSTING are valid only if ADS10==TRUE.
#if !ADS10
    #undef USE_MAIN
    #define USE_MAIN	FALSE
#endif
#if !USE_MAIN
    #undef SEMIHOSTING	
    #define SEMIHOSTING	FALSE
#endif

#define MEGA	(1000000)

///////////////////// clock select ///////////////////////////////////////////////
#define CPU2440A	TRUE	// else CPU2440X

#define FIN 	(12000000)
//#define FIN		(16934400)

// Main clock
#if FIN==12000000
//#define FCLK 304800000
//#define FCLK 300000000
	//#define FCLK 304800000
#define FCLK 400000000
	//#define FCLK 451200000
	//#define FCLK 532800000
	#if FCLK==271500000
	#define HCLK (FCLK/2)
	#define PCLK (HCLK/2)
	#elif FCLK==304800000
	#define HCLK (FCLK/3)
	#define PCLK (HCLK/2)
	#elif FCLK==300000000
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#elif FCLK==400000000
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#elif FCLK==451200000
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#elif FCLK==532800000
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#endif
#else	//FIN=16.9344MHz
#define FCLK 296352000
	#if FCLK==266716800
	#define HCLK (FCLK/2)
	#define PCLK (HCLK/2)
	#elif FCLK==296352000
	#define HCLK (FCLK/3)
	#define PCLK (HCLK/2)
	#elif FCLK==399651840
	#define HCLK (FCLK/3)
	#define PCLK (HCLK/2)
	#elif FCLK==530611200
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#elif FCLK==541900800
	#define HCLK (FCLK/4)
	#define PCLK (HCLK/2)
	#endif
#endif
/////////////////////////////////////////////////////////////////////

// USB clock
#define UCLK 48000000

// BUSWIDTH : 16,32
#define BUSWIDTH    (32)

//64MB
// 0x30000000 ~ 0x30ffffff : Download Area (16MB) Cacheable
// 0x31000000 ~ 0x33feffff : Non-Cacheable Area
// 0x33ff0000 ~ 0x33ff47ff : Heap & RW Area
// 0x33ff4800 ~ 0x33ff7fff : FIQ ~ User Stack Area
// 0x33ff8000 ~ 0x33fffeff : Not Useed Area
// 0x33ffff00 ~ 0x33ffffff : Exception & ISR Vector Table

#define	_RAM_STARTADDRESS 		0x30000000
#define	_ISR_STARTADDRESS 		0x33ffff00     
#define	_MMUTT_STARTADDRESS		0x33ff8000
#define	_STACK_BASEADDRESS		0x33ff8000
#define	_NONCACHE_STARTADDRESS	0x31000000
#if !USE_MAIN
#define HEAPEND		  	0x43ff0000     //obsolete if USE_MAIN is TRUE.
#endif

//USB Device Options
#define USBDMA		1
#define USBDMA_DEMAND 	0	//the downloadFileSize should be (64*n)
#define BULK_PKT_SIZE	64

#define	UPDATE_REFRESH(IN_HCLK)	( rREFRESH = (rREFRESH & ~(0x7ff)) | 2049-(U32)((float)IN_HCLK*7.8/1000000) )

//CLKSEL0/1 Clock selection value define
#define	CLK_HCLK	(3)
#define	CLK_PCLK	(4)
#define	CLK_DCLK	(5)


#ifdef __cplusplus
}
#endif
#endif /*__OPTION_H__*/
