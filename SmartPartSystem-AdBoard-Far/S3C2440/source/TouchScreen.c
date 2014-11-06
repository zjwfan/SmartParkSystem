
/**************************************************************
Touch Screen control
**************************************************************/
#include <string.h>
#include "2440addr.h"
#include "2440lib.h"
#include "config.h"

#include "GUI.h"
//=======================================
#include "app_cfg.h"
#include "TouchScreen.h"
#define LOOP 1
#define ADCPRS 0x27
int tc_pos_x=0;//触摸坐标x
int tc_pos_y=0;//触摸坐标y
//extern OS_EVENT *TouchMbox;
int isDown;

//unsigned char TouchHappen=1;

//	中断处理函数：
static void TSIrqISR(void)
{
    int i;
    U32 Pt[6];
    
    #if OS_CRITICAL_METHOD == 3                                // Allocate storage for CPU status register  
     	OS_CPU_SR  cpu_sr;
   	#endif
 // 	OS_ENTER_CRITICAL();
 	OSIntEnter();
 //	OS_EXIT_CRITICAL();
    rINTSUBMSK |= (BIT_SUB_ADC|BIT_SUB_TC); 
     if(rADCDAT0 & 0x8000)
    {//抬起
    	//Uart_Printf("Up\n"); 
        isDown = 0;
        rADCTSC &= 0xff;    // Set stylus down interrupt
        tc_pos_x = -1;
        tc_pos_y = -1; //抬起触笔时，tc_pos_x,tc_pos_y要值成不大于0的数
    }
    else //按下
    {        
    	//Uart_Printf("Down\n"); 
    	isDown = 1;
        rADCTSC=(0<<8)|(0<<7)|(0<<6)|(1<<5)|(1<<4)|(1<<3)|(0<<2)|(1);
        for(i=0;i<LOOP;i++);            //delay to set up the next channel
        for(i=0;i<3;i++)                           //5 times
        {
             rADCCON|=0x1;               		// Start X-position conversion
            while(rADCCON & 0x1);       		// Check if Enable_start is low
            while(!(0x8000&rADCCON));   		// Check ECFLG
                     Pt[i]=(0x3ff&rADCDAT0);
        }
        Pt[5]=(Pt[0]+Pt[1]+Pt[2])/3;//多次采样取平均值
        tc_pos_x = Pt[5];
        rADCTSC=(0<<8)|(0<<7)|(1<<6)|(1<<5)|(0<<4)|(1<<3)|(0<<2)|(2);
        for(i=0;i<LOOP;i++);            		// delay to set up the next channel
        for(i=0;i<3;i++)                        // 5 times
        {
            rADCCON|=0x1;               		// Start Y-position conversion
            while(rADCCON & 0x1);       		// Check if Enable_start is low
            while(!(0x8000&rADCCON));   		// Check ECFLG
            Pt[i]=(0x3ff&rADCDAT1);
        }
        Pt[5]=(Pt[0]+Pt[1]+Pt[2])/3;			// 多次采样取平均值
 
        tc_pos_y = Pt[5];
        rADCTSC=(1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
}
   //Uart_Printf("\n\ntc_pos_x = %d, tc_pos_y = %d\n",tc_pos_x,tc_pos_y);
  //  OSPrintf("OSMboxPost\n");
    rSUBSRCPND |= BIT_SUB_TC;
    rINTSUBMSK &= ~(BIT_SUB_TC);         // Unmask sub interrupt (TC)     
    ClearPending(BIT_ADC);  
  // OSMboxPost(TouchMbox, &TouchHappen);//向处理触摸进程发消息
    OSIntExit();
   
}



void Touch_Screen_Init(void)
{
    rADCDLY = (50000); 
    rADCCON = (1<<14)|(ADCPRS<<6)|(7<<3)|(0<<2)|(0<<1)|(0);
    rADCTSC = (0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
    pISR_ADC = (U32)TSIrqISR; //?¨ò??D???ìó|oˉêy
    rINTMSK &= ~(BIT_ADC);
    rINTSUBMSK &= ~(BIT_SUB_TC);
}


