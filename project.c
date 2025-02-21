#include<LPC21XX.h>
#include<string.h>
#include<stdlib.h>
#include"defines.h"
#include"types.h"
#include"delay.h"
#include"i2c_defines.h"
#include"i2c_eeprom.h"
#include"lcddefines.h"
#include"i2c.h"
u16 bal=10000;
char choice1;
int main()
{
	InitLCD();
	InitUART0();
	InitKpm();
	Init_I2c();
	Init_RTC();
	i2c_eeprom_write(0x50,0X10,bal);
	i2c_eeprom_write(0x50,0X11,bal>>8);
	delay_ms(1000);

	while(1)
	{	
		
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0+4);
			strLCD("MULTI PURPOSE");
			CmdLCD(GOTO_LINE3_POS0+4);
			strLCD("E-CARD SYSTEM");
			delay_s(2);
			if(rfid())
			{
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("1:ATM CARD");
			CmdLCD(GOTO_LINE2_POS0);
			strLCD("2:PANCARD");
			CmdLCD(GOTO_LINE3_POS0);
			strLCD("3.DRIVING LICENSE");
			CmdLCD(GOTO_LINE4_POS0);
			strLCD("4:VOTER ID CARD");
			delay_s(3);
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("enter your choice:");
			delay_ms(500);
			choice1=KeyScan();
			switch(choice1)
			{	
				case '1':Atm_card();
				       break;
				case '2':pan_card();               
					   break;
				case '3':driving_license();
				       break;
				case '4':voter_card();
				       break;
				case '5':exit(0);
				default:strLCD("invalid input");
			}
			}
	}
}
