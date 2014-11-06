/************************************************
文件名:      Main.c
文件描述:    系统主文件
创建时间:    2014-07-10
作者：       物联闪电小组
************************************************/
//#define DEBUG 

#include "config.h"


INT8U CarInfo[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'\0'};
INT8U CarSave[3]={'#','#','#'};
INT8U CarOld[3]={'#','#','#'};
INT32U CarPix[15][2]={{96,32},{144,32},{187,32},{230,32},{274,32},
{72,102},{122,102},{172,102},{220,103},
{23,183},{73,183},{123,183},{174,183},{222,183},{273,183}};
char UartRFIDRecvData[64];
INT32U CarNum=0,flag=0;
PROGBAR_Handle hProgBar;

/*******************声明外部函数**********************/

extern void motor_back (int q);

extern void uart0_init(int pclk, int baud);
extern void uart0_sendbyte(unsigned char data);
extern void uart0_sendstring(unsigned char *pt);
extern void uart0_printf(char *fmt,...);
extern char uart0_getbyte(void);

extern void uart1_init(int pclk, int baud);
extern void uart1_sendbyte(unsigned char data);
extern void uart1_sendstring(unsigned char *pt);
extern void uart1_printf(char *fmt,...);
extern char uart1_getbyte(void);

extern void uart2_init(int pclk, int baud);
extern void uart2_sendbyte(unsigned char data);
extern void uart2_sendstring(unsigned char *pt);
extern void uart2_printf(char *fmt,...);
extern char uart2_getbyte(void);

extern const GUI_BITMAP bmstr;
extern const GUI_BITMAP bmwel00;
extern const GUI_BITMAP bmPark003;
extern const GUI_BITMAP bmCar000;

/******************任务栈的定义******************/
OS_STK  MainTaskStk[MainTaskStkLength];
OS_STK  DisplayTaskStk[TaskDisplayStkLength];
OS_STK  WirelessSendTaskStk[TaskWirelessSendStkLength];
OS_STK  GUITaskStk[TaskGUIStkLength];
OS_STK  CarInfoCheckTaskStk[TaskCarInfoCheckStkLength];
OS_STK  RFRecogniteTaskStk[TaskRFRecogniteStkLength];
OS_STK  MotorDriveTaskStk[TaskMotorDriveStkLength];
OS_STK  PCCommunicateTaskStk[TaskPCCommunicateStkLength];

/******************事件控制块的定义******************/

OS_EVENT *RFIDMbox;
OS_EVENT *CarSem;
OS_EVENT *WireSem;
OS_EVENT *RFSem;
OS_EVENT *UartRFIDSem;


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
	INT32U i=0;
	INT8U err;
	#if OS_CRITICAL_METHOD == 3
		OS_CPU_SR  cpu_sr;
	#endif
   	
	OS_ENTER_CRITICAL(); 
	Timer0Init();			/*initial timer0 for ucos time tick					*/
	ISRInit();   			/*initial interrupt prio or enable or disable		*/
	OS_EXIT_CRITICAL();
	
	//watchdog_disable();	 //关闭watchdog
	Lcd_N35_Init();
	//	OSPrintfInit();			/*use task to print massage to Uart					*/ 
	OSStatInit();
	
	uart2_init(PCLK, 115200);	//UART0初始化
	uart1_init(PCLK, 9600);	//UART0初始化
	uart0_init(PCLK, 115200);	//UART0初始化
	
	#ifdef DEBUG
	uart0_sendstring("uart0_init!\n");
	#endif
	
	GUI_Init();	
	
	//	Paint_Bmp(0,0,240,320,StartLogoData);
	GUI_DrawBitmap(&bmstr,0,0);
	GUI_SetFont(&GUI_FontHZ12);
	GUI_DispStringAt("系统启动中",130,145);
	hProgBar = PROGBAR_Create(20,160,280,20,WM_CF_SHOW);
	PROGBAR_SetBarColor(hProgBar,0,GUI_GREEN);
	PROGBAR_SetBarColor(hProgBar,1,GUI_RED);
	
	for(i=0;i<=100;i++)
	{
		PROGBAR_SetValue(hProgBar,i);
		GUI_Delay(2);
	}
	
	PROGBAR_Delete(hProgBar);
	Beep(3000,500);
	
	#ifdef DEBUG
	uart0_sendstring("Beep!\n");
	#endif
	
        RFIDMbox = OSMboxCreate((void*)0);
        CarSem = OSSemCreate(0);
        WireSem = OSSemCreate(0);
        RFSem = OSSemCreate(3);
	UartRFIDSem = OSSemCreate(0);
	
        OSTaskCreate (TaskDisplay,(void *)0, &DisplayTaskStk[TaskDisplayStkLength- 1], TaskDisplayPrio);
	OSTaskCreate (TaskWirelessSend,(void *)0, &WirelessSendTaskStk[TaskWirelessSendStkLength- 1], TaskWirelessSendPrio);
