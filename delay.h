#include "types.h"

void delay_us(u32 dlyUS);
void delay_ms(u32 dlyMS);
void delay_s(u32 dlyS);

void WriteLCD(u8 dat);
void CmdLCD(u8 cmd);
void charLCD(u8 ch);
void strLCD(u8 *ptr);
void InitLCD(void);
void u32LCD(u32 n);
void u16LCD(u16 n);
void s32LCD(s32 n);
void F32LCD(f32 n,u8 nDP);
void BinLCD(u32 n,u8 nBD);
void HexLCD(u32 n);
void Build_CGRAM(u8 *p,u32 nBytes);

void Write2IOPIN1(u32 b1,u32 b2,u32 b3,u32 b4);
void InitKpm(void);
u8 ColScan(void);
u8 RowCheck(void);
u8 ColCheck(void);
u8 KeyScan(void);

void InitUART0(void);
u8 U0_RXchar(void);
void U0_TXchar(u8 sByte);
s8 *U0_RXstr(void);
void U0_TXstr(s8 *s);

void Enterpin(u16 *pin);
int Reset_password(void);
int Password_entry(void);
int rfid(void);

void Atm_card(void);
void pan_card(void);
void driving_license(void);



void Init_RTC(void);
u8 myatoi(u8 t1,u8 t2);
u8 myitoa1(u8 t);
u8 myitoa2(u8 t);
void SetCalendar(u8 *cal);
u8* GetCalendar(void);
int check(u8 *cal);
int v_check(u8*cal);


void USER_DETAILS(void);
void license_validity_check(void);

void voter_card(void);
void voter_menu(void);
int vote_status_check(void);
void USER_CARD(void);
void OFFICIER_CARD(void);

int password(void);
