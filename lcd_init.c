#include<LPC21XX.h>
#include"defines.h"
#include"types.h"
#include"delay.h"
#include"lcddefines.h"


void WriteLCD(u8 dat)
{
	WRITEBYTE(IOPIN0,LCD_DATA,dat);
	SSETBIT(IOSET0,LCD_EN);
	delay_us(1);
	SCLRBIT(IOCLR0,LCD_EN);
	delay_ms(2);
}
void CmdLCD(u8 cmd)
{
	SCLRBIT(IOCLR0,LCD_RS);
	WriteLCD(cmd);
}
void charLCD(u8 ch)
{
	SSETBIT(IOSET0,LCD_RS);
	WriteLCD(ch);
}
void strLCD(u8 *ptr)
{
	while(*ptr)
		charLCD(*ptr++);
}
void u32LCD(u32 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
	{
		charLCD('0');
	}
	else
	{
		while(n)
		{
			a[i++]=(n%10)+48;
			n=n/10;
		}
		for(--i;i>=0;i--)
		{
			charLCD(a[i]);
		}
	}
}
void u16LCD(u16 n)
{
	u8 a[10];
	s32 i=0;
	if(n==0)
	{
		charLCD('0');
	}
	else
	{
		while(n)
		{
			a[i++]=(n%10)+48;
			n=n/10;
		}
		for(--i;i>=0;i--)
		{
			charLCD(a[i]);
		}
	}
}
void s32LCD(s32 n)
{
	if(n<0)
	{
		charLCD('-');
		n=-n;
	}
	u32LCD(n);
}
void F32LCD(f32 n,u8 nDP)
{
	s32 i,j;
	if(n<0.0)
	{
		charLCD('-');
		n=-n;
	}
	i=n;
	u32LCD(i);
	charLCD('.');
	for(j=0;j<nDP;j++)
	{
		n=(n-i)*10;
		i=n;
		charLCD(i+48);
	}
}
void BinLCD(u32 n,u8 nBD)
{
	s32 i,cnt=0;
	if(nBD<=16)
	{
		CmdLCD(GOTO_LINE1_POS0);
		for(i=(nBD-1);i>=0;i--)
		{
			charLCD(((n>>i)&1)+48);
		}
	}
	else
	{
		CmdLCD(GOTO_LINE1_POS0);
		for(i=(nBD-1);i>=16;i--)
		{
			charLCD(((n>>i)&1)+48);
			cnt++;
		}
		CmdLCD(GOTO_LINE2_POS0);
		for(i=((nBD-cnt)-1);i>=0;i--)
		{
			charLCD(((n>>i)&1)+48);
		}
	}
}
void HexLCD(u32 n)
{
	u8 a[8],t;
	s32 i=0;
	if(n==0)
	{
		strLCD("00000000");
	}
	else
	{
		while(n)
		{
			t=(n%16);
			//t=(t>9)?((t-10)+'A'):(t+48);
			t=(t<10)?(t+48):((t-10)+'A');
			a[i++]=t;
			//i++;
			n=n/16;
		}
		for(--i;i>=0;i--)
		{
			charLCD(a[i]);
		}
	}
}
void Build_CGRAM(u8 *p,u32 nBytes)
{
	u32 i;
	//point to cgram start
	CmdLCD(GOTO_CGRAM_START);
	for(i=0;i<nBytes;i++)
		//write to cgram via data reg
		charLCD(p[i]);
	//return back display memory
	CmdLCD(GOTO_LINE1_POS0);
}
void InitLCD(void)
{
	WRITEBYTE(IODIR0,LCD_DATA,0XFF);
	SETBIT(IODIR0,LCD_RS);
	SETBIT(IODIR0,LCD_RW);
	SETBIT(IODIR0,LCD_EN);
	delay_ms(40);
	CmdLCD (0X30);
	CmdLCD (0X30);
	CmdLCD (0X30);
	CmdLCD (MODE_8BIT_2LINE);
	CmdLCD (DSP_ON_CUR_BLINK);
	CmdLCD (CLEAR_LCD);
	CmdLCD (SHIFT_CUR_RIGHT);
}

