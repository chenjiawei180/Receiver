#include "decoder.h"
#include "ev1527.h"
#include "usart.h"
#include "at24c256.h"
#include "key.h"
#include "menu.h"
#include "tm1629.h"
#include "timer.h"
#include "gd5800.h"

unsigned char buf_eeprom[8] = { 0 };//д��EEPROM_buf

void DecoderProcess(void)
{	
	unsigned char i, j ,l ,k;
	unsigned char temp_buff[8];//��������ʱ����
	unsigned char temp_buff1[PAGE_LENGTH] = { 0 };//��ѯAT24C256��ʱ����

	unsigned char func_index_temp = 0;		//������ʱ���������ڷŻ������ⲿ����
	unsigned char Two_Menu_F8_E1_temp = 0;	//������ʱ���������ڷŻ������ⲿ����
	unsigned char Two_Menu_F8_E2_temp = 0;	//������ʱ���������ڷŻ������ⲿ����
	unsigned char Two_Menu_F3_E1_temp = 0;	//������ʱ���������ڷŻ������ⲿ����	
	unsigned char Two_Menu_F3_E2_temp = 0;	//������ʱ���������ڷŻ������ⲿ����
	unsigned char Two_Menu_F7_E1_temp = 0;	//������ʱ���������ڷŻ������ⲿ����
	unsigned char Two_Menu_F6_E1_temp = 0;	//������ʱ���������ڷŻ������ⲿ����
	unsigned char Two_Menu_F6_E2_temp = 0;	//������ʱ���������ڷŻ������ⲿ����

	func_index_temp = return_func_index();	//��������Ҫ���ⲿ����
	Two_Menu_F8_E1_temp = return_Two_Menu_F8_E1();	//��������Ҫ���ⲿ����
	Two_Menu_F8_E2_temp = return_Two_Menu_F8_E2();	//��������Ҫ���ⲿ����
	Two_Menu_F3_E1_temp = return_Two_Menu_F3_E1();	//��������Ҫ���ⲿ����
	Two_Menu_F3_E2_temp = return_Two_Menu_F3_E2();	//��������Ҫ���ⲿ����
	Two_Menu_F7_E1_temp = return_Two_Menu_F7_E1();	//��������Ҫ���ⲿ����
	Two_Menu_F6_E1_temp = return_Two_Menu_F6_E1();	//��������Ҫ���ⲿ����
	Two_Menu_F6_E2_temp = return_Two_Menu_F6_E2();	//��������Ҫ���ⲿ����

	receive_rf_decoder();	//���뺯��

	if (return_again_receive_rf_decoder_finished() == 1) //��־λ����1 ˵����2�μ�����ͨ��,���յ���Ч��
	{
#ifdef DEBUG
		uart_printf("decoder_val is %02x %02x %02x .\r\n", (unsigned int)old2_RF_RECE_REG[0], (unsigned int)old2_RF_RECE_REG[1], (unsigned int)old2_RF_RECE_REG[2]); //���԰�����ֵ
#endif
		switch (func_index_temp)
		{
		case MENU_STANDBY://����״̬��
		{
			set_logout_cycle_table(0);//ѭ�����������¼���
			//���̹���
			if ((old2_RF_RECE_REG[2] & 0xf0) == 0x00 && (((old2_RF_RECE_REG[0] >> 4) == Two_Menu_F7_E1_temp) || (Two_Menu_F7_E1_temp == 10)))//���̹��򣬳���Ĭ�ϵ�����
			{
				if (Two_Menu_F8_E1_temp == 1)  //Ϊ����ֵ
				{
					temp_buff[0] = single_key[old2_RF_RECE_REG[2] & 0x0f] ;
				}
				else
				{
					temp_buff[0] = multiple_key[old2_RF_RECE_REG[2] & 0x0f];
				}   
				temp_buff[1] = old2_RF_RECE_REG[0] >> 4;	//Ϊ������
				temp_buff[2] = old2_RF_RECE_REG[0] & 0x0f;	//Ϊ3λ�����һλ
				temp_buff[3] = old2_RF_RECE_REG[1] >> 4;	//Ϊ3λ����ڶ�λ
				temp_buff[4] = old2_RF_RECE_REG[1] & 0x0f;	//Ϊ3λ�������λ
				temp_buff[5] = old2_RF_RECE_REG[0];			//������,���Ҵ��1λID��
				temp_buff[6] = old2_RF_RECE_REG[1];			//������,���Ҵ��2λID��
				temp_buff[7] = old2_RF_RECE_REG[2];			//������,���Ҵ��3λID��

				if (temp_buff[0] != QUXIAO_1 && temp_buff[0])
				{
					for (l = 0; l < Two_Menu_F6_E2_temp; l++)
					{
						submenuf6_1(Two_Menu_F6_E1_temp, temp_buff, temp_buff[0], old2_RF_RECE_REG[2] & 0x0f);
					}
				}
				else
				{
					Cancel_funtion(temp_buff,display_ram);//ȡ������
					tm1629_load();
					display();
					break;
				}		

				tm1629_clear();//����
				decoder_temp_to_mcuram(display_ram, temp_buff);//����ʱ������������뵥Ƭ���ݴ����� 8�ֽ�ת6�ֽ�
				tm1629_load();//��Ƭ����������������������Դ�������
				display();//��ʾ�����
				set_func_index(DECODER_MENU);//��ʱ�������˵���Ϊ��һ�ν�����׼��
				//clear_again_receive_rf_decoder_finished();//���������ɱ�־λ

				break;
			}
			//������ע��,��������Ҫ�ĺ�����
			 for (j = 0; j<(CALL_TABLE_NUMBER + CANCEL_TABLE_NUMBER + ALARM_TABLE_NUMBER); j++)//������־λ
			{
				IRcvStr(I2C_ADDRESS, j*PAGE_LENGTH, temp_buff1, PAGE_LENGTH);//����32���ֽڱ�־λ
				delay10ms();
//				uart_printf("j = %02x \r\n",(unsigned int)j);
				for (i = 0; i<PAGE_LENGTH; i++) //�Զ�����32�ֽڱ�־λ���в鿴�����Ƿ�Ϊ0
				{
					if (temp_buff1[i] == 0)//��־λΪ0����ñ�־λ����Ӧ��������������
					{	
#ifdef DEBUG
						uart_printf("����һ����־λ \r\n");
#endif
						IRcvStr(I2C_ADDRESS, CALL_DATA_START + (j * 32 + i) * 8, temp_buff, 8);//������Ӧ��8���ֽڵ�����
						delay10ms();
						if (((Two_Menu_F8_E1_temp != 1) && temp_buff[5] == old2_RF_RECE_REG[0] && temp_buff[6] == old2_RF_RECE_REG[1] && ((temp_buff[7] >> 4) == (old2_RF_RECE_REG[2] >> 4))) || ((Two_Menu_F8_E1_temp == 1) && temp_buff[5] == old2_RF_RECE_REG[0] && temp_buff[6] == old2_RF_RECE_REG[1] && temp_buff[7] == old2_RF_RECE_REG[2]))// ���жԱȣ����������Ƿ����
						{
#ifdef DEBUG
							uart_printf("����ɹ� \r\n");
							uart_printf("ң�������� %02x %02x %02x.\r\n", (unsigned int)old2_RF_RECE_REG[0], (unsigned int)old2_RF_RECE_REG[1], (unsigned int)old2_RF_RECE_REG[2]); //���԰�����ֵ
							uart_printf("��־��ַΪ %x ! \n\r", (unsigned int)(j*PAGE_LENGTH+i));
							uart_printf("�洢��ַΪ %x ! \n\r", CALL_DATA_START + (j * 32 + i) * 8);
#endif
							temp_buff[7] = old2_RF_RECE_REG[2];
							if (temp_buff[0] < 50)
							{
#ifdef DEBUG
								uart_printf("temp_buff[0] < 50 \r\n");
#endif
								if (Two_Menu_F8_E1_temp == 1)  //Ϊ����ֵ
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
							//��������
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
								Cancel_funtion(temp_buff, display_ram);//ȡ������
								tm1629_load();
								display();
								break;
								break;
							}

								tm1629_clear();//����
								decoder_temp_to_mcuram(display_ram, temp_buff);//������ϵĻ�  ����ʱ������������뵥Ƭ���ݴ����� 8�ֽ�ת6�ֽ�
								tm1629_load();//��Ƭ����������������������Դ�������
								display();//��ʾ�����
#ifdef DEBUG
								uart_printf("decoder success!"); 
#endif

								set_func_index(DECODER_MENU);;//��ʱ�������˵���Ϊ��һ�ν�����׼��
								clear_again_receive_rf_decoder_finished();//���������ɱ�־λ
								break;
								break;
						}
					}
				}
			}	
			 break;
		}

		case DECODER_MENU: //����˵���
		{
			set_logout_cycle_table(0);//ѭ�����������¼���
			 //���̹���
			if ((old2_RF_RECE_REG[2] & 0xf0) == 0x00 && (((old2_RF_RECE_REG[0] >> 4) == Two_Menu_F7_E1_temp) || (Two_Menu_F7_E1_temp == 10)))//���̹��򣬳���Ĭ�ϵ�����
			{
				if (Two_Menu_F8_E1_temp == 1)  //Ϊ����ֵ
				{
					temp_buff[0] = single_key[old2_RF_RECE_REG[2] & 0x0f];
				}
				else
				{
					temp_buff[0] = multiple_key[old2_RF_RECE_REG[2] & 0x0f];
				}
				temp_buff[1] = old2_RF_RECE_REG[0] >> 4;	//Ϊ������
				temp_buff[2] = old2_RF_RECE_REG[0] & 0x0f;	//Ϊ3λ�����һλ
				temp_buff[3] = old2_RF_RECE_REG[1] >> 4;	//Ϊ3λ����ڶ�λ
				temp_buff[4] = old2_RF_RECE_REG[1] & 0x0f;	//Ϊ3λ�������λ
				temp_buff[5] = old2_RF_RECE_REG[0];			//������,���Ҵ��һλID��
				temp_buff[6] = old2_RF_RECE_REG[1];			//������,���Ҵ�ڶ�λID��
				temp_buff[7] = old2_RF_RECE_REG[2];			//������,���Ҵ����λID��
				if (Two_Menu_F3_E1_temp == 1)				//Ϊ��ʱģʽ
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
						Cancel_funtion(temp_buff, display_ram);//ȡ������
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
						for (l = Two_Menu_F3_E2_temp; l>1; l--) //����������һ������
						{
							mcuram_to_mcuram_down(display_ram + (l - 2) * 6);
						}
						for (l = 0; l<6; l++) //����һ��������0
						{
							display_ram[l] = 0;
						}
						decoder_temp_to_mcuram(display_ram, temp_buff); //������ϵĻ�  ����ʱ������������뵥Ƭ���ݴ����� 8�ֽ�ת6�ֽ�
					}			
					tm1629_load();//��Ƭ����������������������Դ�������
					display();//��ʾ�����
				}
				else if (Two_Menu_F3_E1_temp == 2)//Ϊѭ��ģʽ
				{
					for (k = 0; k < Two_Menu_F3_E2_temp; k++)
					{
						if (display_ram[k * 6] == 0)//�ҳ�λ�ڶ��������Ǹ���
						{
							if (display_ram[(k - 1) * 6 + 1] == temp_buff[1] && display_ram[(k - 1) * 6 + 2] == temp_buff[2] && display_ram[(k - 1) * 6 + 3] == temp_buff[3] && display_ram[(k - 1) * 6 + 4] == temp_buff[4])
							{
								decoder_temp_to_mcuram(display_ram + (k-1) * 6, temp_buff);
							}
							else
							{
								decoder_temp_to_mcuram(display_ram + k * 6, temp_buff);//������ϵĻ�  ����ʱ������������뵥Ƭ���ݴ����� 8�ֽ�ת6�ֽ�
							}	
							tm1629_load();//��Ƭ����������������������Դ�������
							display();//��ʾ�����
							break;
						}
					}

				}
				//clear_again_receive_rf_decoder_finished();//���������ɱ�־λ
				break;
			}
			//������ע��,��������Ҫ�ĺ�����
			for (j = 0; j<CALL_TABLE_NUMBER + ALARM_TABLE_NUMBER + CANCEL_TABLE_NUMBER; j++)//������־λ
			{
				IRcvStr(I2C_ADDRESS, j*PAGE_LENGTH, temp_buff1, PAGE_LENGTH);//����32���ֽڱ�־λ
				delay10ms();
				for (i = 0; i<PAGE_LENGTH; i++)
				{
					if (temp_buff1[i] == 0)//�Զ�����32�ֽڱ�־λ���в鿴�����Ƿ�Ϊ0
					{
						IRcvStr(I2C_ADDRESS, CALL_DATA_START + (j * 32 + i) * 8, temp_buff, 8);//������Ӧ��8���ֽڵ�����
						delay10ms();
						if (((Two_Menu_F8_E1_temp != 1) && temp_buff[5] == old2_RF_RECE_REG[0] && temp_buff[6] == old2_RF_RECE_REG[1] && ((temp_buff[7] >> 4) == (old2_RF_RECE_REG[2] >> 4))) || ((Two_Menu_F8_E1_temp == 1) && temp_buff[5] == old2_RF_RECE_REG[0] && temp_buff[6] == old2_RF_RECE_REG[1] && temp_buff[7] == old2_RF_RECE_REG[2]))
						{
#ifdef DEBUG
							uart_printf("��� \r\n");
#endif
							temp_buff[7] = old2_RF_RECE_REG[2];
							if (temp_buff[0] < 50)
							{
								if (Two_Menu_F8_E1_temp == 1)  //Ϊ����ֵ
								{

									temp_buff[0] = single_key[old2_RF_RECE_REG[2] & 0x0f];
								}
								else
								{

									temp_buff[0] = multiple_key[old2_RF_RECE_REG[2] & 0x0f];
								}
							}
			
							if (Two_Menu_F3_E1_temp == 1)//Ϊ��ʱģʽ
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
									Cancel_funtion(temp_buff, display_ram);//ȡ������
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
									for (l = Two_Menu_F3_E2_temp; l>1; l--) //����������һ������
									{
										mcuram_to_mcuram_down(display_ram + (l - 2) * 6);
									}
									for (l = 0; l<6; l++) //����һ��������0
									{
										display_ram[l] = 0;
									}
									decoder_temp_to_mcuram(display_ram, temp_buff); //������ϵĻ�  ����ʱ������������뵥Ƭ���ݴ����� 8�ֽ�ת6�ֽ�
								}
								tm1629_load();//��Ƭ����������������������Դ�������
								display();//��ʾ�����
							}
						else if (Two_Menu_F3_E1_temp == 2)//Ϊѭ��ģʽ
						{
							for (k = 0; k < Two_Menu_F3_E2_temp; k++)
							{
								if (display_ram[k * 6 ] == 0)//�ҳ�λ�ڶ��������Ǹ���
								{
									if (display_ram[(k - 1) * 6 + 1] == temp_buff[1] && display_ram[(k - 1) * 6 + 2] == temp_buff[2] && display_ram[(k - 1) * 6 + 3] == temp_buff[3] && display_ram[(k - 1) * 6 + 4] == temp_buff[4])
									{
										decoder_temp_to_mcuram(display_ram + (k - 1) * 6, temp_buff);
									}
									else
									{
										decoder_temp_to_mcuram(display_ram + k * 6, temp_buff);//������ϵĻ�  ����ʱ������������뵥Ƭ���ݴ����� 8�ֽ�ת6�ֽ�
									}
									tm1629_load();//��Ƭ����������������������Դ�������
									display();//��ʾ�����
									break;
								}
							}
						}
#ifdef DEBUG
							uart_printf("decoder success!");
#endif

#ifdef DEBUG
							uart_printf("display_ram is %02x %02x %02x %02x.\r\n", (unsigned int)display_ram[0], (unsigned int)display_ram[1], (unsigned int)display_ram[2], (unsigned int)display_ram[3]); //���԰�����ֵ
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
			buf_eeprom[0] = old2_RF_RECE_REG[2] & 0x0f;//����ֵ���浽0�ֽ�
			buf_eeprom[1] = Two_Menu_F1_E1[0];/*��ֵ����1 2 3 4�ֽ�*/
			buf_eeprom[2] = Two_Menu_F1_E1[1];
			buf_eeprom[3] = Two_Menu_F1_E1[2];
			buf_eeprom[4] = Two_Menu_F1_E1[3];
			buf_eeprom[5] = old2_RF_RECE_REG[0];// ID����� 5 6 7 �ֽ�
			buf_eeprom[6] = old2_RF_RECE_REG[1];
			if (Two_Menu_F8_E1_temp == 1)
			{
				buf_eeprom[7] = old2_RF_RECE_REG[2];
			}
			else
			{
				buf_eeprom[7] = old2_RF_RECE_REG[2] & 0xf0;
			}

			if (register_call_function(buf_eeprom))//��ֵ�Զ���1
			{
#ifdef DEBUG
				uart_printf("call is %02x %02x %02x .\r\n", (unsigned int)buf_eeprom[5], (unsigned int)buf_eeprom[6], (unsigned int)buf_eeprom[7]); //���԰�����ֵ
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
								  buf_eeprom[0] = old2_RF_RECE_REG[2] & 0x0f;//����ֵ���浽0�ֽ�
								  buf_eeprom[1] = Two_Menu_F1_E2[0];/*��ֵ����1 2 3 4�ֽ�*/
								  buf_eeprom[2] = Two_Menu_F1_E2[1];
								  buf_eeprom[3] = Two_Menu_F1_E2[2];
								  buf_eeprom[4] = Two_Menu_F1_E2[3];
								  buf_eeprom[5] = old2_RF_RECE_REG[0];// ID����� 5 6 7 �ֽ�
								  buf_eeprom[6] = old2_RF_RECE_REG[1];
								  buf_eeprom[7] = old2_RF_RECE_REG[2] & 0xf0;

								  if (register_host_function(buf_eeprom))//��ֵ�Զ���1
								  {
#ifdef DEBUG
									  uart_printf("host is %02x %02x %02x .\r\n", (unsigned int)buf_eeprom[5], (unsigned int)buf_eeprom[6], (unsigned int)buf_eeprom[7]); //���԰�����ֵ
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
			buf_eeprom[0] = BAOJING_1;//����ֵ���浽0�ֽ�
			buf_eeprom[1] = Two_Menu_F1_E3[0];/*��ֵ����1 2 3 4�ֽ�*/
			buf_eeprom[2] = Two_Menu_F1_E3[1];
			buf_eeprom[3] = Two_Menu_F1_E3[2];
			buf_eeprom[4] = Two_Menu_F1_E3[3];
			buf_eeprom[5] = old2_RF_RECE_REG[0];// ID����� 5 6 7 �ֽ�
			buf_eeprom[6] = old2_RF_RECE_REG[1];
			buf_eeprom[7] = old2_RF_RECE_REG[2] & 0xf0;

			if (register_alarm_function(buf_eeprom))//��ֵ�Զ���1
			{
#ifdef DEBUG
				uart_printf("alarm is %02x %02x %02x .\r\n", (unsigned int)buf_eeprom[5], (unsigned int)buf_eeprom[6], (unsigned int)buf_eeprom[7]); //���԰�����ֵ
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
			buf_eeprom[0] = QUXIAO_1;//����ֵ���浽0�ֽ�
			buf_eeprom[1] = Two_Menu_F1_E4[0];/*��ֵ����1 2 3 4�ֽ�*/
			buf_eeprom[2] = Two_Menu_F1_E4[1];
			buf_eeprom[3] = Two_Menu_F1_E4[2];
			buf_eeprom[4] = Two_Menu_F1_E4[3];
			buf_eeprom[5] = old2_RF_RECE_REG[0];// ID����� 5 6 7 �ֽ�
			buf_eeprom[6] = old2_RF_RECE_REG[1];
			buf_eeprom[7] = old2_RF_RECE_REG[2] & 0xf0;

			if (register_cancel_function(buf_eeprom))//��ֵ�Զ���1
			{
#ifdef DEBUG
				uart_printf("cancel is %02x %02x %02x .\r\n", (unsigned int)buf_eeprom[5], (unsigned int)buf_eeprom[6], (unsigned int)buf_eeprom[7]); //���԰�����ֵ
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
			uart_printf("single_key��  ");
			for (i = 0; i < 16; i++)
			{
				
				uart_printf("%02d ", (unsigned int)single_key[i]);
			}
			uart_printf(" \r\n");

			uart_printf("multiple_key��");
			for (i = 0; i < 16; i++)
			{

				uart_printf("%2d ", (unsigned int)multiple_key[i]);
			}
			uart_printf(" \r\n");
#endif	

			break;


		default:break;//Ĭ�ϵ�break
		}
		clear_again_receive_rf_decoder_finished();//�����־λ
	}
	
	LogoutProcess();
	CycleProcess();
}
