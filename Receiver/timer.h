#ifndef __TIMER_H_
#define __TIMER_H_

#include "STC15F2K60S2.H"
#define TIMER50MS 50000

extern void Init_Timer0(void);


extern unsigned char return_await_number_table(void);
extern void set_await_number_table(unsigned char temp);

#endif