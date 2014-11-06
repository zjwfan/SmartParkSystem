//====================================================================
// File Name : RTC.c
// Function  : S3C24X0 RTC Test 
// Program   : Kang, Weon Tark 
// Date      : May 22, 2002
// Version   : 0.0
// History
//   0.0 : Programming start (March 29,2002) -> KWT
//         RTC Test                          -> May 15, 2002 SOP
//====================================================================
#include "config.h"
#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 

//==================================================================================		
void RTC_Time_Set( void )
{
	rRTCCON = 1 ;		//RTC read and write enable

	rBCDYEAR = 0x13 ;		//��
    rBCDMON  = 0x07 ;		//��
    rBCDDATE = 0x15 ;		//��	
	rBCDDAY  = 0x01 ;		//����
	rBCDHOUR = 0x08 ;		//Сʱ
    rBCDMIN  = 0x55 ;		//��
    rBCDSEC  = 0x30 ;		//��
	
	rRTCCON &= ~1 ;		//RTC read and write disable
}

//==================================================================================
void RTC_Display(void) 
{
	U16 year ;
	U8 month, day ;		// week
	U8 hour, minute, second ;

	  //RTC_Time_Set() ;
	   

		rRTCCON = 1 ;		//RTC read and write enable

		year = 0x2000+rBCDYEAR  ;		//��
	    month = rBCDMON  ;		//��
	    day = rBCDDATE  ;		//��	
//		week = rBCDDAY  ;		//����
		hour = rBCDHOUR  ;		//Сʱ
	    minute = rBCDMIN  ;		//��
	    second = rBCDSEC  ;		//��
		
		
        Lcd_printf(5, 226, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0, "%04x-%02x-%02x %02x:%02x:%02x\n", year, month, day, hour, minute, second );
        rRTCCON &= ~1 ;		//RTC read and write disable
     	
    
}