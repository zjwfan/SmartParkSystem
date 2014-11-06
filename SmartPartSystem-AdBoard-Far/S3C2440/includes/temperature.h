#ifndef temperature_H
#define temperature_H

#include "config.h"

void Init_DS18B20(void);
int ReadOneChar(void);
void WriteOneChar(U8);
int ReadTemperature(void);
U32 get_t(void);

#endif