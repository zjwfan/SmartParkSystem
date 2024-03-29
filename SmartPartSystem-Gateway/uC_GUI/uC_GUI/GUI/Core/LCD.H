/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              �C/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCD.H
Purpose     : Declares LCD interface functions
----------------------------------------------------------------------
*/

/*
  ********************************************************************
  *                                                                  *
  *                   Basic type defines                             *
  *                                                                  *
  ********************************************************************

The follwing are defines for types used in the LCD-driver and the
emWin layers on top of that. Since "C" does not provide data types of
fixed length which are identical on all platforms, this is done here.
For most 16/32 controllers, the settings will work fine. However, if
you have similar defines in other sections of your program, you might
want to change or relocate these defines, e.g. in a TYPE.H file.
*/
#ifndef __DEF_H__H
#define __DEF_H__H


#define I8    signed char
#define U8  unsigned char     /* unsigned 8  bits. */
#define I16   signed short    /*   signed 16 bits. */
#define U16 unsigned short    /* unsigned 16 bits. */
#define I32   signed long   /*   signed 32 bits. */
#define U32 unsigned int   /* unsigned 32 bits. */
#define I16P I16              /*   signed 16 bits OR MORE ! */
#define U16P U16              /* unsigned 16 bits OR MORE ! */


#define S32 int
#define S16 short int

#define	S8  char

#define TRUE 	1   
#define FALSE 	0
#define OK		1
#define FAIL	0

#endif



#ifndef LCD_H
#define LCD_H



/*
  ********************************************************************
  *                                                                  *
  *               Settings for windows simulation                    *
  *                                                                  *
  ********************************************************************

Some settings in the configuration may conflict with the values required
in the Simulation. This is why we ignore the target settings for data
types and use the correct settings for the simulation.
(U32 could be defined as long, which would yield a 64 bit type on
the PC)
*/

#ifdef WIN32
  #pragma warning( disable : 4244 )  // Disable warning messages in simulation
  #pragma warning( disable : 4761 )  // Disable warning "integral size mismatch in argument; conversion supplied"
#endif                                      


/*      *************************************************************
        *                                                           *
        *                Constants                                  *
        *                                                           *
        *************************************************************
*/
#define LCD_ERR0 (0x10)
#define LCD_ERR_CONTROLLER_NOT_FOUND (LCD_ERR0+1)
#define LCD_ERR_MEMORY               (LCD_ERR0+2)

/*
      *********************************
      *                               *
      *      Drawing modes            *
      *                               *
      *********************************
*/

#define LCD_DRAWMODE_NORMAL (0)
#define LCD_DRAWMODE_XOR    (1<<0)
#define LCD_DRAWMODE_TRANS  (1<<1)
#define LCD_DRAWMODE_REV    (1<<2)


 
/*      *************************************************************
        *                                                           *
        *                Typedefs                                   *
        *                                                           *
        *************************************************************
*/

typedef int LCD_DRAWMODE;
typedef U32 LCD_COLOR;

 
/********************************************************
*
*     Data structures
*
*********************************************************
*/

typedef struct { I16P x,y; } GUI_POINT;
typedef struct { I16 x0,y0,x1,y1; } LCD_RECT;
/*typedef struct { GUI_POINT P0, P1; } LCD_RECT; */

typedef struct {
  int              NumEntries; 
  char             HasTrans;         
  const LCD_COLOR* pPalEntries; 
} LCD_LOGPALETTE; 

/* This is used for the simulation only ! */
typedef struct {
  int x,y;
  unsigned char KeyStat;
} LCD_tMouseState;

/********************************************************
*
*     Index2Color
*
*********************************************************

  This function needs to be int the public part of the software
  since it is needed by the simulation. Most other driver
  functions are hidden in the private header file.
*/

typedef LCD_COLOR    tLCDDEV_Index2Color     (int Index);
#if !defined (__C51__) /* To bypass Keil-compiler bug */
  tLCDDEV_Index2Color LCD_L0_Index2Color, LCD_L0_1_Index2Color;
#else
  LCD_COLOR    LCD_L0_Index2Color  (int Index);
  LCD_COLOR    LCD_L0_1_Index2Color(int Index);
#endif

