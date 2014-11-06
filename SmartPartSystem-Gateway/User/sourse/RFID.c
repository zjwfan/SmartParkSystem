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
// ISO14443 命令错误代码
#define STATUS_ATS_ERROR			    0xCE	// ATS错误
#define STATUS_PPS_ERROR  			    0xCD	// PPSS错误
#define STATUS_BLOCK_FORMAT_ERROR       0xCC	// 分组帧格式错误 
#define STATUS_CID_ERROR  			    0xCB	// CID错误
#define STATUS_PCB_ERROR				0xCA	// PCB错误
#define STATUS_NAK_ERROR				0xC9	// NAK错误
#define STATUS_LEN_ERROR				0xC8	// 长度错误
#define STATUS_SEND_ERROR				0xC7	// 发送错误
#define STATUS_APDU_ERROR				0xC6	// APDU命令错误
#define STATUS_PERORM_ERROR				0xC4	// 命令执行结果错误
#define STATUS_INVALID_VALUE			0xC3	// 值块格式错误
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
//	TI = 1;                                 // 触发发送中断,dd在中断中发送数据

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


//设置波特率等，需重写
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
	SerDTvalid = FALSE;			  // 本模块只支持固定的波特率 9600 1停止位 无校验位
    REN = 1;
	 ES = 1;
	 */
	 /*
}
*/
/*
#define READ_WRITE_SECTOR   	  1      // 读写扇区号
#define READ_WRITE_VALUE_SECTOR   2      // 读写扇区号
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
		// 延迟一段时间开始读卡	
		//WaitRespDly=200;
		//while (WaitRespDly!=0) {}

		
        /**************************************************************
            寻卡和验证卡，通过之后才可以读写
        *************************************************************/
        // 请求
