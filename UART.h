#ifndef _UART0_H_
#define _UART0_H_

void InitUART0(void);
u8 U0_RXchar(void);
void U0_TXchar(u8 sByte);
s8 *U0_RXstr(void);
void U0_TXstr(s8 *s);
#endif
