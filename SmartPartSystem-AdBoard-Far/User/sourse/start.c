#include "gui.h"
#include "cartoon.h"
#include "config.h"
#include "progbar.h"

PROGBAR_Handle hProgBar;
void start()
{
	char i;
	GUI_Init();
	openCartoon();
	GUI_Delay(100);
	
	hProgBar = PROGBAR_Create(70,220,180,15,WM_CF_SHOW);
	GUI_DispStringHCenterAt("正在进入控制中心",160,205);
	PROGBAR_SetBarColor(hProgBar,0,GUI_RED);
	PROGBAR_SetBarColor(hProgBar,1,GUI_BLACK);
	for(i=0;i<=100;i++)
	{
		PROGBAR_SetValue(hProgBar,i);
		GUI_Delay(1);
	}
	PROGBAR_Delete(hProgBar);
	Beep(1000,200);

}