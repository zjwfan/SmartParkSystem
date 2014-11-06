/*//////////////////////////////////////////////////////////////
define the Tasks' Stk lengh Prio Function on the project
by: tangxiaofeng xidian 503
////////////////////////////////////////////////////////////////
*/
#ifndef APP_CFG_H
#define APP_CFG_H


//Task Stk lengh 

#define	MainTaskStkLength	   1024   	 
#define	TaskDisplayStkLength	   1024*2
#define	TaskWirelessReciveStkLength	   1024*2
#define	TaskGUIStkLength	   1024*2
#define	TaskUartStkLength	   1024*2



//Task function
void    MainTask(void *pdata); 

void    TaskGUI(void *pdata);
void    TaskDisplay(void *pdata); 
void    TaskWirelessRecive(void *pdata);
void    TaskUart(void *pdata); 


//Task Prio
#define NormalTaskPrio       10

#define	TaskDisplayPrio NormalTaskPrio //10
#define	TaskWirelessRecivePrio NormalTaskPrio+2 //10
#define	TaskGUIPrio NormalTaskPrio+1 //10
//#define	TaskSocketPrio    NormalTaskPrio+1   //11
#define   TaskUartPrio        NormalTaskPrio+3  //12
#define    MainTaskPrio 	 NormalTaskPrio+4   //13


#endif