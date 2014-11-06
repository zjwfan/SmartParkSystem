#include "config.h"

char strRF[20],ch;
extern unsigned int YsizeNumber;
extern OS_EVENT *RFIDMbox;
extern OS_EVENT *RFSem;

extern const GUI_BITMAP bmwel00;


void RFmain()
{
	INT32U i=0;
	INT8U errRFSem,errMutex;
	
	#ifdef DEBUG
	uart0_sendstring("RFmain enter\n");
	#endif	
	
	OSSemPend(RFSem,0,errRFSem);
	OSSemPend(RFSem,0,errRFSem);
	OSSemPend(RFSem,0,errRFSem);
	if(OS_NO_ERR == errRFSem)
	{
		#ifdef DEBUG
		uart0_sendstring("RFmain before change 9600\n");
		#endif
		
		//uart1_init(PCLK,9600);  
		
	    
		for(i=0;i<8;i++)
		{
			ch=uart1_getbyte();
			strRF[i]=ch;
		}

		strRF[i]='\0';
		
		
	    OSMboxPostOpt(RFIDMbox,(void*)strRF,OS_POST_OPT_BROADCAST);

	    #ifdef DEBUG
    	uart0_sendstring("TaskRF:OSMboxPostOpt\n");
    	#endif
    	
	}
	#ifdef DEBUG
	uart0_sendstring("RFmain exit\n");
	#endif
}








