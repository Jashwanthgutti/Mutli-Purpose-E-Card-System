#include<LPC214x.h>
#include<string.h>
#include "delay.h"
#include "types.h"
#include"lcddefines.h"

#define FOSC      12000000
#define CCLK  	  5*FOSC
#define PCLK  	  CCLK/4
#define PREINT_VAL (u32)((PCLK/32768)-1)
#define PREFRAC_VAL (u32)(PCLK - ((PREINT_VAL + 1) * 32768))

void Init_RTC(void)
{
  DOM=00;
  MONTH=00;
  YEAR=0000;
}

u8 myatoi(u8 t1,u8 t2)
{
return (((t1&0x0f)*10)+(t2&0x0f));
}

u8 myitoa1(u8 t)
{
return ((t/10)+48);
}

u8 myitoa2(u8 t)
{
return((t%10)+48);
}

void SetCalendar(u8 *cal)
{
DOM=myatoi(cal[0],cal[1]);
MONTH=myatoi(cal[3],cal[4]);
YEAR=2000+myatoi(cal[8],cal[9]);
}
u8 *GetCalendar(void)
{
static u8 cal[11]="00/00/2000";
u8 t;
u16 Y;
t=DOM;
cal[0]=myitoa1(t);
cal[1]=myitoa2(t);
t=MONTH;
cal[3]=myitoa1(t);
cal[4]=myitoa2(t);
Y=YEAR;
cal[8]=((Y%100)/10)+48;
cal[9]=(Y%10)+48;
return cal;
}
int check(u8*cal)
{
//SetCalendar("15/01/2024");
int date,month,year,valid_date=15,valid_month=1,valid_year=2025;
date=myatoi(cal[0],cal[1]);
month=myatoi(cal[3],cal[4]);
year=2000+myatoi(cal[8],cal[9]);
if(year<valid_year||(year==valid_year&&month<valid_month)||(year==valid_year&&month==valid_month&&date<=valid_date))
return 1;
else
return 0;
}
