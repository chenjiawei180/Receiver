#include "timer.h"
#include "key.h"
#include "menu.h"
#include "ev1527.h"

unsigned char await_number = 0;      //待机时刻计算50MS次数变量
unsigned char await_number_table = 0;//0.5秒标志变量

unsigned char main_press_time_table = 0;//main计算时间变量跟标志位
unsigned char main_press_time = 0;

unsigned char again_and_again_time = 0;// 判断重复解码所需要的计算时间变量

void Init_Timer0(void)
{
	TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响	
	TH0 = (65536 - TIMER50MS) >> 8; //重新赋值 5ms
	TL0 = (65536 - TIMER50MS) & 0xff;
	EA = 1;            //总中断打开
	ET0 = 1;           //定时器中断打开
	TR0 = 1;           //定时器开关打开
}

void Init_Timer1(void) 
{
	TMOD |= 0x10;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响	
	TH1 = (65536 - 100) >> 8; //重新赋值 100us
	TL1 = (65536 - 100) & 0xff;
	EA = 1;            //总中断打开
	ET1 = 1;           //定时器中断打开
	//TR1=1;           //定时器开关打开
}

void Timer0_isr(void) interrupt 1  //定时器0中断服务程序
{
	unsigned char func_index_temp = 0;
	TF0 = 0;
	TH0 = (65536 - TIMER50MS) >> 8;		  //重新赋值 50ms
	TL0 = (65536 - TIMER50MS) & 0xff;
	func_index_temp = return_func_index();
	//if (func_index_temp == MENU_STANDBY || func_index_temp == TWO_MENU_F0_YEAR || func_index_temp == TWO_MENU_F0_MOUTH 
	//	|| func_index_temp == TWO_MENU_F0_DAY || func_index_temp == TWO_MENU_F0_WEEK || func_index_temp == TWO_MENU_F0_HOUR
	//	|| func_index_temp == TWO_MENU_F0_MINUTE || func_index_temp == TWO_MENU_F1_E1_D1 || func_index_temp == TWO_MENU_F1_E1_D2
	//	|| func_index_temp == TWO_MENU_F1_E1_D3 || func_index_temp == TWO_MENU_F1_E1_D4 || func_index_temp == TWO_MENU_F1_E2_D1
	//	|| func_index_temp == TWO_MENU_F1_E2_D2 || func_index_temp == TWO_MENU_F1_E2_D3 || func_index_temp == TWO_MENU_F1_E2_D4	
	//	)
	if (1)
	{
		await_number++;
		if (await_number == 10)
		{
			await_number_table++;
			await_number = 0;
		}
		if (await_number_table >= 4)//确保程序正确延时
		{
			await_number_table = 0;
		}		
	}

	if (main_press_time_table == 1) //菜单键按下时间标志
	{
		main_press_time++;			//计算菜单键按下时间长度变量
	}

	if (return_again_and_again_decoder_table() == 1)
	{
		again_and_again_time++;
		if (again_and_again_time >120)
		{
			clear_again_and_again_decoder_table();
			again_and_again_time = 0;
		}
	}

}

void Timer1_isr(void) interrupt 3  //定时器1中断服务程序
{
	TF1 = 0;
	TH1 = (65536 - 100) >> 8;		  //重新赋值 100us
	TL1 = (65536 - 100) & 0xff;
	RF_decode_main();
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

void set_main_press_time_table(unsigned char temp) //设置main_press_time_table变量的值
{
	main_press_time_table = temp;
}

unsigned char return_main_press_time(void)
{
	unsigned char temp = 0;
	temp = main_press_time;
	return temp;
}

void clear_main_press_time(void)
{
	main_press_time = 0;
}