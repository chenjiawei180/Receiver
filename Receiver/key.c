#include "key.h"
#include "usart.h"

unsigned char func_index = 0; //多级菜单索引变量
void(*current_operation_index)();// 多级菜单函数指针

key_table code table[100] =
{
	{ 0, 0, 0, 0, 0, (*delay10ms) },
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

void delay10ms(void)   //误差 -0.054253472222us
{
	unsigned char a, b, c;
	for (c = 4; c>0; c--)
	for (b = 52; b>0; b--)
	for (a = 220; a>0; a--);
	_nop_();  //if Keil,require use intrins.h
}