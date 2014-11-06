/****************************************Copyright (c)**************************************************
**                               Iky Embeddey System Institute
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:          Adc.c
** Last modified Date: 2011-06-06 
** Last Version:       v1.0
** Descriptions:       A/Dת������
** 
**------------------------------------------------------------------------------------------------------
** Modified by:        xiexy
** Modified date:      2011-06-06
** Version:            v1.0
** Descriptions:       ����
**
**------------------------------------------------------------------------------------------------------
** Modified by:     
** Modified date:     
** Version:           
** Descriptions:      
**
********************************************************************************************************/
#include "config.h"

/*****************************************************************************
** Function name: ReadAdc
** Descriptions:  ��ȡ����ch��ָ����ͨ����ת��Ϊ�����źź��ģ���ź���ֵ
**				  
** Input:         int ch: A/Dת��ͨ���� 
** Output:        �ɼ���������
** Created by:    xiexy
** Created Date:  2011-06-06
**----------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**----------------------------------------------------------------------------------
******************************************************************************/	
int ReadAdc(int ch)
{
    int i,result;
	volatile U32 preScaler;  
  	U32 rADCCON_save = rADCCON;
  	
    preScaler = 50000000/ADC_FREQ -1;               //PCLK:50.7MHz
   	rADCCON = (1<<14)|(preScaler<<6)|(ch<<3);   	//setup channel
	Delay(200);										//delay to set up the next channel
    rADCCON|=0x1;   								//start ADC

    while(rADCCON & 0x1);							//check if Enable_start is low
    while(!(rADCCON & 0x8000));						//check if EC(End of Conversion) flag is high

    result = (int)rADCDAT0 & 0x3ff;
    rADCCON = rADCCON_save;
    
    return (result);
}

