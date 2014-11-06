/*****************************************
  NAME: Touchpanel.c
  DESC: ADC & Touch screen test
  HISTORY:
  2003.09.23:Leon YH KIM: draft ver 1.0
 *****************************************/
#include "config.h"
#include "Touchpanel.h"
#include "GUI.H"
#include "bt.h"

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
    	//GUI_Clear();
    	//AdButton=BUTTON_Create(57,27,42,45,GUI_ID_UP,WM_CF_SHOW);
		//BUTTON_SetBitmapEx(AdButton,0,&bmadvance,0,0);
    	//Beep(1000,100);
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
   Uart_Printf("\n\ntc_pos_x = %d, tc_pos_y = %d\n",tc_pos_x,tc_pos_y);
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



void TouchpanelInit(void)
{
   
    rADCDLY=50000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
    rADCCON=(1<<14)+(ADCPRS<<6);   	//ADCPRS En, ADCPRS Value
    rADCTSC=0xd3;  					//Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

    pISR_ADC = (uint32)AdcTsAuto;
    
	rINTMSK &=~BIT_ADC;       		//ADC Touch Screen Mask bit clear
	rINTSUBMSK &=~(BIT_SUB_TC);
}


void AdcTsAuto(void)
{
   	
 	static COORD CoordDat;
 	static INT8U Count = 0;
   	int i;
	U32 saveAdcdly;

    if(rADCDAT0&0x8000)
    {
		//Uart_Printf("\nStylus Up!!\n");
		rADCTSC&=0xff;	// Set stylus down interrupt bit
    }
    //else 
		//Uart_Printf("\nStylus Down!!\n");

	rADCTSC=(1<<3)|(1<<2);         //Pull-up disable, Seq. X,Y postion measure.
	saveAdcdly=rADCDLY;
	rADCDLY=40000;                 //Normal conversion mode delay about (1/50M)*40000=0.8ms

	rADCCON|=0x1;                   //start ADC

	while(rADCCON & 0x1);		//check if Enable_start is low
	while(!(rADCCON & 0x8000));        //check if EC(End of Conversion) flag is high, This line is necessary~!!
		
    while(!(rSRCPND & (BIT_ADC)));  //check if ADC is finished with interrupt bit

    CoordDat.CoordX =(rADCDAT0&0x3ff);
    CoordDat.CoordY =(rADCDAT1&0x3ff);

	 //YH 0627, To check Stylus Up Interrupt.
	 rSUBSRCPND|=BIT_SUB_TC;
	 ClearPending(BIT_ADC);
	 rINTSUBMSK&=~(BIT_SUB_TC);
	 rINTMSK&=~(BIT_ADC);
			 
	 rADCTSC =0xd3;    //Waiting for interrupt
	 rADCTSC=rADCTSC|(1<<8); // Detect stylus up interrupt signal.

	while(1)		//to check Pen-up state
	{
		if(rSUBSRCPND & (BIT_SUB_TC))	//check if ADC is finished with interrupt bit
		{
			//Uart_Printf("Stylus Up Interrupt~!\n");
			break;	//if Stylus is up(1) state
		}
	}	

   OSPrintf("XP=%04d, YP=%04d\n", CoordDat.CoordX, CoordDat.CoordY);    //X-position Conversion data            
	
	//Count++;
	/*往三个消息邮箱发送信号*/
	//OSMboxPost(LEDMbox, &Count);
	//OSMboxPost(BeepMbox,&CoordDat);
	//OSMboxPost(LCDMbox,&CoordDat);
	
	rADCDLY=saveAdcdly; 
	rADCTSC=rADCTSC&~(1<<8); // Detect stylus Down interrupt signal.
    rSUBSRCPND|=BIT_SUB_TC;
    rINTSUBMSK=~(BIT_SUB_TC);	// Unmask sub interrupt (TC)     
    ClearPending(BIT_ADC);
}
