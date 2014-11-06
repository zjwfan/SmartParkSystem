/************************************************
文件名:      Main.c
文件描述:    系统主文件
创建时间:    2014-07-10
作者：       物联闪电小组
************************************************/

#include "config.h"

INT8U CarInfo[15]={0};
INT32U CarPix[15][2]={{96,32},{144,32},{187,32},{230,32},{274,32},
{72,102},{122,102},{172,102},{220,103},
{23,183},{73,183},{123,183},{174,183},{222,183},{273,183}};
INT8U CarSave[3]={'#','#','#'};
INT32U CarNum=0;

extern const GUI_BITMAP bmCar000;
extern const GUI_BITMAP bmCar001;
extern const GUI_BITMAP bmCar002;
extern const GUI_BITMAP bmPark003;
extern const GUI_BITMAP bmnew01;
extern const GUI_BITMAP bmnew02;
extern const GUI_BITMAP bmnew03;
extern const GUI_BITMAP bmnew04;
extern const GUI_BITMAP bmnew05;
extern const GUI_BITMAP bmnew06;
extern const GUI_BITMAP bmnew07;
extern const GUI_BITMAP bmnew08;
extern const GUI_BITMAP bmnew09;
extern const GUI_BITMAP bmnew10;
extern const GUI_BITMAP bmnew11;
extern const GUI_BITMAP bmnew12;
extern const GUI_BITMAP bmnew13;
extern const GUI_BITMAP bmnew14;
extern const GUI_BITMAP bmnew15;
extern const GUI_BITMAP bmnew16;
extern const GUI_BITMAP bmnew17;
extern const GUI_BITMAP bmnew18;
extern const GUI_BITMAP bmnew19;
extern const GUI_BITMAP bmnew88;
extern const GUI_BITMAP bmnew89;
extern const GUI_BITMAP bmnew90;
extern const GUI_BITMAP bmnew91;
extern const GUI_BITMAP bmnew92;
extern const GUI_BITMAP bmnew93;
extern const GUI_BITMAP bmnew94;
extern const GUI_BITMAP bmnew95;
extern const GUI_BITMAP bmnew97;
extern const GUI_BITMAP bmnew98;
extern const GUI_BITMAP bmnew99;
extern const GUI_BITMAP bmnew100;
extern const GUI_BITMAP bmnew101;
extern const GUI_BITMAP bmnew102;
extern const GUI_BITMAP bm0000;
extern const GUI_BITMAP bmstr;


PROGBAR_Handle hProgBar;


OS_STK  MainTaskStk[MainTaskStkLength];
OS_STK  DisplayTaskStk[TaskDisplayStkLength];
OS_STK  WirelessReciveTaskStk[TaskWirelessReciveStkLength];
OS_STK  GUITaskStk[TaskGUIStkLength];



OS_EVENT *CarSem;





/************************************************
函数名:     Main
函数描述:   系统主函数,在汇编执行完后，
            调用的第一个文件，是系统入口
入参:		无
返回值:		0,初始化完毕
************************************************/

int Main(int argc, char **argv) 
{
	
	//初始化目标板
	TargetInit(); 
	
	//初始化uC/OS   
   	OSInit ();	 
  
   	//初始化系统时基
   	OSTimeSet(0);
 
   	//创建系统初始任务
   	OSTaskCreate (MainTask,(void *)0, &MainTaskStk[MainTaskStkLength - 1], MainTaskPrio);																										

	/*启动系统*/
	OSStart ();
	
	return 0;
}


/************************************************
函数名:     MainTask
函数描述:  
************************************************/

