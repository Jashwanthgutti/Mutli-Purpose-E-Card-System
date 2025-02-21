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
#define UART_INT_ENABLE 1
void InitUART0 (void); /* Initialize Serial Interface       */ 
void UART0_Tx(char ch);  
char UART0_Rx(void);
u8 buff[10],dummy;
unsigned char i=0,ch,r_flag;
char num[2][10]={"00315642","00338865"};

void UART0_isr(void) __irq
{
  if((U0IIR & 0x04)) //check if receive interrupt
  {
		ch = U0RBR;	/* Read to Clear Receive Interrupt */
		if(ch == 0x02)
			r_flag=1;
		else if(ch!=0x03 && r_flag==1)
		{
			buff[i++]=ch; 
			UART0_Tx(ch);
		}
		else if(ch==0x03)
		{
			buff[i] = '\0';
			i=0;
			r_flag = 2;
		}
  }
  else
  {
      dummy=U0IIR; //Read to Clear transmit interrupt
  }
   VICVectAddr = 0; /* dummy write */
}
void InitUART0 (void) /* Initialize Serial Interface       */ 
{  
            		
  PINSEL0 |= 0x00000005; /* Enable RxD0 and TxD0              */
  U0LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */
  U0LCR = 0x03;         /* DLAB = 0  */
  
  #if UART_INT_ENABLE > 0

  VICIntSelect = 0x00000000; // IRQ
  VICVectAddr0 = (unsigned)UART0_isr;
  VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */
  VICIntEnable = 1 << 6;   /* Enable UART0 Interrupt */
 
 // U0IIR = 0xc0;
 // U0FCR = 0xc7;
  U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   
             
  #endif
						
}
void UART0_Tx(char ch)  /* Write character to Serial Port    */  
{ 
  while (!(U0LSR & 0x20));
  U0THR = ch;                
}

char UART0_Rx(void)    /* Read character from Serial Port   */
{                     
  while (!(U0LSR & 0x01));
  return (U0RBR);
}

void UART0_Str(char *s)
{
   while(*s)
       UART0_Tx(*s++);
}

u16 pin,newpin[10],cnfpin[10],curpin[10],pin2[10];
u16 orgpin=6281,CNFPIN,CNFPIN,CURPIN,NEWPIN;//default pin"6281"
u32 j,temp;
u8 choice;

int rfid(void)
{
	i2c_eeprom_write(0x50,0X00,orgpin);
	i2c_eeprom_write(0x50,0X01,orgpin>>8);
	while(1)
	{	
		r_flag=0,i=0;
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("PLACE THE CARD");
		delay_ms(100);
		//r_flag=0,i=0;
		while(r_flag!=2);
		//place card on RFID reader
		CmdLCD(GOTO_LINE2_POS0);
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
while(1)
{
			i2c_eeprom_write(0x50,0X00,orgpin);
			i2c_eeprom_write(0x50,0X01,orgpin>>8);
			CmdLCD(GOTO_LINE1_POS0);
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("ENTER PASSWORD");
			/*CmdLCD(GOTO_LINE2_POS0);
			strLCD("2:EDIT PASSWORD");
			delay_s(2);
			CmdLCD(CLEAR_LCD);
			CmdLCD(GOTO_LINE1_POS0);
			strLCD("ENTER CHOICE");
			choice=KeyScan();
			CmdLCD(CLEAR_LCD);
			charLCD(choice);*/
			delay_ms(1000);
            if(Password_entry())
				return 1;
   			return 0;
			/*switch(choice)
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
			}  */
}
}		
void Enterpin(u16 *enteredpin)
{

	s8 value;
	u32 i=0;
	do
	{
		value=KeyScan();
		if(value=='C')
		{
		i--;
		CmdLCD(GOTO_LINE2_POS0+i);
		charLCD(32);
		}
		CmdLCD(GOTO_LINE2_POS0+i);
		charLCD(value);
		delay_ms(100);
		CmdLCD(GOTO_LINE2_POS0+i);
		charLCD('*');
		delay_ms(400);
		enteredpin[i++]=value-48;
	}
	while((value!='D')&&(i<9));
	enteredpin[i-1]='\0';

	for(i=0;i<4;i++)
	{
	pin=10*pin+enteredpin[i];
	}
}
int Password_entry(void)
{  
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	strLCD("ENTER PASSWORD:");
	Enterpin(pin2);
	orgpin=(i2c_eeprom_read(0x50,0X00))|((i2c_eeprom_read(0x50,0X01))<<8);
	if(pin==orgpin)
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
	u32 i;
	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	strLCD("ENTER CURRENT PASSWORD:");
	Enterpin(curpin);
	for(i=0;i<4;i++)
			{
				CURPIN=10*CURPIN+curpin[i];
			}
	orgpin=(i2c_eeprom_read(0x50,0X00))|((i2c_eeprom_read(0x50,0X01)<<8));
	delay_s(1);
	if(CURPIN==orgpin)
	{
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("ENTER NEW PASSWORD:");
		Enterpin(newpin);
		for(i=0;i<4;i++)
			{
				NEWPIN=10*NEWPIN+newpin[i];
			}
		CmdLCD(CLEAR_LCD);
		CmdLCD(GOTO_LINE1_POS0);
		strLCD("CONFIRM PASSWORD:");
		Enterpin(cnfpin);
		for(i=0;i<4;i++)
			{
				CNFPIN=10*CNFPIN+cnfpin[i];
			}
		if(NEWPIN==CNFPIN)
		{
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
