
extern OS_TCB *BeepSem;
void graph()		
{	
	int err1;	
	char string1;	
	char c1=0x01;
	char *error="!!!";
	int sdata[10]={0,1,2,3,4,5,6,7,8,9};
	string1=Uart_getchar();
		switch(string1)
		{
			case'a':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[2],sdata[9],sdata[3]);break;
			case'b':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[0],sdata[0]);break;
			case'c':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[0],sdata[3]);break;
			case'd':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[0],sdata[8]);break;
			case'e':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[1],sdata[2]);break;
			case'f':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[2],sdata[1]);break;
			case'g':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[2],sdata[4]);break;
			case'h':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[2],sdata[8]);break;
			case'i':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[3],sdata[2]);break;
			case'j':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[3],sdata[9]);break;
			case'k':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[4],sdata[5]);break;
			case'l':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[4],sdata[8]);break;
			case'm':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[5],sdata[2]);break;
			case'n':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[6],sdata[1]);break;
			case'o':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[6],sdata[5]);break;
			case'p':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[7],sdata[0]);break;
			case'q':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[7],sdata[3]);break;
			case'r':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[7],sdata[7]);break;
			case's':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[8],sdata[4]);break;
			case't':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[9],sdata[1]);break;
			case'u':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[9],sdata[4]);break;
			case'v':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[4],sdata[0],sdata[0]);break;
			case'w':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[4],sdata[0],sdata[5]);break;
			case'x':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[4],sdata[0],sdata[9]);break;
			case'y':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[4],sdata[1],sdata[5]);break;
			case'z':Lcd_printf(139, 70, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[4],sdata[1],sdata[7]);break;
			
			
			case'.':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[8],sdata[3],sdata[2]);break;
			case'/':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[8],sdata[0],sdata[2]);break;
			case'?':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[7],sdata[7],sdata[0]);break;
			case'Z':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[7],sdata[4],sdata[2]);break;
			case'Y':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[6],sdata[9],sdata[9]);break;
			case'X':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[6],sdata[6],sdata[9]);break;
			case'W':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[6],sdata[4],sdata[5]);break;
			case'V':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[6],sdata[1],sdata[0]);break;
			case'U':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[5],sdata[8],sdata[8]);break;
			case'T':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[5],sdata[5],sdata[7]);break;
			case'S':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[5],sdata[2],sdata[1]);break;
			case'R':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[5],sdata[0],sdata[2]);break;
			case'Q':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[4],sdata[7],sdata[5]);break;
			case'P':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[4],sdata[4],sdata[6]);break;
			case'O':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[4],sdata[1],sdata[3]);break;
			case'N':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[8],sdata[1]);break;
			case'M':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[5],sdata[3]);break;
			case'L':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[3],sdata[1],sdata[9]);break;
			case'K':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[2],sdata[9],sdata[3]);break;
			case'J':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[2],sdata[6],sdata[7]);break;
			case'I':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[2],sdata[3],sdata[4]);break;
			case'H':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[2],sdata[0],sdata[7]);break;
			case'G':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[1],sdata[7],sdata[3]);break;
			case'F':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[1],sdata[4],sdata[3]);break;
			case'E':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[1],sdata[1],sdata[4]);break;
			case'D':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[0],sdata[8],sdata[7]);break;
			case'C':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[0],sdata[5],sdata[3]);break;
			case'B':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[0],sdata[2],sdata[4]);break;
			case'A':Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n%d%d.%d\n",sdata[0],sdata[0],sdata[0]);break;
			case '!':
			Lcd_printf(141, 113, RGB(0xFF,0xFF,0xFF), RGB(0x19,0x19,0x70), 0,"\n!!!\n",error);
			OSSemPend(BeepSem, 0, &err1);
			Beep(100, 100);						/* ±¨¾¯Æ÷·½Ê½1				*/
    		OSSemPost(BeepSem);
    		break;
		
			default:break;
		}
}