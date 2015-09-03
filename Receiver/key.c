#include "key.h"
#include "usart.h"
#include "tm1629.h"
#include "menu.h"

unsigned char func_index = 0; //多级菜单索引变量
void(*current_operation_index)();// 多级菜单函数指针

key_table code table[100] =
{	// 目标索引		    上				下          确认		 退出         函数
	{ MENU_STANDBY, MENU_STANDBY, MENU_STANDBY, MENU_STANDBY, MENU_STANDBY, (*fun0) }, //待机

	{ ONE_MENU_F0, ONE_MENU_F9, ONE_MENU_F1, 0, MENU_STANDBY, (*fun1) }, //F0-F9
	{ ONE_MENU_F1, ONE_MENU_F0, ONE_MENU_F2, 0, MENU_STANDBY, (*fun2) },
	{ ONE_MENU_F2, ONE_MENU_F1, ONE_MENU_F3, 0, MENU_STANDBY, (*fun3) },
	{ ONE_MENU_F3, ONE_MENU_F2, ONE_MENU_F4, 0, MENU_STANDBY, (*fun4) },
	{ ONE_MENU_F4, ONE_MENU_F3, ONE_MENU_F5, 0, MENU_STANDBY, (*fun5) },
	{ ONE_MENU_F5, ONE_MENU_F4, ONE_MENU_F6, 0, MENU_STANDBY, (*fun6) },
	{ ONE_MENU_F6, ONE_MENU_F5, ONE_MENU_F7, 0, MENU_STANDBY, (*fun7) },
	{ ONE_MENU_F7, ONE_MENU_F6, ONE_MENU_F8, 0, MENU_STANDBY, (*fun8) },
	{ ONE_MENU_F8, ONE_MENU_F7, ONE_MENU_F9, 0, MENU_STANDBY, (*fun9) },
	{ ONE_MENU_F9, ONE_MENU_F8, ONE_MENU_F0, 0, MENU_STANDBY, (*fun10) },

	{ ONE_MENU_FH, ONE_MENU_FH, ONE_MENU_FH, 0, MENU_STANDBY, (*fun2) }, //FH


};

unsigned int KeyScan(void)  //Keyboard scan function
{
	unsigned int Val = 0;
	HKeyPort |= 0x1f;//Row height
	LKeyPort &= 0x07;
	if ((HKeyPort & 0x1f) != 0x1f)//Press button
	{
		delay10ms();  //Remove jitter
		if ((HKeyPort & 0x1f) != 0x1f)   //Press button
		{
			HKeyPort |= 0x1f; //检测第一列
			LKeyPort |= 0xf8;
			LKeyPort &= 0x7f;
			if ((HKeyPort & 0x1f) != 0x1f)
			{
				//	return_standby_time=0;
				//	sound_table=1;
				Val = HKeyPort & 0x1f;
				Val <<= 8;
				Val += (LKeyPort & 0xf8);
				if (Val == 0x1e78)
				{
					//		main_press_time=0;
					//		main_press_time_table=1;
				}
				while ((HKeyPort & 0x1f) != 0x1f);
				delay10ms();
				while ((HKeyPort & 0x1f) != 0x1f);
				//	main_press_time_table=0;
				return Val;
			}
		}
	}
	return 0x0fff;
}

unsigned char KeyDecoder(void)
{
	unsigned int key_val = 0;

	key_val = KeyScan();
#if 0	  
	if (key_val == 0x0fff) /*do not key press*/
	{
		/*sjz 有注册管理器标志*/
		if (register_manager == 1)
		{

			if (func_index == FUNC_STANDBY)
			{
				if ((old2_RF_RECE_REG[2] & 0x0f) == 0x01)
				{
					accumulate_decoder++;
				}
				else
				{
					accumulate_decoder = 0;
				}
				if (accumulate_decoder>10)
				{
					accumulate_50ms = 20;
					accumulate_decoder = 0;
				}
			}
			register_manager = 0;
			switch (old2_RF_RECE_REG[2] & 0x0f)
			{
			case 0x01:key_val = 0x1778; break;
			case 0x02:key_val = 0x1b78; break;
			case 0x04:key_val = 0x1d78; break;
			case 0x08:key_val = 0x1e78; break;
			default:break;
			}
		}
	}
#endif 	 

#ifdef DEBUG
	if (key_val != 0x0fff)
	uart_printf("key_val is %x .\r\n", key_val); //测试按键键值
#endif
	switch (key_val)
	{
	case 0x1778:return KEY_RETURN; break;//1 按下相应的键显示相对应的码值
	case 0x1d78:return KEY_DOWN; break;//2  
	case 0x1b78:return KEY_UP; break;//4
	case 0x1e78:return KEY_FUNC; break;//5 按下相应的键显示相对应的码值
	default:return 0xff; break;
	}
}

void KeyProcess(void)
{
	unsigned char key_value;
	key_value = KeyDecoder();
	switch (key_value)
	{
		case KEY_FUNC:		func_index = table[func_index].enter; break;
		case KEY_RETURN:	func_index = table[func_index].esc; break;
		case KEY_UP:		func_index = table[func_index].up; break;
		case KEY_DOWN:		func_index = table[func_index].down; break;
		default:break;
	}
	current_operation_index = table[func_index].index_operation;
	(*current_operation_index)();//执行当前操作函数
}

void delay10ms(void)   //误差 -0.054253472222us
{
	unsigned char a, b, c;
	for (c = 4; c>0; c--)
	for (b = 52; b>0; b--)
	for (a = 220; a>0; a--);
	_nop_();  //if Keil,require use intrins.h
}