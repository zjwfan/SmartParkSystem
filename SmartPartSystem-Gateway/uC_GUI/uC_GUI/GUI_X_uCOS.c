
#include <stdio.h>
#include "gui_x.h"

#include "uCOS_II.h"
#include "GUI.H"
#include "stdio.H"
	

static  OS_EVENT  *DispSem;

static  int        KeyPressed;
static  char       KeyIsInited;
static  OS_EVENT  *KeySem;


/*
*********************************************************************************************************
*                                        TIMING FUNCTIONS
*
* Notes: Some timing dependent routines of uC/GUI require a GetTime and delay funtion. 
*        Default time unit (tick), normally is 1 ms.
*********************************************************************************************************
*/

int  GUI_X_GetTime (void) //实现结合函数；
{
    return ((int)OSTimeGet());
}


void  GUI_X_Delay (int period) 
{
    INT32U  ticks;

	ticks=period;
    //ticks = (period * 1000) / OS_TICKS_PER_SEC;
    OSTimeDly(ticks);
}


/*
*********************************************************************************************************
*                                    MU
LTITASKING INTERFACE FUNCTIONS
*
* Note(1): 1) The following routines are required only if uC/GUI is used in a true multi task environment, 
*             which means you have more than one thread using the uC/GUI API.  In this case the #define 
*             GUI_OS 1   needs to be in GUIConf.h
*********************************************************************************************************
*/

void  GUI_X_InitOS (void)
{ 
    DispSem = OSSemCreate(1);
}


void  GUI_X_Lock (void)
{ 
    INT8U  err;
    
    
    OSSemPend(DispSem, 0, &err); 
}


void  GUI_X_Unlock (void)
{ 
    OSSemPost(DispSem);
}

//INT32U  GUI_X_GetTaskId (void) 
unsigned int  GUI_X_GetTaskId (void) 
{ 
    //return ((INT32U)(OSTCBCur->OSTCBPrio));
    return ((unsigned int)(OSTCBCur->OSTCBPrio));
}


/*
*********************************************************************************************************
*                                      KEYBOARD INTERFACE FUNCTIONS
*
* Purpose: The keyboard routines are required only by some widgets.
*          If widgets are not used, they may be eliminated.
*
* Note(s): If uC/OS-II is used, characters typed into the log window will be placed	in the keyboard buffer. 
*          This is a neat feature which allows you to operate your target system without having to use or 
*          even to have a keyboard connected to it. (useful for demos !)
*********************************************************************************************************
*/
#define FALSE 0
#define TRUE  1
static  void  CheckInit (void) 
{
    if (KeyIsInited == FALSE) {
        KeyIsInited = TRUE;
        GUI_X_Init();
    }
}


void GUI_X_Init (void) 
{
    KeySem = OSSemCreate(0);
}


int  GUI_X_GetKey (void) 
{
    int r;


    r          = KeyPressed;
    CheckInit();
    KeyPressed = 0;
    return (r);
}


int  GUI_X_WaitKey (void) 
{
    int    r;
    INT8U  err;


    CheckInit();
    if (KeyPressed == 0) {
        OSSemPend(KeySem, 0, &err);
    }
    r          = KeyPressed;
    KeyPressed = 0;
    return (r);
}


void  GUI_X_StoreKey (int k) 
{
    KeyPressed = k;
    OSSemPost(KeySem);
}

extern int TX,TY;

int  GUI_TOUCH_X_MeasureX(void) 
{
		 return TX;
	 
}  
int  GUI_TOUCH_X_MeasureY(void)
{
	     return TY;
	
}

void GUI_X_Log(const char *s){ }

void GUI_X_Warn(const char *s){}

void GUI_X_ErrorOut(const char *s){}

void GUI_X_ExecIdle(void) {}