#include "timer.h"
#include "key.h"
#include "menu.h"

unsigned char await_number = 0;      //待机时刻计算50MS次数变量
unsigned char await_number_table = 0;//0.5秒标志变量

void Init_Timer0(void)
{
	TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响	
	TH0 = (65536 - TIMER50MS) >> 8; //重新赋值 5ms
	TL0 = (65536 - TIMER50MS) & 0xff;
	EA = 1;            //总中断打开
	ET0 = 1;           //定时器中断打开
	TR0 = 1;           //定时器开关打开
}

void Timer0_isr(void) interrupt 1
{
	unsigned char func_index_temp = 0;
	TF0 = 0;
	TH0 = (65536 - TIMER50MS) >> 8;		  //重新赋值 50ms
	TL0 = (65536 - TIMER50MS) & 0xff;
	func_index_temp = return_func_index();
	if (func_index_temp == MENU_STANDBY)
	{
		await_number++;
		if (await_number == 10)
		{
			await_number_table++;
			await_number = 0;
		}
	}

}

unsigned char return_await_number_table(void)	//返回await_number_table变量的值
{
	unsigned char await_number_table_temp=0;
	await_number_table_temp = await_number_table;
	return await_number_table_temp;
}

void set_await_number_table(unsigned char temp)	//设置await_number_table变量的值
{
	await_number_table = temp;
}