void MainTask(void *pdata) 
{
	INT32U i;

   	#if OS_CRITICAL_METHOD == 3
   		OS_CPU_SR  cpu_sr;
   	#endif
   	
  	OS_ENTER_CRITICAL(); 
  	Timer0Init();			/*initial timer0 for ucos time tick					*/
	ISRInit();   			/*initial interrupt prio or enable or disable		*/
	OS_EXIT_CRITICAL();
	
	Lcd_N35_Init();
	OSPrintfInit();			/*use task to print massage to Uart					*/ 
	OSStatInit();
	
	Uart_Init(0,115200);
	Uart_Select(2);

         GUI_Init();	
	
	GUI_DrawBitmap(&bmstr,0,0);
	GUI_SetFont(&GUI_FontHZ12);
	GUI_DispStringAt("系统启动中",130,145);
	GUI_SetFont(&GUI_FontHZ12);
//	GUI_DispStringAt("物联闪电小组出品",80,300);
	hProgBar = PROGBAR_Create(20,160,280,20,WM_CF_SHOW);
	PROGBAR_SetBarColor(hProgBar,0,GUI_GREEN);
	PROGBAR_SetBarColor(hProgBar,1,GUI_RED);
	for(i=0;i<=100;i++)
	{
		PROGBAR_SetValue(hProgBar,i);
		GUI_Delay(1);
	}
	PROGBAR_Delete(hProgBar);
	Beep(3000,500);
//	OSTimeDly(10000);

	CarSem = OSSemCreate(0);
        OSTaskCreate (TaskDisplay,(void *)0, &DisplayTaskStk[TaskDisplayStkLength- 1], TaskDisplayPrio);
	OSTaskCreate (TaskWirelessRecive,(void *)0, &WirelessReciveTaskStk[TaskWirelessReciveStkLength- 1], TaskWirelessRecivePrio);
	OSTaskCreate (TaskGUI,(void *)0, &GUITaskStk[TaskGUIStkLength- 1], TaskGUIPrio);
	
	while(1)
	{	
		OSTaskDel(MainTaskPrio);
	}

}

void    TaskDisplay(void *pdata)
{
	INT32U accountNum=0,i,j;
	INT8U errSem;
	GUI_Init();
	while(1)
	{
//		Uart_SendString("TaskDisplay Come in\n");
		GUI_DrawBitmap(&bmnew01,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew02,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew03,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew04,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew05,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew06,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew07,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew08,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew09,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew10,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew11,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew12,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew13,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew14,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew15,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew16,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew17,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew18,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew19,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bm0000,0,0);
		OSTimeDly(200);
		GUI_DrawBitmap(&bmnew88,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew89,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew90,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew91,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew92,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew93,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew94,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew95,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew97,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew98,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew99,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew100,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew101,0,0);
		OSTimeDly(150);
		GUI_DrawBitmap(&bmnew102,0,0);
		OSTimeDly(150);
		for(j=0;j<50;j++)
		{
			GUI_DrawBitmap(&bmPark003,0,0);
//			OSTimeDly(100);

			OSSemPend(CarSem,100,errSem);
			if(OS_NO_ERR == errSem)
			{
				CarNum=0;
				for(i=0;i<15;i++)//画小车
				{
					if('1' == CarInfo[i])
					{
						GUI_DrawBitmap(&bmCar000,CarPix[i][0],CarPix[i][1]);
						CarNum++;
						OSTimeDly(10);
						
					}
					Lcd_printf(85,5,0xffff,0x0000,0,"当前剩余车位数：%d",15-CarNum);
				}
				
				OSTimeDly(1000);
			}
			
		}

		OSTimeDly(2);
	}

}

void    TaskGUI(void *pdata)
{
	while(1)
	{

		GUI_Exec();
		GUI_Exec1();
		OSTimeDly(2);
	}

}

void    TaskWirelessRecive(void *pdata)
{
	INT8U ch;
	
	Uart_Init(0,115200);
	Uart_Select(2);

	while(1)
	{
		ch=Uart_Getch();
		if(ch == '#')
		{
			ch=Uart_Getch();
			CarSave[0]=ch;
			
			Lcd_printf(0,25,0xffff,0x0000,0,"当前：%d",ch);
			Delay(100);
			
			ch=Uart_Getch();
			CarSave[1]=ch;
			
			Lcd_printf(0,45,0xffff,0x0000,0,"当前：%d",ch);
			Delay(100);
			ch=Uart_Getch();
			CarSave[2]=ch;
			
			Lcd_printf(0,65,0xffff,0x0000,0,"当前：%d",ch);
			Delay(100);
			if(CarInfo[5]==CarSave[0] && CarInfo[6]==CarSave[1] && CarInfo[7]==CarSave[2])
			{
				;
			}
			else
			{
				CarInfo[5]=CarSave[0];
				CarInfo[6]=CarSave[1];
				CarInfo[7]=CarSave[2];
				OSSemPost(CarSem);
				Lcd_printf(0,85,0xffff,0x0000,0,"当前：%d%d%d",CarInfo[5],CarInfo[6],CarInfo[7]);
				Delay(100);
			}
		
		}
   		OSTimeDly(20);

	}

}