/*      *************************************************************
        *                                                           *
        *                   Defines                                 *
        *                                                           *
        *    for device capabilities                                *
        *                                                           *
        *************************************************************

The following is the list of device capabilities which can, but do
not have to be implemented in the driver. This way the driver can be
enhanced in the future without affecting the driver interface,
keeping older drivers compatible.
More DevCaps can always be added in the future, as older drivers
are guaranteed to return 0 for all unimplemented features or queries.

The values below define the legal parameters to the LCD_GetDeviceCaps
and the LCD_GetpCapFunc routines.
*/

#define LCD_DEVCAP_NUMCOLORS    0x0     /* Quest number of colors
                                           which LCD can display */
#define LCD_DEVCAP_XSIZE        0x1     /* Quest horiz. res. of display */
#define LCD_DEVCAP_YSIZE        0x2     /* Quest vert. res. of display */
#define LCD_DEVCAP_VXSIZE       0x3     /* Quest vert. res. of virtual disp.*/
#define LCD_DEVCAP_VYSIZE       0x4     /* Quest vert. res. of virtual disp.*/
#define LCD_DEVCAP_XORG         0x5     /* X-origin ... usually 0 */
#define LCD_DEVCAP_YORG         0x6     /* Y-origin ... usually 0 */
#define LCD_DEVCAP_CONTROLLER   0x7     /* LCD Controller (Numerical) */
#define LCD_DEVCAP_BITSPERPIXEL 0x8     /* Bits per pixel ... 1/2/4/8 */
#define LCD_DEVCAP_NUMPAGES     0x10    /* Quest number of pages of display */

#define LCD_DEVCAP_COLOR        0x1000  /* Quest Color[0]
                                0x1001     Quest Color[1]
                                0x1002     Quest Color[2]
                                         ...                    */
 /* The next 4095 entries are reserved so up to 4096 colors
    can be queried          ...*/

int LCD_GetXSize(void);
int LCD_GetYSize(void);
int LCD_GetVXSize(void);
int LCD_GetVYSize(void);
U32 LCD_GetNumColors(void);
int LCD_GetBitsPerPixel(void);
int LCD_GetFixedPalette(void);
int LCD_GetXMag(void);
int LCD_GetYMag(void);
int LCD_GetNumDisplays(void);
int LCD_GetXSize_1(void);
int LCD_GetYSize_1(void);
int LCD_GetVXSize_1(void);
int LCD_GetVYSize_1(void);
U32 LCD_GetNumColors_1(void);
int LCD_GetBitsPerPixel_1(void);
int LCD_GetFixedPalette_1(void);
int LCD_GetXMag_1(void);
int LCD_GetYMag_1(void);

#define LCD_GET_XSIZE()        LCD_GetXSize()
#define LCD_GET_YSIZE()        LCD_GetYSize()
#define LCD_GET_VXSIZE()       LCD_GetVXSize()
#define LCD_GET_VYSIZE()       LCD_GetVYSize()
#define LCD_GET_NUMCOLORS()    LCD_GetNumColors()
#define LCD_GET_BITSPERPIXEL() LCD_GetBitsPerPixel()


/*********************************************************************
*
*      LCD_CLIP function table
*
**********************************************************************
*/
typedef void         tLCD_HL_DrawHLine    (int x0, int y0,  int x1);
typedef void         tLCD_HL_DrawPixel    (int x0, int y0);

typedef struct {
  tLCD_HL_DrawHLine*          pfDrawHLine;
  tLCD_HL_DrawPixel*          pfDrawPixel;
} tLCD_HL_APIList;

#if defined (__C51__) /* To bypass Keil-compiler bug */
  void LCD_DrawHLine(int x0, int y0,  int x1);
  void LCD_DrawPixel(int x0, int y0);
#else
  tLCD_HL_DrawHLine LCD_DrawHLine;
  tLCD_HL_DrawPixel LCD_DrawPixel;
#endif

void LCD_DrawVLine  (int x, int y0,  int y1);



/*********************************************************************
*
*              Declarations for LCD_
*
**********************************************************************
*/

void LCD_SetClipRectEx(const LCD_RECT* pRect);
void LCD_SetClipRectMax(void);

/* Get device capabilities (0 if not supported) */
I32  LCD_GetDevCap(int Index);

/* Initialize LCD using config-paramters */
int LCD_Init(void);

