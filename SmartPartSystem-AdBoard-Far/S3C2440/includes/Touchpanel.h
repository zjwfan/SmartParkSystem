/*****************************************
  NAME: Touchpanel.h
  DESC: ADC & Touch screen test
  HISTORY:
  2003.09.23:Leon YH KIM: draft ver 1.0
 *****************************************/
#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

#include "Def1.h"
#include "2440addr.h"
#include "2440lib.h"

//extern  AdButton;
//extern  GUI_ID_UP;
#define REQCNT 30
#define ADCPRS 9	//YH 0627
#define LOOP 1

void TouchpanelInit(void);
void Touch_Screen_Init(void);
void AdcTsAuto(void);




#endif