//	OSTaskCreate (TaskGUI,(void *)0, &GUITaskStk[TaskGUIStkLength- 1], TaskGUIPrio);
	OSTaskCreate (TaskCarInfoCheck,(void *)0, &CarInfoCheckTaskStk[TaskCarInfoCheckStkLength- 1], TaskCarInfoCheckPrio);
	OSTaskCreate (TaskRFRecognite,(void *)0, &RFRecogniteTaskStk[TaskRFRecogniteStkLength- 1], TaskRFRecognitePrio);
	OSTaskCreate (TaskMotorDrive,(void *)0, &MotorDriveTaskStk[TaskMotorDriveStkLength- 1], TaskMotorDrivePrio);
	OSTaskCreate (TaskPCCommunicate,(void *)0, &PCCommunicateTaskStk[TaskPCCommunicateStkLength- 1], TaskPCCommunicatePrio);
	

	OSTaskDel(MainTaskPrio);
	
}

void    TaskDisplay(void *pdata)
{
	INT32U i;
	INT8U errMbox,errSem;
	void *msg;
	GUI_Init();
	GUI_DrawBitmap(&bmPark003,0,0);//停车场车位图
	while(1)
	{
	
	#ifdef DEBUG
		uart0_sendstring("TaskDisplay\n");
	#endif
	
		OSSemPend(CarSem,100,errSem);
		if(OS_NO_ERR == errSem)
		{
			#ifdef DEBUG
			uart0_sendstring("TaskDisplay:OSSemPend\n");
			#endif
			
			GUI_DrawBitmap(&bmPark003,0,0);//停车场车位图
			CarNum=0;
			for(i=0;i<15;i++)//画小车
			{
				
				if(1 == CarInfo[i])
				{
					GUI_DrawBitmap(&bmCar000,CarPix[i][0],CarPix[i][1]);
					CarNum++;
					//OSTimeDly(10);
				}
				Lcd_printf(85,5,0xffff,0x0000,0,"当前剩余车位数：%d",15-CarNum);
			}
			
		}
		
		msg = OSMboxPend(RFIDMbox,1000,&errMbox);
		if(OS_NO_ERR == errMbox)
		{
			#ifdef DEBUG
			uart0_sendstring("TaskDisplay:OSMboxPend\n");
			#endif
			
			GUI_DrawBitmap(&bmwel00,0,0);  
			//			GUI_DispStringAt(msg,70,15);
			Lcd_printf(65,13,0xffff,0x0000,0,msg);
			OSTimeDly(5530);
			GUI_DrawBitmap(&bmPark003,0,0);//停车场车位图
			OSSemPost(RFSem);
		}
		OSTimeDly(50);
	}
	
}

void    TaskGUI(void *pdata)
{
	while(1)
	{
		#ifdef DEBUG
		uart0_sendstring("TaskGUI\n");	
		#endif
		
		GUI_Exec();	
		GUI_Exec1();
		OSTimeDly(200);
	}
	
}

void    TaskWirelessSend(void *pdata)
{
	INT8U errMutex,errWireSem;
	INT32U i=0;
	uart2_init(PCLK, 115200);
	while(1)
	{
		OSSemPend(WireSem,0,errWireSem);
		if(OS_NO_ERR == errWireSem)
		{
			#ifdef DEBUG
			uart0_sendstring("TaskWireless\n");
			
			uart0_sendstring("TaskWireless:Send start\n");
			#endif
			
			//uart2_init(PCLK, 115200);

			if(CarInfo[5]==CarSave[0] && CarInfo[6]==CarSave[1] && CarInfo[7]==CarSave[2] && flag>2)
			//if(0)
			{
				flag=0;
				OSTimeDly(2000);
			}
			else
			{
				uart2_printf("#%d%d%d\n",CarInfo[5],CarInfo[6],CarInfo[7]);
				
				flag ++;
				
				#ifdef DEBUG
				uart0_printf("#%d%d%d\n",CarInfo[5],CarInfo[6],CarInfo[7]);
				#endif
				
				CarSave[0]=CarInfo[5];
				CarSave[1]=CarInfo[6] ;
				CarSave[2]=CarInfo[7];
			}
			
		}
		#ifdef DEBUG
		uart0_sendstring("TaskWireless:Send stop\n");
		#endif
		
		OSTimeDly(20);
	}
}

