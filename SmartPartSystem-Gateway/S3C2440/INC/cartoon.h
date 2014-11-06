#include "w35lcd.h"

extern const GUI_BITMAP bmwel;
extern unsigned char logo1[];
extern unsigned char logo2[];
extern unsigned char logo3[];
extern unsigned char logo4[];
extern unsigned char logo5[];
extern unsigned char logo6[];
extern unsigned char logo7[];
extern unsigned char logo8[];
extern unsigned char logo9[];
extern unsigned char logo10[];
extern unsigned char logo11[];
extern unsigned char logo12[];
extern unsigned char logo20[];
extern unsigned char logo21[];
extern unsigned char logo22[];
extern unsigned char logo23[];
extern unsigned char logo24[];
extern unsigned char logo25[];
extern unsigned char logo26[];
extern unsigned char logo27[];

extern unsigned char readmee[];
extern unsigned char about[];
extern unsigned char control[];

extern unsigned char help[];
void openCartoon()
{
	Paint_Bmp(0,0,320,240,logo1);
	Paint_Bmp(0,0,320,240,logo1);
	Paint_Bmp(0,0,320,240,logo2);
	Paint_Bmp(0,0,320,240,logo2);
	Paint_Bmp(0,0,320,240,logo3);
	Paint_Bmp(0,0,320,240,logo3);
	Paint_Bmp(0,0,320,240,logo4);
	Paint_Bmp(0,0,320,240,logo4);
	Paint_Bmp(0,0,320,240,logo5);
	Paint_Bmp(0,0,320,240,logo5);
	Paint_Bmp(0,0,320,240,logo6);
    Paint_Bmp(0,0,320,240,logo6);
	Paint_Bmp(0,0,320,240,logo7);
	Paint_Bmp(0,0,320,240,logo7);
	Paint_Bmp(0,0,320,240,logo8);
	Paint_Bmp(0,0,320,240,logo8);
	Paint_Bmp(0,0,320,240,logo10);
	Paint_Bmp(0,0,320,240,logo10);
	Paint_Bmp(0,0,320,240,logo11);
	Paint_Bmp(0,0,320,240,logo11);
	Paint_Bmp(0,0,320,240,logo12);
	Paint_Bmp(0,0,320,240,logo12);
	Paint_Bmp(0,0,320,240,logo20);
	Paint_Bmp(0,0,320,240,logo20);
	Paint_Bmp(0,0,320,240,logo21);
	Paint_Bmp(0,0,320,240,logo21);
	Paint_Bmp(0,0,320,240,logo22);
	Paint_Bmp(0,0,320,240,logo22);
	Paint_Bmp(0,0,320,240,logo23);
	Paint_Bmp(0,0,320,240,logo23);
	Paint_Bmp(0,0,320,240,logo24);
	Paint_Bmp(0,0,320,240,logo24);
	Paint_Bmp(0,0,320,240,logo25);
	Paint_Bmp(0,0,320,240,logo25);
	Paint_Bmp(0,0,320,240,logo26);
	Paint_Bmp(0,0,320,240,logo26);
	Paint_Bmp(0,0,320,240,logo27);
	Paint_Bmp(0,0,320,240,logo27);
	GUI_DrawBitmap(&bmwel,0,0);


}