void LCD_SetBkColor   (LCD_COLOR Color); /* Set background color */
void LCD_SetColor     (LCD_COLOR Color); /* Set foreground color */
void LCD_SetPixelIndex(int x, int y, int ColorIndex);
void LCD_L0_DrawHLine  (int x0, int y,  int x1);
void LCD_L0_DrawVLine  (int x, int y0,  int y1);
/* Palette routines (Not available on all drivers) */
void LCD_InitLUT(void);

/* Decompressors */
typedef void tfDrawSpecialBitmap(int x0,int y0,int xsize, int ysize, const U8*pPixel, const LCD_LOGPALETTE* pLogPal, int xMag, int yMag);
#if !defined (__C51__) /* To bypass Keil-compiler bug */
  tfDrawSpecialBitmap LCD_DrawBitmap_RLE8, LCD_DrawBitmap_RLE4;
#else
  void LCD_DrawBitmap_RLE8(int x0,int y0,int xsize, int ysize, const U8*pPixel, const LCD_LOGPALETTE* pLogPal, int xMag, int yMag);
  void LCD_DrawBitmap_RLE4(int x0,int y0,int xsize, int ysize, const U8*pPixel, const LCD_LOGPALETTE* pLogPal, int xMag, int yMag);
#endif

LCD_DRAWMODE LCD_SetDrawMode  (LCD_DRAWMODE dm);
void LCD_SetColorIndex(int Index);
void LCD_SetBkColorIndex(int Index);
void LCD_FillRect(int x0, int y0, int x1, int y1);
void LCD_DrawBitmap   (int x0, int y0,
                       int xsize, int ysize,
                       int xMul, int yMul,
                       int BitsPerPixel,
                       int BytesPerLine,
                       const U8* pPixel,
                       const LCD_LOGPALETTE* pLogPal);
typedef void tLCD_SetPixelAA(int x, int y, U8 Intens);

#if defined (__C51__)  /* To bypass Keil-compiler bug */
  void tLCD_SetPixelAA(int x, int y, U8 Intens);
  void LCD_SetPixelAA_NoTrans(int x, int y, U8 Intens);
#else
  tLCD_SetPixelAA LCD_SetPixelAA;
  tLCD_SetPixelAA LCD_SetPixelAA_NoTrans;
#endif

LCD_COLOR LCD_AA_MixColors(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens);
LCD_COLOR    LCD_GetPixelColor(int x, int y);     /* Get RGB color of pixel */
unsigned int LCD_GetPixelIndex(int x, int y);
int LCD_GetBkColorIndex (void);
int LCD_GetColorIndex (void);



/*      *************************************************************
        *                                                           *
        *      LCD  publics not used by GUI                         *
        *                                                           *
        *************************************************************
  

The following functions can, but do not have to be defined in the LCD driver
as they are not used by the GUI level.

*/



#define LCD_On          LCD_L0_On
#define LCD_Off         LCD_L0_Off
#define LCD_ReInit      LCD_L0_ReInit
#define LCD_SetLUTEntry LCD_L0_SetLUTEntry


#define LCD_CC_UNLOCK (0)    /* Default mode: Cache is transparent */
#define LCD_CC_LOCK   (1)    /* Cache is locked, no write operations */
#define LCD_CC_FLUSH  (2)    /* Flush cache, do not change mode */
U8 LCD_L0_ControlCache(U8 mode);
/* Check if controller is still properly initialized */
int  LCD_L0_CheckInit(void);        /* returns if init is still O.K.
                                      by reading all available registers of
                                      Display controller.
                                      0: Init O.K.
                                    */

#define LCD_ControlCache LCD_L0_ControlCache

/*
        ******************************************
        *                                        *
        *        Support for paging              *
        *                                        *
        ******************************************
*/

int LCD_SelPage  (int NewPage);    /* returns formerly sel. page */
int LCD_ShowPage (int NewPage);    /* sets / returns visible page */
int LCD_GetSelPage (void);         /* returns selected page */
int LCD_GetVisPage (void);         /* returns visible page */


/*      *************************************************************
        *                                                           *
        *      LCD  imports                                         *
        *                  (for routines in LCDColor)               *
        *                                                           *
        *************************************************************
*/
  

int              LCD_Color2Index     (LCD_COLOR Color);
LCD_COLOR        LCD_Index2Color     (int Index);

/*********************************************************************
*
*       LCD_X_...
*
**********************************************************************
*/

void LCD_X_Init(void);
void LCD_X_On  (void);
void LCD_X_Off (void);

#endif /* LCD_H */




