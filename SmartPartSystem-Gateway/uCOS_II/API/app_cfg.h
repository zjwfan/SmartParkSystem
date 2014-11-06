/*//////////////////////////////////////////////////////////////
define the Tasks' Stk lengh Prio Function on the project
by: tangxiaofeng xidian 503
////////////////////////////////////////////////////////////////
*/
#ifndef APP_CFG_H
#define APP_CFG_H


//Task Stk lengh 

#define	MainTaskStkLength	   		   1024   	 
#define	TaskDisplayStkLength	  	   1024*2
#define	TaskCarInfoCheckStkLength	   1024*2
#define	TaskWirelessSendStkLength	   1024*2
#define	TaskGUIStkLength	  		   1024*2
#define	TaskRFRecogniteStkLength	   1024*2
#define	TaskMotorDriveStkLength	  	   1024*2
#define	TaskPCCommunicateStkLength	   1024*2

#define	TaskUartStkLength	           1024*2

//Task function
void    MainTask(void *pdata); 
void    TaskRFRecognite(void *pdata);
void    TaskCarInfoCheck(void *pdata);
void    TaskGUI(void *pdata);
void    TaskDisplay(void *pdata); 
void    TaskWirelessSend(void *pdata);
void    TaskMotorDrive(void *pdata);
void    TaskPCCommunicate(void *pdata);

void    TaskUart(void *pdata);

//Task Prio
#define NormalTaskPrio      		 10
/*
#define	TaskRFRecognitePrio		 NormalTaskPrio+5 
#define TaskPCCommunicatePrio    NormalTaskPrio+2 
#define	TaskCarInfoCheckPrio 	 NormalTaskPrio+4
#define	TaskDisplayPrio			 NormalTaskPrio+3 
#define	TaskMotorDrivePrio 		 NormalTaskPrio+1 
#define	TaskWirelessSendPrio	 NormalTaskPrio+6
*/
#define	TaskRFRecognitePrio		 NormalTaskPrio+4 
#define TaskPCCommunicatePrio    NormalTaskPrio+5 
#define	TaskCarInfoCheckPrio 	 NormalTaskPrio+3
#define	TaskDisplayPrio			 NormalTaskPrio+1 
#define	TaskMotorDrivePrio 		 NormalTaskPrio+2 
#define	TaskWirelessSendPrio	 NormalTaskPrio+6
/*
#define	TaskRFRecognitePrio		 NormalTaskPrio+4 
#define TaskPCCommunicatePrio    NormalTaskPrio+5 
#define	TaskCarInfoCheckPrio 	 NormalTaskPrio+3
#define	TaskDisplayPrio			 NormalTaskPrio+1 
#define	TaskMotorDrivePrio 		 NormalTaskPrio+2 
#define	TaskWirelessSendPrio	 NormalTaskPrio+6
*/

#define MainTaskPrio 			 NormalTaskPrio+7 
#define	TaskGUIPrio				 NormalTaskPrio+8

#define TaskUartPrio     	     NormalTaskPrio+10  //12
	
#endif