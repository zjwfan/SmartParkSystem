#include "gui.h"
#include "cartoon.h"
#include "config.h"






void start()
{
	char i;
	char j;
	GUI_Init();
	openCartoon();
	GUI_Delay(50);

	Beep(2000,200);

}

