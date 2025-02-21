#include<stdlib.h>
#include<string.h>
#include"lcddefines.h"
#include"defines.h"
#include"types.h"
#include"delay.h"
#include"i2c_defines.h"
#include"i2c_eeprom.h"

const u32 accno=334455;
u8 name[20]="ARM_PROJECT";
extern u16 bal;

int deposit()
{
int num[10];
char ch;
int i=0;
u16 amt=0;
CmdLCD(GOTO_LINE2_POS0);
while(1)
{
ch=KeyScan();
delay_ms(100);
num[i++]=ch-48;
charLCD(ch);
if(i==4)
break;			  
}
for(i=0;i<4;i++)
{
amt=10*amt+num[i];
}
bal=(i2c_eeprom_read(0x50,0X10))|((i2c_eeprom_read(0x50,0X11)<<8));
bal=bal+amt;
i2c_eeprom_write(0x50,0X10,bal);
i2c_eeprom_write(0x50,0X11,bal>>8);
CmdLCD(CLEAR_LCD);
CmdLCD(GOTO_LINE1_POS0);
strLCD("BALANCE:");
u16LCD(bal);
delay_ms(1000);
return 1;
}
int withdrawl()
{
int num[10];
char ch;
int i=0;
u16 amt=0;
bal=(i2c_eeprom_read(0x50,0X10))|((i2c_eeprom_read(0x50,0X11)<<8));
while(1)
{
ch=KeyScan();
delay_ms(100);
num[i++]=ch-48;
charLCD(ch);
if(i==4)
break;			  
}
for(i=0;i<4;i++)
{
amt=10*amt+num[i];
}
	if(amt<=bal)
	{	if(bal<500)
		{
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("MINIMUM BALENCE 500");
		CmdLCD(GOTO_LINE2_POS0);
		strLCD("AMOUNT NOT WITHDRAWN");
		delay_s(2);
		return 0;
		}
		else
		{
		bal=(bal-amt);
		i2c_eeprom_write(0x50,0X10,bal);
		i2c_eeprom_write(0x50,0X11,bal>>8);
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("BALENCE:");
		u16LCD(bal);
		delay_s(1);
		return 1;
		}
	}
	else
	{
		strLCD("insufficient bal");
		delay_s(1);
		return 0;
	}
}
int balenq(void)
{
	bal=(i2c_eeprom_read(0x50,0X10))|((i2c_eeprom_read(0x50,0X11)<<8));
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	strLCD("current bal:");
	CmdLCD(GOTO_LINE2_POS0);
	u32LCD(bal);
	delay_s(2);
	return 1;
}
int mini_statement()
{
	bal=(i2c_eeprom_read(0x50,0X10))|((i2c_eeprom_read(0x50,0X11)<<8));
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	strLCD("Acc.no:");
	CmdLCD(GOTO_LINE1_POS0+8);
	u32LCD(accno);
	CmdLCD(GOTO_LINE2_POS0);
	strLCD("Name:");
	CmdLCD(GOTO_LINE2_POS0+5);
	strLCD(name);
	CmdLCD(GOTO_LINE3_POS0);
	strLCD("Balance:");
	CmdLCD(GOTO_LINE3_POS0+8);
	u32LCD(bal);
	delay_s(2);
	return 1;
}
void Atm_card(void)
{
char choice;
if(password())
	{
	while(1)
	{
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("1:DEPOSIT");
		CmdLCD(GOTO_LINE2_POS0);
		strLCD("2:WITHDRAWL");
		CmdLCD(GOTO_LINE3_POS0);
		strLCD("3:BALENQUIRY");
		CmdLCD(GOTO_LINE4_POS0);
		strLCD("4.MINI_STATEMENT");
		CmdLCD(GOTO_LINE1_POS0+10);
		strLCD("5:EXIT");
		delay_s(2);
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("enter your choice:");
		delay_s(1);
		choice=KeyScan();
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		switch(choice)
		{	
			case '1':strLCD("amt to deposit");
			       deposit();
			       break;
			case '2':strLCD("amt to withdraw");
			       if(withdrawl())
			       {
				       CmdLCD(CLEAR_LCD);
				       CmdLCD(GOTO_LINE1_POS0);
				       strLCD("withdrawl done");
			       }
			       else
			       {
				       CmdLCD(CLEAR_LCD);
				       CmdLCD(GOTO_LINE1_POS0);
				       strLCD("withdrawl fail");
			       }
			       break;
			case '3':balenq();
			       break;
			case '4':mini_statement();
			       break;
			case '5':exit(0);
		}
		}
	}

}
