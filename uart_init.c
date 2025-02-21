/*#include<lpc21xx.h>
#include"types.h"
#include"defines.h"
#include"delay.h"
#define FUN2 1
#define CFGPIN(WORD,PIN,FUNC) WORD=((PIN<16)?((WORD&~(3<<(PIN*2)))|(FUNC<<(PIN*2))):((WORD&~(3<<((PIN-16)*2)))|(FUNC<<((PIN-16)*2))))
#define FOSC 12000000
#define CCLK 5*FOSC
#define PCLK CCLK/4
#define BAUD 9600
#define DIVISOR (PCLK/(16*BAUD))
#define WORD_LEN_8 3
#define DLAB_BIT 7
#define DR_BIT 0
#define THRE_BIT 5
#define TEMT_BIT 6
#define TXD0_PIN FUN2
#define RXD0_PIN FUN2
void InitUART0(void)
{
	CFGPIN (PINSEL0,0,FUN2);
	CFGPIN (PINSEL0,1,FUN2);
	U0LCR=1<<DLAB_BIT|WORD_LEN_8;
	U0DLL=DIVISOR;
	U0DLM=DIVISOR>>8;
	CLRBIT(U0LCR,DLAB_BIT);
}


u8 U0_RXchar(void)
{
	while(READBIT(U0LSR,DR_BIT)==0);
	return U0RBR;
}


void U0_TXchar(u8 sByte)
{
	U0THR=sByte;
	while(READBIT(U0LSR,TEMT_BIT)==0);
}


s8 *U0_RXstr(void)
{
	static s8 rBuff[20];
	s32 i=0;
	while(1)
	{
		rBuff[i]=U0_RXchar();
		if(rBuff[i]=='\r')
		{
			rBuff[i]='\0';
			break;
		}
		i++;
	}
	return rBuff;
}


void U0_TXstr(s8 *s)
{
	while(*s)
		U0_TXchar(*s++);
}
*/
