#include<LPC21XX.h>
#include"defines.h"
#include"types.h"
#include"delay.h"
//keypad PINS CONNECTION TO PORT PINS    PORT1
#define ROW0 16
#define ROW1 17
#define ROW2 18
#define ROW3 19
#define COL0 20
#define COL1 21
#define COL2 22
#define COL3 23
#define OPINS_8  0
u8 KpmLUT[4][4]=
{
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};
void Write2IOPIN1(u32 b1,u32 b2,u32 b3,u32 b4)
{
	WRITEBIT(IOPIN1,ROW0,b1);
	WRITEBIT(IOPIN1,ROW1,b2);
	WRITEBIT(IOPIN1,ROW2,b3);
	WRITEBIT(IOPIN1,ROW3,b4);
}
void InitKpm(void)
{
	SETBIT(IODIR1,ROW0);
	SETBIT(IODIR1,ROW1);
	SETBIT(IODIR1,ROW2);
	SETBIT(IODIR1,ROW3);
}
u8 ColScan(void)
{
	u32 t1,t2,t3,t4;
	t1=READBIT(IOPIN1,COL0);
	t2=READBIT(IOPIN1,COL1);
	t3=READBIT(IOPIN1,COL2);
	t4=READBIT(IOPIN1,COL3);
	return(t1&t2&t3&t4);
}
u8 RowCheck()
{
	u8 rNo;
	Write2IOPIN1(0,1,1,1);
	if(!ColScan())
	{
		rNo=0;
		goto last;
	}
	Write2IOPIN1(1,0,1,1);
	if(!ColScan())
	{
		rNo=1;
		goto last;
	}
	Write2IOPIN1(1,1,0,1);
	if(!ColScan())
	{
		rNo=2;
		goto last;
	}
	Write2IOPIN1(1,1,1,0);
	if(!ColScan())
	{
		rNo=3;
	}
last:
	Write2IOPIN1(0,0,0,0);
	return rNo;
}
u8 ColCheck()
{
	u8 cNo;
	if(READBIT(IOPIN1,COL0)==0)
		cNo=0;
	else if(READBIT(IOPIN1,COL1)==0)
		cNo=1;
	else if(READBIT(IOPIN1,COL2)==0)
		cNo=2;
	else if(READBIT(IOPIN1,COL3)==0)
		cNo=3;
	return cNo;
}
u8 KeyScan()
{
	u8 rNo,cNo,KeyV;
	while(ColScan());
	rNo=RowCheck();
	cNo=ColCheck();
	KeyV=KpmLUT[rNo][cNo];
	while(!ColScan());
	return KeyV;
}
