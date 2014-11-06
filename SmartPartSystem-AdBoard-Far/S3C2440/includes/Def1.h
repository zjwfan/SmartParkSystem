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


//#define ESC_KEY	 0x1b
#endif /*__DEF_H__*/

