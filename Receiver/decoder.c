#include "decoder.h"
#include "ev1527.h"
#include "usart.h"
#include "at24c256.h"
#include "key.h"
#include "menu.h"
#include "tm1629.h"
#include "timer.h"
#include "gd5800.h"

unsigned char buf_eeprom[8] = { 0 };//写入EEPROM_buf

void DecoderProcess(void)
{	
	unsigned char i, j ,l ,k;
	unsigned char temp_buff[8];//解码用临时数组
	unsigned char temp_buff1[PAGE_LENGTH] = { 0 };//查询AT24C256临时数组

	unsigned char func_index_temp = 0;		//创建临时变量，用于放回其他外部变量
	unsigned char Two_Menu_F8_E1_temp = 0;	//创建临时变量，用于放回其他外部变量
	unsigned char Two_Menu_F8_E2_temp = 0;	//创建临时变量，用于放回其他外部变量
	unsigned char Two_Menu_F3_E1_temp = 0;	//创建临时变量，用于放回其他外部变量	
	unsigned char Two_Menu_F3_E2_temp = 0;	//创建临时变量，用于放回其他外部变量
	unsigned char Two_Menu_F7_E1_temp = 0;	//创建临时变量，用于放回其他外部变量
	unsigned char Two_Menu_F6_E1_temp = 0;	//创建临时变量，用于放回其他外部变量
	unsigned char Two_Menu_F6_E2_temp = 0;	//创建临时变量，用于放回其他外部变量

	func_index_temp = return_func_index();	//返回所需要的外部变量
	Two_Menu_F8_E1_temp = return_Two_Menu_F8_E1();	//返回所需要的外部变量
	Two_Menu_F8_E2_temp = return_Two_Menu_F8_E2();	//返回所需要的外部变量
	Two_Menu_F3_E1_temp = return_Two_Menu_F3_E1();	//返回所需要的外部变量
	Two_Menu_F3_E2_temp = return_Two_Menu_F3_E2();	//返回所需要的外部变量
	Two_Menu_F7_E1_temp = return_Two_Menu_F7_E1();	//返回所需要的外部变量
	Two_Menu_F6_E1_temp = return_Two_Menu_F6_E1();	//返回所需要的外部变量
	Two_Menu_F6_E2_temp = return_Two_Menu_F6_E2();	//返回所需要的外部变量

	receive_rf_decoder();	//解码函数

	if (return_again_receive_rf_decoder_finished() == 1) //标志位等于1 说明在2次检验下通过,接收到有效码
	{
#ifdef DEBUG
		uart_printf("decoder_val is %02x %02x %02x .\r\n", (unsigned int)old2_RF_RECE_REG[0], (unsigned int)old2_RF_RECE_REG[1], (unsigned int)old2_RF_RECE_REG[2]); //测试按键键值
#endif
		switch (func_index_temp)
		{
		case MENU_STANDBY://待机状态下
		{
			set_logout_cycle_table(0);//循环跟销号重新计数
			//键盘规则
			if ((old2_RF_RECE_REG[2] & 0xf0) == 0x00 && (((old2_RF_RECE_REG[0] >> 4) == Two_Menu_F7_E1_temp) || (Two_Menu_F7_E1_temp == 10)))//键盘规则，程序按默认的来编
			{
				if (Two_Menu_F8_E1_temp == 1)  //为按键值
				{
					temp_buff[0] = single_key[old2_RF_RECE_REG[2] & 0x0f] ;
				}
				else
				{
					temp_buff[0] = multiple_key[old2_RF_RECE_REG[2] & 0x0f];
				}   
				temp_buff[1] = old2_RF_RECE_REG[0] >> 4;	//为防区号
				temp_buff[2] = old2_RF_RECE_REG[0] & 0x0f;	//为3位组码第一位
				temp_buff[3] = old2_RF_RECE_REG[1] >> 4;	//为3位组码第二位
				temp_buff[4] = old2_RF_RECE_REG[1] & 0x0f;	//为3位组码第三位
				temp_buff[5] = old2_RF_RECE_REG[0];			//备用项,暂且存第1位ID码
				temp_buff[6] = old2_RF_RECE_REG[1];			//备用项,暂且存第2位ID码
				temp_buff[7] = old2_RF_RECE_REG[2];			//备用项,暂且存第3位ID码

				if (temp_buff[0] != QUXIAO_1 && temp_buff[0])
				{
					for (l = 0; l < Two_Menu_F6_E2_temp; l++)
					{
						submenuf6_1(Two_Menu_F6_E1_temp, temp_buff, temp_buff[0], old2_RF_RECE_REG[2] & 0x0f);
					}
				}
				else
				{
					Cancel_funtion(temp_buff,display_ram);//取消函数
					tm1629_load();
					display();
					break;
				}		

				tm1629_clear();//清屏
				decoder_temp_to_mcuram(display_ram, temp_buff);//将临时数组的数据移入单片机暂存数组 8字节转6字节
				tm1629_load();//单片机把数组内容载入数码管显存数组中
				display();//显示数码管
				set_func_index(DECODER_MENU);//此时跳入解码菜单，为下一次解码做准备
				//clear_again_receive_rf_decoder_finished();//清除解码完成标志位

				break;
			}
			//呼叫器注册,搜索所需要的呼叫器
			 for (j = 0; j<(CALL_TABLE_NUMBER + CANCEL_TABLE_NUMBER + ALARM_TABLE_NUMBER); j++)//搜索标志位
			{
				IRcvStr(I2C_ADDRESS, j*PAGE_LENGTH, temp_buff1, PAGE_LENGTH);//读出32个字节标志位
				delay10ms();
//				uart_printf("j = %02x \r\n",(unsigned int)j);
				for (i = 0; i<PAGE_LENGTH; i++) //对读出的32字节标志位进行查看，看是否为0
				{
					if (temp_buff1[i] == 0)//标志位为0代表该标志位所对应的数据区有数据
					{	
#ifdef DEBUG
						uart_printf("发现一个标志位 \r\n");
#endif
						IRcvStr(I2C_ADDRESS, CALL_DATA_START + (j * 32 + i) * 8, temp_buff, 8);//读出对应的8个字节的数据
						delay10ms();
						if (((Two_Menu_F8_E1_temp != 1) && temp_buff[5] == old2_RF_RECE_REG[0] && temp_buff[6] == old2_RF_RECE_REG[1] && ((temp_buff[7] >> 4) == (old2_RF_RECE_REG[2] >> 4))) || ((Two_Menu_F8_E1_temp == 1) && temp_buff[5] == old2_RF_RECE_REG[0] && temp_buff[6] == old2_RF_RECE_REG[1] && temp_buff[7] == old2_RF_RECE_REG[2]))// 进行对比，看看数据是否符合
						{
#ifdef DEBUG
							uart_printf("对码成功 \r\n");
							uart_printf("遥控器码是 %02x %02x %02x.\r\n", (unsigned int)old2_RF_RECE_REG[0], (unsigned int)old2_RF_RECE_REG[1], (unsigned int)old2_RF_RECE_REG[2]); //测试按键键值
							uart_printf("标志地址为 %x ! \n\r", (unsigned int)(j*PAGE_LENGTH+i));
							uart_printf("存储地址为 %x ! \n\r", CALL_DATA_START + (j * 32 + i) * 8);
#endif
							temp_buff[7] = old2_RF_RECE_REG[2];
							if (temp_buff[0] < 50)
							{
#ifdef DEBUG
								uart_printf("temp_buff[0] < 50 \r\n");
#endif
								if (Two_Menu_F8_E1_temp == 1)  //为按键值
								{

									temp_buff[0] = single_key[old2_RF_RECE_REG[2] & 0x0f];
								}
								else
								{

									temp_buff[0] = multiple_key[old2_RF_RECE_REG[2] & 0x0f];
								}
							}
#ifdef DEBUG
							uart_printf("cancen funtion \r\n");
#endif
							//语音函数
							if (temp_buff[0] != QUXIAO_1 && temp_buff[0])
							{
#ifdef DEBUG
								uart_printf("cancen funtion fault \r\n");
#endif
								for (l = 0; l < Two_Menu_F6_E2_temp; l++)
								{
									submenuf6_1(Two_Menu_F6_E1_temp, temp_buff, temp_buff[0], old2_RF_RECE_REG[2] & 0x0f);
								}
							}
							else
							{
#ifdef DEBUG
								uart_printf("cancen funtion success \r\n");
#endif
								Cancel_funtion(temp_buff, display_ram);//取消函数
								tm1629_load();
								display();
								break;
								break;
							}

								tm1629_clear();//清屏
								decoder_temp_to_mcuram(display_ram, temp_buff);//如果符合的话  将临时数组的数据移入单片机暂存数组 8字节转6字节
								tm1629_load();//单片机把数组内容载入数码管显存数组中
								display();//显示数码管
#ifdef DEBUG
								uart_printf("decoder success!"); 
#endif

								set_func_index(DECODER_MENU);;//此时跳入解码菜单，为下一次解码做准备
								clear_again_receive_rf_decoder_finished();//清除解码完成标志位
								break;
								break;
						}
					}
				}
			}	
			 break;
		}

		case DECODER_MENU: //解码菜单下
		{
			set_logout_cycle_table(0);//循环跟销号重新计数
			 //键盘规则
			if ((old2_RF_RECE_REG[2] & 0xf0) == 0x00 && (((old2_RF_RECE_REG[0] >> 4) == Two_Menu_F7_E1_temp) || (Two_Menu_F7_E1_temp == 10)))//键盘规则，程序按默认的来编
			{
				if (Two_Menu_F8_E1_temp == 1)  //为按键值
				{
					temp_buff[0] = single_key[old2_RF_RECE_REG[2] & 0x0f];
				}
				else
				{
					temp_buff[0] = multiple_key[old2_RF_RECE_REG[2] & 0x0f];
				}
				temp_buff[1] = old2_RF_RECE_REG[0] >> 4;	//为防区号
				temp_buff[2] = old2_RF_RECE_REG[0] & 0x0f;	//为3位组码第一位
				temp_buff[3] = old2_RF_RECE_REG[1] >> 4;	//为3位组码第二位
				temp_buff[4] = old2_RF_RECE_REG[1] & 0x0f;	//为3位组码第三位
				temp_buff[5] = old2_RF_RECE_REG[0];			//备用项,暂且存第一位ID码
				temp_buff[6] = old2_RF_RECE_REG[1];			//备用项,暂且存第二位ID码
				temp_buff[7] = old2_RF_RECE_REG[2];			//备用项,暂且存第三位ID码
				if (Two_Menu_F3_E1_temp == 1)				//为即时模式
				{	
					if (temp_buff[0] != QUXIAO_1 && temp_buff[0])
					{
						for (l = 0; l < Two_Menu_F6_E2_temp; l++)
						{
							submenuf6_1(Two_Menu_F6_E1_temp, temp_buff, temp_buff[0], old2_RF_RECE_REG[2] & 0x0f);
						}
					}
					else
					{
						Cancel_funtion(temp_buff, display_ram);//取消函数
						tm1629_load();
						display();
						break;
					}

					if (display_ram[1] == temp_buff[1] && display_ram[2] == temp_buff[2] && display_ram[3] == temp_buff[3] && display_ram[4] == temp_buff[4])
					{
						decoder_temp_to_mcuram(display_ram, temp_buff);
					}
					else
					{
						for (l = Two_Menu_F3_E2_temp; l>1; l--) //整体往下移一组数据
						{
							mcuram_to_mcuram_down(display_ram + (l - 2) * 6);
						}
						for (l = 0; l<6; l++) //讲第一组数据清0
						{
							display_ram[l] = 0;
						}
						decoder_temp_to_mcuram(display_ram, temp_buff); //如果符合的话  将临时数组的数据移入单片机暂存数组 8字节转6字节
					}			
					tm1629_load();//单片机把数组内容载入数码管显存数组中
					display();//显示数码管
				}
				else if (Two_Menu_F3_E1_temp == 2)//为循环模式
				{
					for (k = 0; k < Two_Menu_F3_E2_temp; k++)
					{
						if (display_ram[k * 6] == 0)//找出位于队列最后的那个点
						{
							if (display_ram[(k - 1) * 6 + 1] == temp_buff[1] && display_ram[(k - 1) * 6 + 2] == temp_buff[2] && display_ram[(k - 1) * 6 + 3] == temp_buff[3] && display_ram[(k - 1) * 6 + 4] == temp_buff[4])
							{
								decoder_temp_to_mcuram(display_ram + (k-1) * 6, temp_buff);
							}
							else
							{
								decoder_temp_to_mcuram(display_ram + k * 6, temp_buff);//如果符合的话  将临时数组的数据移入单片机暂存数组 8字节转6字节
							}	
							tm1629_load();//单片机把数组内容载入数码管显存数组中
							display();//显示数码管
							break;
						}
					}

				}
				//clear_again_receive_rf_decoder_finished();//清除解码完成标志位
				break;
			}
			//呼叫器注册,搜索所需要的呼叫器
			for (j = 0; j<CALL_TABLE_NUMBER + ALARM_TABLE_NUMBER + CANCEL_TABLE_NUMBER; j++)//搜索标志位
			{
				IRcvStr(I2C_ADDRESS, j*PAGE_LENGTH, temp_buff1, PAGE_LENGTH);//读出32个字节标志位
				delay10ms();
				for (i = 0; i<PAGE_LENGTH; i++)
				{
					if (temp_buff1[i] == 0)//对读出的32字节标志位进行查看，看是否为0
					{
						IRcvStr(I2C_ADDRESS, CALL_DATA_START + (j * 32 + i) * 8, temp_buff, 8);//读出对应的8个字节的数据
						delay10ms();
						if (((Two_Menu_F8_E1_temp != 1) && temp_buff[5] == old2_RF_RECE_REG[0] && temp_buff[6] == old2_RF_RECE_REG[1] && ((temp_buff[7] >> 4) == (old2_RF_RECE_REG[2] >> 4))) || ((Two_Menu_F8_E1_temp == 1) && temp_buff[5] == old2_RF_RECE_REG[0] && temp_buff[6] == old2_RF_RECE_REG[1] && temp_buff[7] == old2_RF_RECE_REG[2]))
						{
#ifdef DEBUG
							uart_printf("你好 \r\n");
#endif
							temp_buff[7] = old2_RF_RECE_REG[2];
							if (temp_buff[0] < 50)
							{
								if (Two_Menu_F8_E1_temp == 1)  //为按键值
								{

									temp_buff[0] = single_key[old2_RF_RECE_REG[2] & 0x0f];
								}
								else
								{

									temp_buff[0] = multiple_key[old2_RF_RECE_REG[2] & 0x0f];
								}
							}
			
							if (Two_Menu_F3_E1_temp == 1)//为即时模式
							{

								if (temp_buff[0] != QUXIAO_1 && temp_buff[0])
								{
#ifdef DEBUG
									uart_printf("cancen funtion fault \r\n");
#endif
									for (l = 0; l < Two_Menu_F6_E2_temp; l++)
									{
										submenuf6_1(Two_Menu_F6_E1_temp, temp_buff, temp_buff[0], old2_RF_RECE_REG[2] & 0x0f);
									}
								}
								else
								{
#ifdef DEBUG
									uart_printf("cancen funtion success \r\n");
#endif
									Cancel_funtion(temp_buff, display_ram);//取消函数
									tm1629_load();
									display();
									break;
									break;
								}

								if (display_ram[1] == temp_buff[1] && display_ram[2] == temp_buff[2] && display_ram[3] == temp_buff[3] && display_ram[4] == temp_buff[4])
								{
									decoder_temp_to_mcuram(display_ram, temp_buff);
								}
								else
								{
									for (l = Two_Menu_F3_E2_temp; l>1; l--) //整体往下移一组数据
									{
										mcuram_to_mcuram_down(display_ram + (l - 2) * 6);
									}
									for (l = 0; l<6; l++) //讲第一组数据清0
									{
										display_ram[l] = 0;
									}
									decoder_temp_to_mcuram(display_ram, temp_buff); //如果符合的话  将临时数组的数据移入单片机暂存数组 8字节转6字节
								}
								tm1629_load();//单片机把数组内容载入数码管显存数组中
								display();//显示数码管
							}
						else if (Two_Menu_F3_E1_temp == 2)//为循环模式
						{
							for (k = 0; k < Two_Menu_F3_E2_temp; k++)
							{
								if (display_ram[k * 6 ] == 0)//找出位于队列最后的那个点
								{
									if (display_ram[(k - 1) * 6 + 1] == temp_buff[1] && display_ram[(k - 1) * 6 + 2] == temp_buff[2] && display_ram[(k - 1) * 6 + 3] == temp_buff[3] && display_ram[(k - 1) * 6 + 4] == temp_buff[4])
									{
										decoder_temp_to_mcuram(display_ram + (k - 1) * 6, temp_buff);
									}
									else
									{
										decoder_temp_to_mcuram(display_ram + k * 6, temp_buff);//如果符合的话  将临时数组的数据移入单片机暂存数组 8字节转6字节
									}
									tm1629_load();//单片机把数组内容载入数码管显存数组中
									display();//显示数码管
									break;
								}
							}
						}
#ifdef DEBUG
							uart_printf("decoder success!");
#endif

#ifdef DEBUG
							uart_printf("display_ram is %02x %02x %02x %02x.\r\n", (unsigned int)display_ram[0], (unsigned int)display_ram[1], (unsigned int)display_ram[2], (unsigned int)display_ram[3]); //测试按键键值
#endif
							clear_again_receive_rf_decoder_finished();
							break;
							break;
						}
					}
				}
			}
			break;
		}

		case TWO_MENU_F1_E1_D1:
		case TWO_MENU_F1_E1_D2:
		case TWO_MENU_F1_E1_D3:
		case TWO_MENU_F1_E1_D4:
		{
			set_func_index(TWO_MENU_F1_E1_D4);
			buf_eeprom[0] = old2_RF_RECE_REG[2] & 0x0f;//按键值保存到0字节
			buf_eeprom[1] = Two_Menu_F1_E1[0];/*数值存入1 2 3 4字节*/
			buf_eeprom[2] = Two_Menu_F1_E1[1];
			buf_eeprom[3] = Two_Menu_F1_E1[2];
			buf_eeprom[4] = Two_Menu_F1_E1[3];
			buf_eeprom[5] = old2_RF_RECE_REG[0];// ID码存入 5 6 7 字节
			buf_eeprom[6] = old2_RF_RECE_REG[1];
			if (Two_Menu_F8_E1_temp == 1)
			{
				buf_eeprom[7] = old2_RF_RECE_REG[2];
			}
			else
			{
				buf_eeprom[7] = old2_RF_RECE_REG[2] & 0xf0;
			}

			if (register_call_function(buf_eeprom))//数值自动加1
			{
#ifdef DEBUG
				uart_printf("call is %02x %02x %02x .\r\n", (unsigned int)buf_eeprom[5], (unsigned int)buf_eeprom[6], (unsigned int)buf_eeprom[7]); //测试按键键值
				uart_printf("call is %02x %02x %02x %02x.\r\n", (unsigned int)Two_Menu_F1_E1[0], (unsigned int)Two_Menu_F1_E1[1], (unsigned int)Two_Menu_F1_E1[2], (unsigned int)Two_Menu_F1_E1[3]);
				uart_printf("caller is rigister success \r\n");
#endif					
				delay10ms();
				if (Two_Menu_F1_E1[3] == 9)
				{
					if (Two_Menu_F1_E1[2] == 9)
					{
						if (Two_Menu_F1_E1[1] == 9)
						{
							Two_Menu_F1_E1[0]++;
							Two_Menu_F1_E1[1] = 0;
							Two_Menu_F1_E1[2] = 0;
							if (Two_Menu_F1_E1[0] == 21)
							{
								Two_Menu_F1_E1[0] = 0;
								Two_Menu_F1_E1[3] = 1;
							}
							else
							{
								Two_Menu_F1_E1[3] = 1;
							}
						}
						else
						{
							Two_Menu_F1_E1[1]++;
							Two_Menu_F1_E1[2] = 0;
							Two_Menu_F1_E1[3] = 0;
						}
					}
					else
					{
						Two_Menu_F1_E1[2]++;
						Two_Menu_F1_E1[3] = 0;
					}
				}
				else
					Two_Menu_F1_E1[3]++;
			}
			break;
		}

		case TWO_MENU_F1_E2_D1:
		case TWO_MENU_F1_E2_D2:
		case TWO_MENU_F1_E2_D3:
		case TWO_MENU_F1_E2_D4:
		{
								  set_func_index(TWO_MENU_F1_E2_D4);
								  buf_eeprom[0] = old2_RF_RECE_REG[2] & 0x0f;//按键值保存到0字节
								  buf_eeprom[1] = Two_Menu_F1_E2[0];/*数值存入1 2 3 4字节*/
								  buf_eeprom[2] = Two_Menu_F1_E2[1];
								  buf_eeprom[3] = Two_Menu_F1_E2[2];
								  buf_eeprom[4] = Two_Menu_F1_E2[3];
								  buf_eeprom[5] = old2_RF_RECE_REG[0];// ID码存入 5 6 7 字节
								  buf_eeprom[6] = old2_RF_RECE_REG[1];
								  buf_eeprom[7] = old2_RF_RECE_REG[2] & 0xf0;

								  if (register_host_function(buf_eeprom))//数值自动加1
								  {
#ifdef DEBUG
									  uart_printf("host is %02x %02x %02x .\r\n", (unsigned int)buf_eeprom[5], (unsigned int)buf_eeprom[6], (unsigned int)buf_eeprom[7]); //测试按键键值
									  uart_printf("host is %02x %02x %02x %02x.\r\n", (unsigned int)Two_Menu_F1_E2[0], (unsigned int)Two_Menu_F1_E2[1], (unsigned int)Two_Menu_F1_E2[2], (unsigned int)Two_Menu_F1_E2[3]);
									  uart_printf("host is rigister success \r\n");
#endif					
									  delay10ms();
									  if (Two_Menu_F1_E2[3] == 9)
									  {
										  if (Two_Menu_F1_E2[2] == 9)
										  {
											  if (Two_Menu_F1_E2[1] == 9)
											  {
												  Two_Menu_F1_E2[0]++;
												  Two_Menu_F1_E2[1] = 0;
												  Two_Menu_F1_E2[2] = 0;
												  if (Two_Menu_F1_E2[0] == 21)
												  {
													  Two_Menu_F1_E2[0] = 0;
													  Two_Menu_F1_E2[3] = 1;
												  }
												  else
												  {
													  Two_Menu_F1_E2[3] = 1;
												  }


											  }
											  else
											  {
												  Two_Menu_F1_E2[1]++;
												  Two_Menu_F1_E2[2] = 0;
												  Two_Menu_F1_E2[3] = 0;
											  }
										  }
										  else
										  {
											  Two_Menu_F1_E2[2]++;
											  Two_Menu_F1_E2[3] = 0;
										  }
									  }
									  else
										  Two_Menu_F1_E2[3]++;
								  }
								  break;
		}

		case TWO_MENU_F1_E3_D1:
		case TWO_MENU_F1_E3_D2:
		case TWO_MENU_F1_E3_D3:
		case TWO_MENU_F1_E3_D4:
		{
			set_func_index(TWO_MENU_F1_E3_D4);
			buf_eeprom[0] = BAOJING_1;//按键值保存到0字节
			buf_eeprom[1] = Two_Menu_F1_E3[0];/*数值存入1 2 3 4字节*/
			buf_eeprom[2] = Two_Menu_F1_E3[1];
			buf_eeprom[3] = Two_Menu_F1_E3[2];
			buf_eeprom[4] = Two_Menu_F1_E3[3];
			buf_eeprom[5] = old2_RF_RECE_REG[0];// ID码存入 5 6 7 字节
			buf_eeprom[6] = old2_RF_RECE_REG[1];
			buf_eeprom[7] = old2_RF_RECE_REG[2] & 0xf0;

			if (register_alarm_function(buf_eeprom))//数值自动加1
			{
#ifdef DEBUG
				uart_printf("alarm is %02x %02x %02x .\r\n", (unsigned int)buf_eeprom[5], (unsigned int)buf_eeprom[6], (unsigned int)buf_eeprom[7]); //测试按键键值
				uart_printf("alarm is %02x %02x %02x %02x.\r\n", (unsigned int)Two_Menu_F1_E3[0], (unsigned int)Two_Menu_F1_E3[1], (unsigned int)Two_Menu_F1_E3[2], (unsigned int)Two_Menu_F1_E3[3]);
				uart_printf("alarm is rigister success \r\n");
#endif					
				delay10ms();
				if (Two_Menu_F1_E3[3] == 9)
				{
					if (Two_Menu_F1_E3[2] == 9)
					{
						if (Two_Menu_F1_E3[1] == 9)
						{
							Two_Menu_F1_E3[0]++;
							Two_Menu_F1_E3[1] = 0;
							Two_Menu_F1_E3[2] = 0;
							if (Two_Menu_F1_E3[0] == 21)
							{
								Two_Menu_F1_E3[0] = 0;
								Two_Menu_F1_E3[3] = 1;
							}
							else
							{
								Two_Menu_F1_E3[3] = 1;
							}
						}
						else
						{
							Two_Menu_F1_E3[1]++;
							Two_Menu_F1_E3[2] = 0;
							Two_Menu_F1_E3[3] = 0;
						}
					}
					else
					{
						Two_Menu_F1_E3[2]++;
						Two_Menu_F1_E3[3] = 0;
					}
				}
				else
					Two_Menu_F1_E3[3]++;
			}
			break;
		}
		case TWO_MENU_F1_E4_D1:
		case TWO_MENU_F1_E4_D2:
		case TWO_MENU_F1_E4_D3:
		case TWO_MENU_F1_E4_D4:
		{
			set_func_index(TWO_MENU_F1_E4_D4);
			buf_eeprom[0] = QUXIAO_1;//按键值保存到0字节
			buf_eeprom[1] = Two_Menu_F1_E4[0];/*数值存入1 2 3 4字节*/
			buf_eeprom[2] = Two_Menu_F1_E4[1];
			buf_eeprom[3] = Two_Menu_F1_E4[2];
			buf_eeprom[4] = Two_Menu_F1_E4[3];
			buf_eeprom[5] = old2_RF_RECE_REG[0];// ID码存入 5 6 7 字节
			buf_eeprom[6] = old2_RF_RECE_REG[1];
			buf_eeprom[7] = old2_RF_RECE_REG[2] & 0xf0;

			if (register_cancel_function(buf_eeprom))//数值自动加1
			{
#ifdef DEBUG
				uart_printf("cancel is %02x %02x %02x .\r\n", (unsigned int)buf_eeprom[5], (unsigned int)buf_eeprom[6], (unsigned int)buf_eeprom[7]); //测试按键键值
				uart_printf("cancel is %02x %02x %02x %02x.\r\n", (unsigned int)Two_Menu_F1_E4[0], (unsigned int)Two_Menu_F1_E4[1], (unsigned int)Two_Menu_F1_E4[2], (unsigned int)Two_Menu_F1_E4[3]);
				uart_printf("cancel is rigister success \r\n");
#endif					
				delay10ms();
				if (Two_Menu_F1_E4[3] == 9)
				{
					if (Two_Menu_F1_E4[2] == 9)
					{
						if (Two_Menu_F1_E4[1] == 9)
						{
							Two_Menu_F1_E4[0]++;
							Two_Menu_F1_E4[1] = 0;
							Two_Menu_F1_E4[2] = 0;
							if (Two_Menu_F1_E4[0] == 21)
							{
								Two_Menu_F1_E4[0] = 0;
								Two_Menu_F1_E4[3] = 1;
							}
							else
							{
								Two_Menu_F1_E4[3] = 1;
							}
						}
						else
						{
							Two_Menu_F1_E4[1]++;
							Two_Menu_F1_E4[2] = 0;
							Two_Menu_F1_E4[3] = 0;
						}
					}
					else
					{
						Two_Menu_F1_E4[2]++;
						Two_Menu_F1_E4[3] = 0;
					}
				}
				else
					Two_Menu_F1_E4[3]++;
			}
			break;
		}
		case TWO_MENU_F8_E2_SET :

			if (Two_Menu_F8_E1_temp == 1)
			{
				single_key[old2_RF_RECE_REG[2] & 0x0f] = Two_Menu_F8_E2_temp;
			}
			else
			{
				multiple_key[old2_RF_RECE_REG[2] & 0x0f] = Two_Menu_F8_E2_temp;
			}
#ifdef DEBUG
			uart_printf("single_key：  ");
			for (i = 0; i < 16; i++)
			{
				
				uart_printf("%02d ", (unsigned int)single_key[i]);
			}
			uart_printf(" \r\n");

			uart_printf("multiple_key：");
			for (i = 0; i < 16; i++)
			{

				uart_printf("%2d ", (unsigned int)multiple_key[i]);
			}
			uart_printf(" \r\n");
#endif	

			break;


		default:break;//默认的break
		}
		clear_again_receive_rf_decoder_finished();//清除标志位
	}
	
	LogoutProcess();
	CycleProcess();
}
