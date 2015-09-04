#ifndef __TIMER_H_
#define __TIMER_H_

#include "STC15F2K60S2.H"
#define TIMER50MS 50000

extern void Init_Timer0(void);


extern unsigned char return_await_number_table(void);
extern void set_await_number_table(unsigned char temp);

extern void set_main_press_time_table(unsigned char temp);
extern unsigned char return_main_press_time(void);
extern void clear_main_press_time(void);
#endif