void    TaskCarInfoCheck(void *pdata)
{
	INT8U i=1,flag=0;
	while(1)
	{
		#ifdef DEBUG
		uart0_sendstring("TaskCarInfoCheck\n");
		#endif
		
		CarOld[0]=CarInfo[5];
		CarOld[1]=CarInfo[6];
		CarOld[2]=CarInfo[7];
		
		for(i=0;i<15;i++)
		{
			CarInfo[i]=0;
		}
		
		rGPFCON&=0xc0ff;
		i=rGPFDAT&0x0010;
		if(i==0 && CarInfo[5]==0)
 		{
 			CarInfo[5]=1; 
 			flag=1;
 		}
 		
		i=rGPFDAT&0x0020;
		if(i==0 && CarInfo[6]==0)
 		{
 			CarInfo[6]=1; 
 			flag=1;
 		}

		i=rGPFDAT&0x0040;
		if(i==0 && CarInfo[7]==0)
 		{
 			CarInfo[7]=1; 
 			flag=1;
 		}
		
		if(1==flag)
 		{
 			if(CarInfo[5]==CarOld[0] &&CarInfo[6]==CarOld[1]  && CarInfo[7]==CarOld[2])
 			{
 				;
 			}
 			else
 			{
	 			flag=0;
	 			i=1;
	 			OSSemPost(CarSem);
	 			OSSemPost(WireSem);
	 			
	 			#ifdef DEBUG
	 			uart0_sendstring("TaskCarInfoCheck:OSSemPost\n");
	 			#endif
 			}
 		}    	
		
		OSTimeDly(2000);
	}

}


void    TaskRFRecognite(void *pdata)
{
	uint8 err;
	uart1_init(PCLK,9600);  
	Uart_INT();					// 初始化串口0的接收中断
	while(1)
	{
		#ifdef DEBUG
		uart0_sendstring("TaskRFRecongnite IN\n");
		#endif
		
		//RFmain();
		OSSemPend(UartRFIDSem,0,&err);		// 等待1个数据帧接收完成
		if(OS_NO_ERR == err)
	    	{
	    		 //num=strlen(UartRFIDRecvData);
	    		 //Lcd_printf(10,80,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,"从串口接收字符串为：%d",num);
	    		 // 将接收到的字符串在LCD上显示出来
	    		 //Lcd_printf(10,100,RGB( 0xFF,0xFF,0xFF),RGB( 0x00,0x00,0x00),0,UartRFIDRecvData);
	    		 OSMboxPostOpt(RFIDMbox,(void*)UartRFIDRecvData,OS_POST_OPT_BROADCAST);
	    	}
		#ifdef DEBUG
		uart0_sendstring("TaskRFRecongnite Come\n");
		#endif
		
		OSTimeDly(2);
	}
	
}

void    TaskMotorDrive(void *pdata)
{
	INT8U err;
	void *msg;
	char RF[10];
	while(1)
	{
		#ifdef DEBUG
		uart0_sendstring("TaskMotorDrive\n");
		#endif
		
		msg = (char *)OSMboxPend(RFIDMbox,0,&err);
		if(OS_NO_ERR == err)
		{
			#ifdef DEBUG
			uart0_sendstring("TaskMotorDrive:OSMboxPend\n");
			#endif
			
			strcpy(RF,msg);
			//Lcd_printf(5,180,0xffff,0x0000,0,"当前数：%c,%c",RF[2],RF[6]);
			if((RF[2]=='2' && RF[6] == '3')  || (RF[2]=='7' && RF[6] == '7') )
			{
				motor_forward (2);
				OSTimeDly(10000);
				motor_back (2);
				OSSemPost(RFSem);
			}
		}
		OSTimeDly(2);
	}
	
	
}

void    TaskPCCommunicate(void *pdata)
{
	char car,str[100]="str01";
	INT8U errMutex;
	INT8U err,i;
	void *msg;
	uart0_init(PCLK, 115200);	//UART0初始化
	while(1)
	{
		#ifdef DEBUG
		uart0_sendstring("TaskPCCommunicate!\n");
		#endif
		
		msg = OSMboxPend(RFIDMbox,0,&err);
		if(OS_NO_ERR == err)
		{
			#ifdef DEBUG
			uart0_sendstring("TaskPCCommunicate:OSMboxPend\n");
			#endif
			strcpy(str,"str0");
			strcat(str,msg);
			str[4]='1';
			strcat(str,"ner");			
			uart0_printf("%s\n",str);
			uart0_printf("%s\n",str);
			OSSemPost(RFSem);
		}
		OSTimeDly(200);
	}
	
}

