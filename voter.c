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

u8 vote_status=0;

const u8 cgramLUT[]=
{
0x15,0x15,0x15,0x15,0x1F,0x04,0x04,0x00,
0x1D,0x05,0x05,0x1F,0x14,0x14,0x17,0x00,
0x04,0x0E,0x1F,0x1F,0x04,0x04,0x04,0x04,
0x1F,0x1F,0x1F,0x1F,0x10,0x10,0x10,0x10,
};

char choice2;
void voter_menu(void)
{			CmdLCD(CLEAR_LCD);
			Build_CGRAM((u8*)cgramLUT,32);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("1:party1--->");
			charLCD(0);
			CmdLCD(GOTO_LINE2_POS0);
			strLCD("2:party2--->");
			charLCD(1);
			CmdLCD(GOTO_LINE3_POS0);
			strLCD("3:party3--->");
			charLCD(2);
			CmdLCD(GOTO_LINE4_POS0);
			strLCD("4:party4--->");
			charLCD(3);
			delay_s(2);
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("enter your choice:");
			choice2=KeyScan();
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			switch(choice2)
			{	
				case '1':i2c_eeprom_write(0x50,0X20,1);
						strLCD("voted to party1");
						charLCD(0);
						delay_s(2);
					    break;
				case '2':i2c_eeprom_write(0x50,0X20,1);
						strLCD("voted to party2");
						charLCD(1);
						delay_s(2);
						break;
				case '3':i2c_eeprom_write(0x50,0X20,1);
						strLCD("voted to party3");
						charLCD(2);
						delay_s(2);
				        break;
				case '4':i2c_eeprom_write(0x50,0X20,1);
						strLCD("voted to party4");
						charLCD(3);
				        break;
				case '5':exit(0);
				default:strLCD("invalid input");
			}
}

void voter_card(void)
{
char choice;
if(password())
{
i2c_eeprom_write(0x50,0X20,0);
while(1)
	{
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("1:USER CARD");
		CmdLCD(GOTO_LINE2_POS0);
		strLCD("2:OFFICIER CARD");
		CmdLCD(GOTO_LINE3_POS0);
		strLCD("3:EXIT");
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
			case '1':USER_CARD();
			       break;
			case '2':OFFICIER_CARD();
			       break;
			case '3':exit(0);
		}
	}
}
}
void USER_CARD(void)
{
vote_status=i2c_eeprom_read(0x50,0X20);
CmdLCD(CLEAR_LCD);
CmdLCD(GOTO_LINE1_POS0);
strLCD("CHECKING VOTING");
CmdLCD(GOTO_LINE2_POS0);
strLCD("STATUS");
delay_s(2);
if(vote_status)
{
CmdLCD(CLEAR_LCD);
CmdLCD(GOTO_LINE1_POS0);
strLCD("voting done");
delay_s(2);
}
else
{
CmdLCD(CLEAR_LCD);
CmdLCD(GOTO_LINE1_POS0);
strLCD("voting not done");
CmdLCD(GOTO_LINE2_POS0);
strLCD("please vote now");
delay_s(2);
voter_menu();
}
}
void OFFICIER_CARD(void)
{
CmdLCD(CLEAR_LCD);
CmdLCD(GOTO_LINE1_POS0);
strLCD("Resetting voting");
CmdLCD(GOTO_LINE2_POS0);
strLCD("status");
delay_s(2);
i2c_eeprom_write(0x50,0X20,0);
CmdLCD(CLEAR_LCD);
CmdLCD(GOTO_LINE2_POS0+6);
strLCD("DONE");
delay_s(2);
}