/*        if (PiccRequest(REQSTD,DataBfr) != OK) {
//			
            continue;   
        } 
		//ucLedShowBuf[3] = 1;						// 第4位数码管指示请求成功
        // 防碰撞 ,成功 Card_Snr 返回的是卡号	  
        if(PiccAnticoll(SELCODE1,0,Card_Snr) != OK) {
			//ucLedShowBuf[2] = 2;				 	// 第3位数码管指示防撞碰失败
            continue; 
        }
 		//ucLedShowBuf[3] = 2;					 	// 第4位数码管指示防撞碰成功


		

		WaitRespDly=200;
        while (WaitRespDly!=0) {}

		ucLedShowBuf[3] =  Card_Snr[3] &  0xf;	// 显示卡号
		ucLedShowBuf[2] = (Card_Snr[3] >> 4);
		ucLedShowBuf[1] =  Card_Snr[2] &  0xf;
		ucLedShowBuf[0] = (Card_Snr[2] >> 4);
		
		OSMboxPost(RFIDMbox,(void*)ucLedShowBuf);
		
		WaitRespDly=200;
        while (WaitRespDly!=0) {}

        /*************************************************************
            写入卡的数据与读出卡的数据比对正确，蜂鸣器响一下
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
                    //正常模式，无校验位，每帧一个停止位，8位数据
     //    [10]       [9]     [8]        [7]        [6]      [5]         [4]           [3:2]        [1:0] 
     // Clock Sel,  Tx Int,  Rx Int, Rx Time Out, Rx err, Loop-back, Send break,  Transmit Mode, Receive Mode 
     //     0         1       0    ,     0          1        0           0     ,       01          01 
     //   PCLK       Level    Pulse    Disable    Generate  Normal      Normal        Interrupt or Polling 
     //PCLK时钟 发送/接受时分别用电平/脉冲中断方式 接收超时中断关 接收出错时触发中断 不设置回环 发送中断 发送和接收为中断或者轮询模式
    rUCON0  = 0x245;   // Control register 
    rUBRDIV0=( (int)(pclk/16./baud+0.5) -1 );   //Baud rate divisior register 0 
                                        //波特率设置
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
    if(whichUart==0)    //选择通道0
    { 
        if(data=='\n')     //遇到结束符
        { 
            while(!(rUTRSTAT0 & 0x2));     //直到发送缓存区为空
           // Delay(1);          //because the slow response of hyper_terminal 
            WrUTXH0('\r');    //向发送缓存写入转义字符
        } 
        while(!(rUTRSTAT0 & 0x2));   //等发送完上一个数据
      //  Delay(1); 
        WrUTXH0(data);     //写入下一个数据到发送缓存
    } 
    else if(whichUart==1)     //选择通道1
    { 
        if(data=='\n')     //遇到结束符
        { 
            while(!(rUTRSTAT1 & 0x2));     //直到发送缓存区为空
            //Delay(1);                 //because the slow response of hyper_terminal 
            rUTXH1 = '\r';    //向发送缓存写入转义字符
        } 
        while(!(rUTRSTAT1 & 0x2));   //等发送完上一个数据
        //Delay(1); 
        rUTXH1 = data;     //写入下一个数据到发送缓存
    }   
    else if(whichUart==2)     //选择通道2
    { 
        if(data=='\n')     //遇到结束符
        { 
            while(!(rUTRSTAT2 & 0x2));     //直到发送缓存区为空
            //Delay(1);                 //because the slow response of hyper_terminal 
            rUTXH2 = '\r';    //向发送缓存写入转义字符
        } 
        while(!(rUTRSTAT2 & 0x2));   //等发送完上一个数据
        //Delay(1); 
        rUTXH2 = data;     //写入下一个数据到发送缓存
    }       
}         

void Uart_SendString(char *pt) 
{ 
    while(*pt)     //判断是否为空指针
        Uart_SendByte(*pt++);     //发送数据，指针指向下一个数据
} 

void Uart_TxEmpty(int ch) 
{ 
    if(ch==0) 
        while(!(rUTRSTAT0 & 0x4)); //等到发送移位寄存器空
    else if(ch==1) 
        while(!(rUTRSTAT1 & 0x4)); //等到发送移位寄存器空
         
    else if(ch==2) 
        while(!(rUTRSTAT2 & 0x4)); //等到发送移位寄存器空
} 
 
char Uart_Getch(void) 
{ 
    if(whichUart==0) 
    {       
        while(!(rUTRSTAT0 & 0x1));     //直到接收到数据 
        return RdURXH0();     //返回接收缓存地址
    } 
    else if(whichUart==1) 
    {       
        while(!(rUTRSTAT1 & 0x1));     //直到接收到数据
        return RdURXH1();    //返回接收缓存地址
    } 
    else if(whichUart==2) 
    { 
        while(!(rUTRSTAT2 & 0x1));     //直到接收到数据
        return RdURXH2();    //返回接收缓存地址
    } 
     
    return 0 ; 
} 

void Uart_GetString(char *string) 
{ 
    char *string2 = string; 
    char c; 
    while((c = Uart_Getch())!='\r')     //收到的不是回车将继续
    { 
        if(c=='\b')     //收到的是退格符
        { 
            if( (int)string2 < (int)string ) 
            { 
                Uart_Printf("\b \b");     //打印两个退格
                string--;     //string指向上一个字符地址
            } 
        } 
        else 
        { 
            *string++ = c; //将c付给string指向的地址，string指向下一个地址
            Uart_SendByte(c);     //发送c存放 的字符
        } 
    } 
    *string='\0';     //string变成空指针
    Uart_SendByte('\n');     //发送结束
} 
char Uart_GetKey(void) 
{ 
    if(whichUart==0)     //通道0
    {       
        if(rUTRSTAT0 & 0x1)    //接收到数据
            return RdURXH0();     //返回接收缓存地址
        else 
            return 0; 
    } 
    else if(whichUart==1)     //通道1
    { 
        if(rUTRSTAT1 & 0x1)    //收到数据
            return RdURXH1();     //返回接收缓存地址
        else 
            return 0; 
    } 
    else if(whichUart==2)     //通道2
    {       
        if(rUTRSTAT2 & 0x1)    //收到数据
            return RdURXH2();     //返回接收缓存地址
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
     
    Uart_GetString(string);     //从串口接收数据，存放在string指向的地址
     
    if(string[0]=='-')     //接收的是负整数
    { 
        minus = 1;     //负整数
        string++;     //string增加一个字节
    } 
     
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))   //接收的是十六进制数
    { 
        base    = 16;     //十六进制数
        string += 2;     //string增加两个字节
    } 
    lastIndex = strlen(string) - 1;      //string的长度
    if(lastIndex<0)     //长度小于0则返回-1
        return -1; 
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )    //最后一个字符为h或H
    { 
        base = 16;     //十六进制数
        string[lastIndex] = 0;     //将十六进制数的最后一位赋0
        lastIndex--;     //string长度-1
    } 

    if(base==10)     //十进制数
    { 
        result = atoi(string);     //字符串string转化成整型
        result = minus ? (-1*result):result;     //是负数则乘-1
    } 
    else 
    { 
        for(i=0;i<=lastIndex;i++) 
        { 
            if(isalpha(string))     //如果string是英文字母
            { 
                if(isupper(string))     //如果是大写字母
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

    while(   ( c = Uart_Getch()) != '\r'  )     //回车
    { 
           if(c=='\b')       p_string--;     //退格符，p_string指向上一个字符
           else           *p_string++=c;  //c的值给p_string指向下一个字符
           Uart_SendByte( c ) ;     //发送该字符
    } 

    *p_string = '\0';     //字符串结束

      i = 0 ; 
      while( string != '\0' )     //判断字符串是否结束
      { 
           data = data * 10 ;   //data*10
           if( string[i]<'0'||string[i]>'9' )     //不是数字字符
                 return -1 ; 
           data = data + ( string[i]-'0' ) ;    //转换成整数
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