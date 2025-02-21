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

u16 pin[10],newpin[10],cnfpin[10],curpin[10];
u16 orgpin=6281;//default pin"6281"
u32 j,temp;
u8 choice;

extern u8 i,buff[11],num[2][11];//RFID CARD NUMBER
extern u8 r_flag;

int rfid(void)
{
	i2c_eeprom_write(0x50,0X00,orgpin);
	i2c_eeprom_write(0x50,0X01,orgpin>>8);
	strLCD("pin saved");
	delay_s(1);
	while(1)
	{
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("PLACE THE CARD");
		delay_ms(100);
		r_flag=0,i=0;
		while(r_flag!=2);
		//place card on RFID reader
		CmdLCD(GOTO_LINE2_POS0);
		strLCD(buff);
		delay_ms(2000);
		//checking whether card is valid or not
		for(j=0;j<2;j++)
		{
			if(strcmp((const char*)buff,(const char*)num[j])==0)
			{
				CmdLCD(GOTO_LINE3_POS0);
				strLCD("VALID CARD");
				delay_ms(500);
				temp=1;
				break;
			}
		}

		if(temp==0)
		{
			CmdLCD(GOTO_LINE3_POS0);
			strLCD("INVALID CARD");
			delay_ms(500);
		}
		if(temp)
		return 1;
		else 
		return 0;
		}
}
		//CHECKING PASSWORD VALIDITY
int password(void)
{
			i2c_eeprom_write(0x50,0X00,orgpin);
			i2c_eeprom_write(0x50,0X01,orgpin>>8);
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("1:ENTER PASSWORD");
			CmdLCD(GOTO_LINE2_POS0);
			strLCD("2:EDIT PASSWORD");
			delay_s(2);
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("ENTER CHOICE");
			choice=KeyScan();
			CmdLCD(CLEAR_LCD);
			charLCD(choice);
			delay_ms(100);
			switch(choice)
			{
				case '1':if(Password_entry())
							return 1;
						else
							return 0;
				case '2':if(Reset_password())
							return 1;
						else
							return 0;
				default :CmdLCD(CLEAR_LCD);
					 CmdLCD(GOTO_LINE1_POS0);
					 strLCD("INVALID CHOICE");
					 delay_s(2);
			}
			return 0;
}		
void Enterpin(u16 *pin)
{
	s8 value;
	u32 i=0;
	do
	{
		value=KeyScan();
		CmdLCD(GOTO_LINE2_POS0+i);
		charLCD(value);
		delay_ms(100);
		CmdLCD(GOTO_LINE2_POS0+i);
		charLCD('*');
		delay_ms(400);
		pin[i++]=value-48;
	}
	while((value!='D')&&(i<9));
	pin[i-1]='\0';
}
int Password_entry(void)
{
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	strLCD("ENTER PASSWORD:");
	Enterpin(pin);
	orgpin=(i2c_eeprom_read(0x50,0X00))|(i2c_eeprom_read(0x50,0X01));
	delay_s(1);
	if((strcmp((const char*)pin,(const char*)orgpin))==0)
	{
		CmdLCD(CLEAR_LCD);
		strLCD("valid password");
		delay_s(1);
		return 1;
	}
	else
	{
		CmdLCD(CLEAR_LCD);
		strLCD("Invalid password");
		delay_s(1);
		return 0;
	}
}
int Reset_password(void)
{
	u32 NEWPIN=0,i;
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	strLCD("ENTER CURRENT PASSWORD:");
	Enterpin(curpin);
	orgpin=(i2c_eeprom_read(0x50,0X00))|(i2c_eeprom_read(0x50,0X01));
	delay_s(1);
	if(strcmp((const char*)curpin,(const char*)orgpin)==0)
	{
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("ENTER NEW PASSWORD:");
		Enterpin(newpin);
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("CONFIRM PASSWORD:");
		Enterpin(cnfpin);
		if(strcmp((const char*)newpin,(const char*)cnfpin)==0)
		{
			for(i=0;i<4;i++)
			{
				NEWPIN=10*NEWPIN+newpin[i];
			}
			i2c_eeprom_write(0x50,0X00,NEWPIN);
			i2c_eeprom_write(0x50,0X01,NEWPIN>>8);
			delay_ms(500);
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("PASSWORD UPDATED");
			CmdLCD(GOTO_LINE2_POS0);
			strLCD("SUCCESSFULLY");
			delay_ms(500);
			return 1;
		}
		else
		{
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("NEW PASSWORD IS");
			CmdLCD(GOTO_LINE2_POS0);
			strLCD("NOT MATCHING");
			delay_ms(500);
			return 0;
		}
	}
	else
	{
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("ERROR INVALID");
		CmdLCD(GOTO_LINE2_POS0);
		strLCD("PASSWORD");
		CmdLCD(GOTO_LINE3_POS0);
		strLCD("DIDNOT CHANGED");
		delay_ms(500);
		return 0;
	}
}


