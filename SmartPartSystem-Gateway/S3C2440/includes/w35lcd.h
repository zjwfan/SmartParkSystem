/**************************************************************
The initial and control for 240¡Á320 16Bpp TFT LCD----LtS350Q1
**************************************************************/
#ifndef _LCD_H_
#define _LCD_H_

#include "option.h"

void Lcd_N35_Init(void);
void Lcd_EnvidOnOff(int);
//extern unsigned char iphone_pic[];
//extern unsigned char osx_800x480[];
//extern unsigned char VGA_1024x768_DEMO[];
//extern unsigned char sunflower_640x480[];
void Lcd_ClearScr( U16 c);
void PutPixel(U32 x,U32 y, U16 c );
void GUI_Point(U32 x,U32 y, U16 c );
//void PutPixel(unsigned int x , unsigned int y , unsigned int c);
void Glib_Rectangle(int x1,int y1,int x2,int y2,int color);
//void Glib_Rectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Glib_FilledRectangle(int x1,int y1,int x2,int y2,int color);
//void Glib_FilledRectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Glib_Line(int x1,int y1,int x2,int y2,int color);
//void Glib_Line(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void Paint_Bmp(int x0,int y0,int h,int l,unsigned char bmp[]);

//================================
// ×ÖÌåÊä³ö
//================================
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st); 
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st);  

#endif
void Lcd_printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...); 


#define RGB(r,g,b)   (unsigned int)( (r << 16) + (g << 8) + b )
#define FROM_BCD(n)		((((n) >> 4) * 10) + ((n) & 0xf))
#define TO_BCD(n)		((((n) / 10) << 4) | ((n) % 10))