/*
INT8U  SerDTvalid;

*/
/***************************************************************************
#define RCAP2_50us             65536L - OSC_FREQ/240417L
#define RCAP2_1ms              65536L - OSC_FREQ/12021L
#define RCAP2_5ms              65536L - OSC_FREQ/2400L
#define RCAP2_10ms             65536L - OSC_FREQ/1200L

/****************************************************************************
#define ZLG500S_DELAY_1s		200
#define ZLG500S_DELAY_500ms		100
#define ZLG500S_DELAY_250ms		 50
#define ZLG500S_DELAY_100ms		 20
#define ZLG500S_DELAY_10ms		  2
#define ZLG500S_DELAY_bb_time		 30
#define ZLG500S_UP_POWER_TIME		2000
/****************************************************************************
#define FRAMELEN	0	  
#define CMDTYPE		1	
#define COMMAND		2	
#define STATUS		2	 
#define LENGTH          3	 
#define DATA            4	  
#define PRTCLEN		6	 
/****************************************************************************
#define STX     0x20
#define ETX     0x03    
#define ACK	0x06   
/****************************************************************************
////////////////////////////////////#define OK		0
#define COMMERR		1
#define SEQERR		2
#define BCCERR		4
/****************************************************************************
#define REQSTD		0x26
#define REQALL		0x52
#define SELCODE1	0x93
#define SELCODE2	0x95
#define SELCODE3	0x97
#define KEYA		0x60
#define KEYB		0x61
#define DECREMENT	0xc0
#define INCREMENT	0xc1
#define RESTORE		0xc2
#define ISO14443A	0x00
#define ISO14443B	0x80
/****************************************************************************
//sfr16   RCAP2LH = 0xCA;
//sfr16   T2LH    = 0xCC;
/****************************************************************************
// ISO14443 ����������
#define STATUS_ATS_ERROR			    0xCE	// ATS����
#define STATUS_PPS_ERROR  			    0xCD	// PPSS����
#define STATUS_BLOCK_FORMAT_ERROR       0xCC	// ����֡��ʽ���� 
#define STATUS_CID_ERROR  			    0xCB	// CID����
#define STATUS_PCB_ERROR				0xCA	// PCB����
#define STATUS_NAK_ERROR				0xC9	// NAK����
#define STATUS_LEN_ERROR				0xC8	// ���ȴ���
#define STATUS_SEND_ERROR				0xC7	// ���ʹ���
#define STATUS_APDU_ERROR				0xC6	// APDU�������
#define STATUS_PERORM_ERROR				0xC4	// ����ִ�н������
#define STATUS_INVALID_VALUE			0xC3	// ֵ���ʽ����
/****************************************************************************


#define MAXSERSIZE      31+6
INT8U SerBfr[MAXSERSIZE];	
INT8U ucBlockBuf[16];	
INT8U SerBfrIdx;

INT8U WaitRespDly=0;
	
static INT8U SEQ = 0xf0;	

INT8U DataBfr[16];
INT8U Card_Snr[4];

INT8U  KEYA_BUF[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
INT8U  KEYB_BUF[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

INT8U  DataWriteBuf[16];
INT8U  DataReadBuf[16];

INT8U  ucLedShowBuf[4];
//===========================================================================
void memcopy(INT8U *DBfr,INT8U *SBfr,INT8U Len)
{
	INT8U i;
	for(i=0;i<Len;i++)
	{
		DBfr[i] = SBfr[i];
	}
}

char memcomp (void *s1, void *s2, int n)
{
	INT8U i;
	for(i=0;i<n;i++)
	{
		if (((char*)s1)[i] != ((char*)s2)[i]){
			return -1;	
		}
	}
	return 0;	
}


//===========================================================================
INT8U Ser_Rcv(void)
{
	INT8U i,BCC;
	INT8U Status = FALSE;

	if(SerDTvalid) {									
		if(SerBfr[SerBfr[FRAMELEN]-1] == ETX && SerBfr[FRAMELEN] == (SerBfr[LENGTH]+PRTCLEN)) {		
			BCC = 0;
			for(i=0; i<(SerBfr[FRAMELEN]-1); i++) {
				BCC ^= SerBfr[i];
			}
			if(BCC == 0xff) {
				Status = TRUE;
            }
		}
		SerDTvalid = FALSE;
	}
	return Status;
}
//===========================================================================
INT8U Ser_Send(void)
{
    INT8U i,BCC;

	SerBfr[FRAMELEN] = SerBfr[LENGTH]+PRTCLEN;	
	BCC = 0;
	for(i=0; i<(SerBfr[FRAMELEN]-2); i++) {
		BCC ^= SerBfr[i];
    }
	SerBfr[SerBfr[FRAMELEN]-2] = ~BCC;
	SerBfr[SerBfr[FRAMELEN]-1] = ETX;

	SerBfrIdx = FRAMELEN;
//	TI = 1;                                 // ���������ж�,dd���ж��з�������

	return TRUE;
}
//===========================================================================
INT8U ATS522_Cmd(void)
{
	INT8U Status;
	SEQ += 0x10;
	SerBfr[CMDTYPE] |= SEQ;		
	Status = Ser_Send();

	if(!Status) {
		return COMMERR;
    }

	WaitRespDly = ZLG500S_DELAY_500ms;	
	while(WaitRespDly){
		if(Ser_Rcv())
			break;
	}
	if(WaitRespDly == 0) {					
		Status = COMMERR;
	} else {
		if(SEQ == (SerBfr[CMDTYPE]&0xf0)) {				
			Status = 0;
		} else  {
			Status = SEQERR;
        }
	}
	return Status;
}

//===========================================================================
INT8U GetDvcInfo(INT8U *DvcInfo)
{
	INT8U Status;
	SerBfr[CMDTYPE] = 1;
	SerBfr[COMMAND] = 'A';
	SerBfr[LENGTH] = 0;
	Status = ATS522_Cmd();
	if(Status == 0)
	{
		Status = SerBfr[STATUS];
		if(Status == 0)
		     memcopy(DvcInfo,&SerBfr[DATA],SerBfr[LENGTH]);
	}
	return Status;
}

INT8U PiccRequest(INT8U Req_Code,INT8U *TagType)
{
	INT8U Status;
	SerBfr[CMDTYPE] = 2;
	SerBfr[COMMAND] = 'A';
	SerBfr[LENGTH] = 1;
	SerBfr[DATA] = Req_Code;
	Status = ATS522_Cmd();
	if(Status == 0)
	{
		Status = SerBfr[STATUS];
		if(Status == 0)
		{
			*TagType = SerBfr[DATA];
			*(TagType+1) = SerBfr[DATA+1];
		}
	}
	return Status;
}
//===========================================================================
INT8U PiccAnticoll(INT8U Sel_Code,INT8U Bcnt,INT8U *PiccSnr)
{   
	INT8U Status;
	SerBfr[CMDTYPE] = 2;
	SerBfr[COMMAND] = 'B';
	SerBfr[LENGTH] = 2;
	SerBfr[DATA] = Sel_Code;
	SerBfr[DATA+1] = Bcnt;
	Status = ATS522_Cmd();
	if(Status == 0)
	{
		Status = SerBfr[STATUS];
		if(Status == 0)
		{
			memcopy(PiccSnr,&SerBfr[DATA],4);
		}
	}
	return Status;
}
//===========================================================================
INT8U PiccSelect(INT8U Sel_Code,INT8U *PiccSnr,INT8U *Sak)
{   
	INT8U Status;
	SerBfr[CMDTYPE] = 2;
	SerBfr[COMMAND] = 'C';
	SerBfr[LENGTH] = 5;
	SerBfr[DATA] = Sel_Code;
	memcopy(&SerBfr[DATA+1],PiccSnr,4);
	Status = ATS522_Cmd();
	if(Status == 0)
	{
		Status = SerBfr[STATUS];
		if(Status == 0)
		{
			*Sak = SerBfr[DATA];
		}
	}
	return Status;
}
//===========================================================================
INT8U PiccHalt()
{
	INT8U Status;

	SerBfr[CMDTYPE] = 2;
	SerBfr[COMMAND] = 'D';
	SerBfr[LENGTH] = 0;

	Status = ATS522_Cmd();

	if(Status == 0)
	{
		Status = SerBfr[STATUS];
	}
	return Status;
}

//===========================================================================
INT8U PiccAuthKey(INT8U KeyAB,INT8U *PiccSnr,INT8U *Key,INT8U Block)
{
	INT8U Status;

	SerBfr[CMDTYPE] = 2;
	SerBfr[COMMAND] = 'F';
	SerBfr[LENGTH] = 12;
	SerBfr[DATA] = KeyAB;
	memcopy(&SerBfr[DATA+1],PiccSnr,4);
	memcopy(&SerBfr[DATA+5],Key,6);
	SerBfr[DATA+11] = Block;

	Status = ATS522_Cmd();

	if(Status == 0)
	{
		Status = SerBfr[STATUS];
	}
	return Status;
}
//===========================================================================
INT8U PiccRead(INT8U Block,INT8U *Bfr)
{
	INT8U Status;

	SerBfr[CMDTYPE] = 2;
	SerBfr[COMMAND] = 'G';
	SerBfr[LENGTH] = 1;
	SerBfr[DATA] = Block;

	Status = ATS522_Cmd();

	if(Status == 0)
	{
		Status = SerBfr[STATUS];
		if(Status == 0)
		{
			memcopy(Bfr,&SerBfr[DATA],16);
		}
	}
	return Status;
}
//===========================================================================
INT8U PiccWrite(INT8U Block,INT8U *Bfr)
{
	INT8U Status;

	SerBfr[CMDTYPE] = 2;
	SerBfr[COMMAND] = 'H';
	SerBfr[LENGTH] = 17;
	SerBfr[DATA] = Block;
	memcopy(&SerBfr[DATA+1],Bfr,16);

	Status = ATS522_Cmd();

	if(Status == 0)
	{
		Status = SerBfr[STATUS];
	}
	return Status;
}


//���ò����ʵȣ�����д
void RC522_COMM_Init(void)	
{
/*
	PCON = 0x80;              	// SMOD = 1;
	SCON = 0x50;              	// Mode 1, 8-bit UART, enable receiption
	TMOD = 0x21;	      		// Timer 1, mode 2, 8-bit auto reload,
	TH1 = 0xfa;
	TL1 = 0xfa;
	TR1 = 1;
	TH0 = 0x00;
	TL0 = 0x01;
	ET0 = 1;        
	TR0 = 1;        
	EA = 1;
	SerDTvalid = FALSE;			  // ��ģ��ֻ֧�̶ֹ��Ĳ����� 9600 1ֹͣλ ��У��λ
    REN = 1;
	 ES = 1;
	 */
	 /*
}
*/
/*
#define READ_WRITE_SECTOR   	  1      // ��д������
#define READ_WRITE_VALUE_SECTOR   2      // ��д������
/****************************************************************************/
/****************************************************************************/
/*
void RFIDmain(void)
{

	long lValue,lReadValue;

//    SPEAK=0;
//	LED_OK = 1;
//    T2_timeout_init(RCAP2_5ms);


    RC522_COMM_Init();

//        SPEAK=1;     
//        WaitRespDly=40;
//        while (WaitRespDly!=0) {}
//        SPEAK=0; 

   	GetDvcInfo(DataBfr); 
    while (1) {

//      	SPEAK = 1;
		// �ӳ�һ��ʱ�俪ʼ����	
		//WaitRespDly=200;
		//while (WaitRespDly!=0) {}

		
        /**************************************************************
            Ѱ������֤����ͨ��֮��ſ��Զ�д
        *************************************************************/
        // ����
