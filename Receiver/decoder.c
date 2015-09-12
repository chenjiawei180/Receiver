#include "decoder.h"
#include "ev1527.h"
#include "usart.h"
#include "at24c256.h"
#include "key.h"
#include "menu.h"

unsigned char buf_eeprom[8] = { 0 };//写入EEPROM_buf

void DecoderProcess(void)
{
	unsigned char func_index_temp = 0;
	unsigned char Two_Menu_F8_E1_temp = 0;
	func_index_temp = return_func_index();
	Two_Menu_F8_E1_temp = return_Two_Menu_F8_E1();
	receive_rf_decoder();
	if (return_again_receive_rf_decoder_finished() == 1)
	{
#ifdef DEBUG
		uart_printf("decoder_val is %02x %02x %02x .\r\n", (unsigned int)old2_RF_RECE_REG[0], (unsigned int)old2_RF_RECE_REG[1], (unsigned int)old2_RF_RECE_REG[2]); //测试按键键值
#endif
		switch (func_index_temp)
		{
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
								  buf_eeprom[0] = old2_RF_RECE_REG[2] & 0x0f;//按键值保存到0字节
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
										  Two_Menu_F1_E2[3]++;
								  }
								  break;
		}
		case TWO_MENU_F1_E4_D1:
		case TWO_MENU_F1_E4_D2:
		case TWO_MENU_F1_E4_D3:
		case TWO_MENU_F1_E4_D4:
		{
								  set_func_index(TWO_MENU_F1_E4_D4);
								  buf_eeprom[0] = old2_RF_RECE_REG[2] & 0x0f;//按键值保存到0字节
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
		default:break;
		}
	}
	clear_again_receive_rf_decoder_finished();//清除标志位
}
