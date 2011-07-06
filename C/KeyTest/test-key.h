#ifndef __ZLG9290_H__
#define __ZLG9290_H__


#define ZLG_SLAVE_ADDR	(0x70 >> 1)


#define REG_Sys		0x00
#define REG_Key		0x01
#define REG_Cnt		0x02
#define REG_Func	0x03
#define REG_Cmd0	0x07
#define REG_Cmd1	0x08
#define REG_Flas	0x0C
#define REG_Num		0x0D
#define REG_Dis0	0x10
#define REG_Dis1	0x11
#define REG_Dis2	0x12
#define REG_Dis3	0x13
#define REG_Dis4	0x14
#define REG_Dis5	0x15
#define REG_Dis6	0x16
#define REG_Dis7	0x17

#define SEG_A		(1<<7)
#define SEG_B		(1<<6)
#define SEG_C		(1<<5)
#define SEG_D		(1<<4)
#define SEG_E		(1<<3)
#define SEG_F		(1<<2)
#define SEG_G		(1<<1)
#define SEG_DIP		(1<<0)


#endif /*__ZLG9290_H__*/