/*        if (PiccRequest(REQSTD,DataBfr) != OK) {
//			
            continue;   
        } 
		//ucLedShowBuf[3] = 1;						// ��4λ�����ָʾ����ɹ�
        // ����ײ ,�ɹ� Card_Snr ���ص��ǿ���	  
        if(PiccAnticoll(SELCODE1,0,Card_Snr) != OK) {
			//ucLedShowBuf[2] = 2;				 	// ��3λ�����ָʾ��ײ��ʧ��
            continue; 
        }
 		//ucLedShowBuf[3] = 2;					 	// ��4λ�����ָʾ��ײ���ɹ�


		

		WaitRespDly=200;
        while (WaitRespDly!=0) {}

		ucLedShowBuf[3] =  Card_Snr[3] &  0xf;	// ��ʾ����
		ucLedShowBuf[2] = (Card_Snr[3] >> 4);
		ucLedShowBuf[1] =  Card_Snr[2] &  0xf;
		ucLedShowBuf[0] = (Card_Snr[2] >> 4);
		
		OSMboxPost(RFIDMbox,(void*)ucLedShowBuf);
		
		WaitRespDly=200;
        while (WaitRespDly!=0) {}

        /*************************************************************
            д�뿨������������������ݱȶ���ȷ����������һ��
        *************************************************************/
//        LED_OK = 0;     
/*        WaitRespDly=40;
        while (WaitRespDly!=0) {}
//        LED_OK = 1;

        PiccHalt();
    }
}
*/

















/*
void Uart_Init(int pclk,int baud) 
{ 
    int i; 
    if(pclk == 0) 
    pclk    = PCLK; 
    rUFCON0 = 0x0;   //UART channel 0 FIFO control register, FIFO disable 
    rUFCON1 = 0x0;   //UART channel 1 FIFO control register, FIFO disable 
    rUFCON2 = 0x0;   //UART channel 2 FIFO control register, FIFO disable 
    rUMCON0 = 0x0;   //UART chaneel 0 MODEM control register, AFC disable 
    rUMCON1 = 0x0;   //UART chaneel 1 MODEM control register, AFC disable 
//UART0 
    rULCON0 = 0x3;   //Line control register : Normal,No parity,1 stop,8 bits 
                    //����ģʽ����У��λ��ÿ֡һ��ֹͣλ��8λ����
     //    [10]       [9]     [8]        [7]        [6]      [5]         [4]           [3:2]        [1:0] 
     // Clock Sel,  Tx Int,  Rx Int, Rx Time Out, Rx err, Loop-back, Send break,  Transmit Mode, Receive Mode 
     //     0         1       0    ,     0          1        0           0     ,       01          01 
     //   PCLK       Level    Pulse    Disable    Generate  Normal      Normal        Interrupt or Polling 
     //PCLKʱ�� ����/����ʱ�ֱ��õ�ƽ/�����жϷ�ʽ ���ճ�ʱ�жϹ� ���ճ���ʱ�����ж� �����ûػ� �����ж� ���ͺͽ���Ϊ�жϻ�����ѯģʽ
    rUCON0  = 0x245;   // Control register 
    rUBRDIV0=( (int)(pclk/16./baud+0.5) -1 );   //Baud rate divisior register 0 
                                        //����������
//UART1 
    rULCON1 = 0x3; 
    rUCON1  = 0x245; 
    rUBRDIV1=( (int)(pclk/16./baud+0.5) -1 ); 
//UART2 
    rULCON2 = 0x3; 
    rUCON2  = 0x245; 
    rUBRDIV2=( (int)(pclk/16./baud+0.5) -1 );     

    for(i=0;i<100;i++); 
} 

 
void Uart_Select(int ch) 
{ 
    whichUart = ch; 
} 
   
void Uart_SendByte(int data) 
{ 
    if(whichUart==0)    //ѡ��ͨ��0
    { 
        if(data=='\n')     //����������
        { 
            while(!(rUTRSTAT0 & 0x2));     //ֱ�����ͻ�����Ϊ��
           // Delay(1);          //because the slow response of hyper_terminal 
            WrUTXH0('\r');    //���ͻ���д��ת���ַ�
        } 
        while(!(rUTRSTAT0 & 0x2));   //�ȷ�������һ������
      //  Delay(1); 
        WrUTXH0(data);     //д����һ�����ݵ����ͻ���
    } 
    else if(whichUart==1)     //ѡ��ͨ��1
    { 
        if(data=='\n')     //����������
        { 
            while(!(rUTRSTAT1 & 0x2));     //ֱ�����ͻ�����Ϊ��
            //Delay(1);                 //because the slow response of hyper_terminal 
            rUTXH1 = '\r';    //���ͻ���д��ת���ַ�
        } 
        while(!(rUTRSTAT1 & 0x2));   //�ȷ�������һ������
        //Delay(1); 
        rUTXH1 = data;     //д����һ�����ݵ����ͻ���
    }   
    else if(whichUart==2)     //ѡ��ͨ��2
    { 
        if(data=='\n')     //����������
        { 
            while(!(rUTRSTAT2 & 0x2));     //ֱ�����ͻ�����Ϊ��
            //Delay(1);                 //because the slow response of hyper_terminal 
            rUTXH2 = '\r';    //���ͻ���д��ת���ַ�
        } 
        while(!(rUTRSTAT2 & 0x2));   //�ȷ�������һ������
        //Delay(1); 
        rUTXH2 = data;     //д����һ�����ݵ����ͻ���
    }       
}         

void Uart_SendString(char *pt) 
{ 
    while(*pt)     //�ж��Ƿ�Ϊ��ָ��
        Uart_SendByte(*pt++);     //�������ݣ�ָ��ָ����һ������
} 

void Uart_TxEmpty(int ch) 
{ 
    if(ch==0) 
        while(!(rUTRSTAT0 & 0x4)); //�ȵ�������λ�Ĵ�����
    else if(ch==1) 
        while(!(rUTRSTAT1 & 0x4)); //�ȵ�������λ�Ĵ�����
         
    else if(ch==2) 
        while(!(rUTRSTAT2 & 0x4)); //�ȵ�������λ�Ĵ�����
} 
 
char Uart_Getch(void) 
{ 
    if(whichUart==0) 
    {       
        while(!(rUTRSTAT0 & 0x1));     //ֱ�����յ����� 
        return RdURXH0();     //���ؽ��ջ����ַ
    } 
    else if(whichUart==1) 
    {       
        while(!(rUTRSTAT1 & 0x1));     //ֱ�����յ�����
        return RdURXH1();    //���ؽ��ջ����ַ
    } 
    else if(whichUart==2) 
    { 
        while(!(rUTRSTAT2 & 0x1));     //ֱ�����յ�����
        return RdURXH2();    //���ؽ��ջ����ַ
    } 
     
    return 0 ; 
} 

void Uart_GetString(char *string) 
{ 
    char *string2 = string; 
    char c; 
    while((c = Uart_Getch())!='\r')     //�յ��Ĳ��ǻس�������
    { 
        if(c=='\b')     //�յ������˸��
        { 
            if( (int)string2 < (int)string ) 
            { 
                Uart_Printf("\b \b");     //��ӡ�����˸�
                string--;     //stringָ����һ���ַ���ַ
            } 
        } 
        else 
        { 
            *string++ = c; //��c����stringָ��ĵ�ַ��stringָ����һ����ַ
            Uart_SendByte(c);     //����c��� ���ַ�
        } 
    } 
    *string='\0';     //string��ɿ�ָ��
    Uart_SendByte('\n');     //���ͽ���
} 
char Uart_GetKey(void) 
{ 
    if(whichUart==0)     //ͨ��0
    {       
        if(rUTRSTAT0 & 0x1)    //���յ�����
            return RdURXH0();     //���ؽ��ջ����ַ
        else 
            return 0; 
    } 
    else if(whichUart==1)     //ͨ��1
    { 
        if(rUTRSTAT1 & 0x1)    //�յ�����
            return RdURXH1();     //���ؽ��ջ����ַ
        else 
            return 0; 
    } 
    else if(whichUart==2)     //ͨ��2
    {       
        if(rUTRSTAT2 & 0x1)    //�յ�����
            return RdURXH2();     //���ؽ��ջ����ַ
        else 
            return 0; 
    }     

      return 0 ; 
} 

int Uart_GetIntNum(void) 
{ 
    char str[30]; 
    char *string = str; 
    int base     = 10; 
    int minus    = 0; 
    int result   = 0; 
    int lastIndex;     
    int i; 
     
    Uart_GetString(string);     //�Ӵ��ڽ������ݣ������stringָ��ĵ�ַ
     
    if(string[0]=='-')     //���յ��Ǹ�����
    { 
        minus = 1;     //������
        string++;     //string����һ���ֽ�
    } 
     
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))   //���յ���ʮ��������
    { 
        base    = 16;     //ʮ��������
        string += 2;     //string���������ֽ�
    } 
    lastIndex = strlen(string) - 1;      //string�ĳ���
    if(lastIndex<0)     //����С��0�򷵻�-1
        return -1; 
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )    //���һ���ַ�Ϊh��H
    { 
        base = 16;     //ʮ��������
        string[lastIndex] = 0;     //��ʮ�������������һλ��0
        lastIndex--;     //string����-1
    } 

    if(base==10)     //ʮ������
    { 
        result = atoi(string);     //�ַ���stringת��������
        result = minus ? (-1*result):result;     //�Ǹ������-1
    } 
    else 
    { 
        for(i=0;i<=lastIndex;i++) 
        { 
            if(isalpha(string))     //���string��Ӣ����ĸ
            { 
                if(isupper(string))     //����Ǵ�д��ĸ
                    result = (result<<4) + string - 'A' + 10; 
                else 
                    result = (result<<4) + string - 'a' + 10; 
            } 
            else 
                result = (result<<4) + string - '0'; 
        } 
        result = minus ? (-1*result):result; 
    } 
    return result; 
} 

int Uart_GetIntNum_GJ(void) 
{ 
    char string[16] ; 
    char *p_string = string ; 
    char c; 
    int i = 0 ; 
    int data = 0 ; 

    while(   ( c = Uart_Getch()) != '\r'  )     //�س�
    { 
           if(c=='\b')       p_string--;     //�˸����p_stringָ����һ���ַ�
           else           *p_string++=c;  //c��ֵ��p_stringָ����һ���ַ�
           Uart_SendByte( c ) ;     //���͸��ַ�
    } 

    *p_string = '\0';     //�ַ�������

      i = 0 ; 
      while( string != '\0' )     //�ж��ַ����Ƿ����
      { 
           data = data * 10 ;   //data*10
           if( string[i]<'0'||string[i]>'9' )     //���������ַ�
                 return -1 ; 
           data = data + ( string[i]-'0' ) ;    //ת��������
           i++ ;       
      }     
       
      return data ;
} 

void Uart_Printf(char *fmt,...) 
{ 
    va_list ap; 
    char string[256]; 
    va_start(ap,fmt); 
    vsprintf(string,fmt,ap); 
    Uart_SendString(string); 
    va_end(ap); 
} 
*/