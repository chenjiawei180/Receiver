#include "at24c256.h"
#include "key.h"
#include "usart.h"

bit ack;	              //Ӧ���־λ
unsigned char xdata at24c64_buff[PAGE_LENGTH] = { 0 }; //24C256ҳ��洢����
unsigned char xdata eeprom_buff[8] = { 0 };			   //ע�����黺��

void DelayUs2x(unsigned char t)
{
	while (--t);
}
/*------------------------------------------------
mS��ʱ����������������� unsigned char t���޷���ֵ
unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
------------------------------------------------*/
void DelayMs(unsigned char t)
{
	while (t--)
	{
		//������ʱ1mS
		DelayUs2x(245);
		DelayUs2x(245);
	}
}


/*------------------------------------------------
��������
------------------------------------------------*/
void Start_I2c()
{
	SDA = 1;   //������ʼ�����������ź�
	_Nop();
	SCL = 1;
	_Nop();    //��ʼ��������ʱ�����4.7us,��ʱ
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SDA = 0;     //������ʼ�ź�
	_Nop();    //��ʼ��������ʱ�����4��
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SCL = 0;    //ǯסI2C���ߣ�׼�����ͻ��������
	_Nop();
	_Nop();
}
/*------------------------------------------------
��������
------------------------------------------------*/
void Stop_I2c()
{
	SDA = 0;    //���ͽ��������������ź�
	_Nop();   //���ͽ���������ʱ���ź�
	SCL = 1;    //������������ʱ�����4��
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SDA = 1;    //����I2C���߽����ź�
	_Nop();
	_Nop();
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
�ֽ����ݴ��ͺ���
����ԭ��: void  SendByte(unsigned char c);
����:  ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0 ��)
��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
------------------------------------------------------------------*/
void  SendByte(unsigned char c)
{
	unsigned char BitCnt;
	for (BitCnt = 0; BitCnt<8; BitCnt++)  //Ҫ���͵����ݳ���Ϊ8λ
	{
		if ((c << BitCnt) & 0x80)
			SDA = 1;   //�жϷ���λ
		else
			SDA = 0;
		_Nop();
		SCL = 1;               //��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
		_Nop();
		_Nop();             //��֤ʱ�Ӹߵ�ƽ���ڴ���4��
		_Nop();
		_Nop();
		_Nop();
		SCL = 0;
	}
	_Nop();
	_Nop();
	SDA = 1;               //8λ��������ͷ������ߣ�׼������Ӧ��λ
	_Nop();
	_Nop();
	SCL = 1;
	_Nop();
	_Nop();
	_Nop();
	if (SDA == 1)ack = 0;
	else ack = 1;        //�ж��Ƿ���յ�Ӧ���ź�
	SCL = 0;
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
�ֽ����ݴ��ͺ���
����ԭ��: unsigned char  RcvByte();
����:  �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
���������Ӧ������
------------------------------------------------------------------*/
unsigned char  RcvByte()
{
	unsigned char retc;
	unsigned char BitCnt;
	retc = 0;
	SDA = 1;             //��������Ϊ���뷽ʽ
	for (BitCnt = 0; BitCnt<8; BitCnt++)
	{
		_Nop();
		SCL = 0;       //��ʱ����Ϊ�ͣ�׼����������λ
		_Nop();
		_Nop();      //ʱ�ӵ͵�ƽ���ڴ���4.7us
		_Nop();
		_Nop();
		_Nop();
		SCL = 1;       //��ʱ����Ϊ��ʹ��������������Ч
		_Nop();
		_Nop();
		retc = retc << 1;
		if (SDA == 1)retc = retc + 1; //������λ,���յ�����λ����retc��
		_Nop();
		_Nop();
	}
	SCL = 0;
	_Nop();
	_Nop();
	return(retc);
}


/*----------------------------------------------------------------
Ӧ���Ӻ���
ԭ��:  void Ack_I2c(void);

----------------------------------------------------------------*/
void Ack_I2c(void)
{
	SDA = 0;
	_Nop();
	_Nop();
	_Nop();
	SCL = 1;
	_Nop();
	_Nop();              //ʱ�ӵ͵�ƽ���ڴ���4��
	_Nop();
	_Nop();
	_Nop();
	SCL = 0;               //��ʱ���ߣ�ǯסI2C�����Ա��������
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
��Ӧ���Ӻ���
ԭ��:  void NoAck_I2c(void);

----------------------------------------------------------------*/
void NoAck_I2c(void)
{
	SDA = 1;
	_Nop();
	_Nop();
	_Nop();
	SCL = 1;
	_Nop();
	_Nop();              //ʱ�ӵ͵�ƽ���ڴ���4��
	_Nop();
	_Nop();
	_Nop();
	SCL = 0;                //��ʱ���ߣ�ǯסI2C�����Ա��������
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
�����ӵ�ַ�������Ͷ��ֽ����ݺ���
����ԭ��: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
��ַsla���ӵ�ַsuba������������sָ������ݣ�����no���ֽڡ�
�������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
----------------------------------------------------------------*/
bit ISendStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no)
{
	unsigned char i;
	Start_I2c();               //��������
	SendByte(sla);             //����������ַ
	if (ack == 0)return(0);
	SendByte(suba >> 8);            //���������ӵ�ַ
	if (ack == 0)return(0);
	SendByte(suba);
	if (ack == 0)return(0);
	for (i = 0; i<no; i++)
	{
		SendByte(*s);            //��������
		if (ack == 0)return(0);
		s++;
	}
	Stop_I2c();                  //��������
	return(1);
}


/*----------------------------------------------------------------
�����ӵ�ַ������ȡ���ֽ����ݺ���
����ԭ��: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);
����:     ���������ߵ����͵�ַ���ӵ�ַ,�����ݣ��������ߵ�ȫ����,������
��ַsla���ӵ�ַsuba�����������ݷ���sָ��Ĵ洢������no���ֽڡ�
�������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
----------------------------------------------------------------*/
bit IRcvStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no)
{
	unsigned char i;
	Start_I2c();               //��������
	SendByte(sla);             //����������ַ
	if (ack == 0)return(0);
	SendByte(suba >> 8);            //���������ӵ�ַ
	if (ack == 0)return(0);
	SendByte(suba);            //���������ӵ�ַ
	if (ack == 0)return(0);
	Start_I2c();
	SendByte(sla + 1);
	if (ack == 0)return(0);
	for (i = 0; i<no - 1; i++)
	{
		*s = RcvByte();              //��������
		Ack_I2c();                //���;ʹ�λ 
		s++;
	}
	*s = RcvByte();
	NoAck_I2c();                 //���ͷ�Ӧλ
	Stop_I2c();                    //��������
	return(1);
}

bit register_call_function(unsigned char *buf)
{
		unsigned char base_address = 0;//��׼��ַ   
		unsigned char offset_address = 0;//ƫ�Ƶ�ַ
		//��ַ=  ��׼��ַ*32 +ƫ�Ƶ�ַ
		/*��Ѱ�����ID���Ƿ����*/
		for (base_address = CALL_TABLE_START; base_address<ALL_TABLE_NUMBER; base_address++)	//32*32 =1024����־λ
		{
			IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//��AT24C256�������32���ֽڵı�־λ
			delay10ms();
			for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽڱ�־λ����û��0�ľ�OK
			{
				if (at24c64_buff[offset_address] == 0) //�����־λ����0  �����ҿ� ���ID���Ƿ�ע���
				{
					IRcvStr(I2C_ADDRESS, (CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);
					delay10ms();

					if ((*(eeprom_buff + 5) == *(buf + 5)) && (*(eeprom_buff + 6) == *(buf + 6)) && (*(eeprom_buff + 7) == *(buf + 7)))
					{
						//IF�������� �����ID��ע���
						Two_Menu_F1_E1[0] = *(eeprom_buff + 1);
						Two_Menu_F1_E1[1] = *(eeprom_buff + 2);
						Two_Menu_F1_E1[2] = *(eeprom_buff + 3);
						Two_Menu_F1_E1[3] = *(eeprom_buff + 4);
#ifdef DEBUG
						uart_printf("caller register is old! \n\r"); 
#endif

//						if (sound_table == 1)
//						{
//#if SOUND
//							GD5800_select_chapter(SETERROR_POSITION);
//#endif
//							sound_table = 0;
//						}
						return 0;
					}
				}
			}
		}
		//�������ִ�е���������ID��û��ע���
		for (base_address = CALL_TABLE_START; base_address<CALL_TABLE_NUMBER; base_address++)
		{
			IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//��AT24C64�������32���ֽڵı�־λ
			delay10ms();
			for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽڱ�־λ����û��0�ľ�OK
			{
				if (at24c64_buff[offset_address] != 0)
				{
					ISendStr(I2C_ADDRESS, (CALL_DATA_START + ((base_address*PAGE_LENGTH) + offset_address) * 8), buf, 8);
					delay10ms();
					at24c64_buff[offset_address] = 0;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//����־λ�Լ����ݷֱ�д����־����������
#ifdef DEBUG
					uart_printf("caller register is new! \n\r");
#endif
//					if (sound_table == 1)
//					{
//#if SOUND
//						GD5800_select_chapter(SETSUCCESS_POSITION);
//#endif
//						sound_table = 0;
//					}
					return 1;
					//��ʾ�ɹ�				
				}
			}
		}
		return 0;
}

bit register_host_function(unsigned char *buf)
{
	unsigned char base_address = 0;//��׼��ַ   
	unsigned char offset_address = 0;//ƫ�Ƶ�ַ
	//��ַ=  ��׼��ַ*32 +ƫ�Ƶ�ַ
	/*��Ѱ�����ID���Ƿ����*/
	for (base_address = CALL_TABLE_START; base_address<ALL_TABLE_NUMBER; base_address++)	//32*32 =1024����־λ
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//��AT24C256�������32���ֽڵı�־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽڱ�־λ����û��0�ľ�OK
		{
			if (at24c64_buff[offset_address] == 0) //�����־λ����0  �����ҿ� ���ID���Ƿ�ע���
			{
				IRcvStr(I2C_ADDRESS, (CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);
				delay10ms();

				if ((*(eeprom_buff + 5) == *(buf + 5)) && (*(eeprom_buff + 6) == *(buf + 6)) && (*(eeprom_buff + 7) == *(buf + 7)))
				{
					//IF�������� �����ID��ע���
					Two_Menu_F1_E2[0] = *(eeprom_buff + 1);
					Two_Menu_F1_E2[1] = *(eeprom_buff + 2);
					Two_Menu_F1_E2[2] = *(eeprom_buff + 3);
					Two_Menu_F1_E2[3] = *(eeprom_buff + 4);
#ifdef DEBUG
					uart_printf("host register is old! \n\r");
#endif

					//						if (sound_table == 1)
					//						{
					//#if SOUND
					//							GD5800_select_chapter(SETERROR_POSITION);
					//#endif
					//							sound_table = 0;
					//						}
					return 0;
				}
			}
		}
	}
	//�������ִ�е���������ID��û��ע���
	for (base_address = HOST_TABLE_START; base_address<HOST_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//��AT24C64�������32���ֽڵı�־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽڱ�־λ����û��0�ľ�OK
		{
			if (at24c64_buff[offset_address] != 0)
			{
				ISendStr(I2C_ADDRESS, (HOST_DATA_START + ((base_address*PAGE_LENGTH) + offset_address) * 8), buf, 8);
				delay10ms();
				at24c64_buff[offset_address] = 0;
				ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
				delay10ms();
				//����־λ�Լ����ݷֱ�д����־����������
#ifdef DEBUG
				uart_printf("host register is new! \n\r");
#endif
				//					if (sound_table == 1)
				//					{
				//#if SOUND
				//						GD5800_select_chapter(SETSUCCESS_POSITION);
				//#endif
				//						sound_table = 0;
				//					}
				return 1;
				//��ʾ�ɹ�				
			}
		}
	}
	return 0;
}

bit register_alarm_function(unsigned char *buf)
{
	unsigned char base_address = 0;//��׼��ַ   
	unsigned char offset_address = 0;//ƫ�Ƶ�ַ
	//��ַ=  ��׼��ַ*32 +ƫ�Ƶ�ַ
	/*��Ѱ�����ID���Ƿ����*/
	for (base_address = CALL_TABLE_START; base_address<ALL_TABLE_NUMBER; base_address++)	//32*32 =1024����־λ
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//��AT24C256�������32���ֽڵı�־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽڱ�־λ����û��0�ľ�OK
		{
			if (at24c64_buff[offset_address] == 0) //�����־λ����0  �����ҿ� ���ID���Ƿ�ע���
			{
				IRcvStr(I2C_ADDRESS, (CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);
				delay10ms();

				if ((*(eeprom_buff + 5) == *(buf + 5)) && (*(eeprom_buff + 6) == *(buf + 6)) && (*(eeprom_buff + 7) == *(buf + 7)))
				{
					//IF�������� �����ID��ע���
					Two_Menu_F1_E3[0] = *(eeprom_buff + 1);
					Two_Menu_F1_E3[1] = *(eeprom_buff + 2);
					Two_Menu_F1_E3[2] = *(eeprom_buff + 3);
					Two_Menu_F1_E3[3] = *(eeprom_buff + 4);
#ifdef DEBUG
					uart_printf("alarmer register is old! \n\r");
#endif

					//						if (sound_table == 1)
					//						{
					//#if SOUND
					//							GD5800_select_chapter(SETERROR_POSITION);
					//#endif
					//							sound_table = 0;
					//						}
					return 0;
				}
			}
		}
	}
	//�������ִ�е���������ID��û��ע���
	for (base_address = ALARM_TABLE_START; base_address<ALARM_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//��AT24C64�������32���ֽڵı�־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽڱ�־λ����û��0�ľ�OK
		{
			if (at24c64_buff[offset_address] != 0)
			{
				ISendStr(I2C_ADDRESS, (ALARM_DATA_START + ((base_address*PAGE_LENGTH) + offset_address) * 8), buf, 8);
				delay10ms();
				at24c64_buff[offset_address] = 0;
				ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
				delay10ms();
				//����־λ�Լ����ݷֱ�д����־����������
#ifdef DEBUG
				uart_printf("alarmer register is new! \n\r");
#endif
				//					if (sound_table == 1)
				//					{
				//#if SOUND
				//						GD5800_select_chapter(SETSUCCESS_POSITION);
				//#endif
				//						sound_table = 0;
				//					}
				return 1;
				//��ʾ�ɹ�				
			}
		}
	}
	return 0;
}

bit register_cancel_function(unsigned char *buf)
{
	unsigned char base_address = 0;//��׼��ַ   
	unsigned char offset_address = 0;//ƫ�Ƶ�ַ
	//��ַ=  ��׼��ַ*32 +ƫ�Ƶ�ַ
	/*��Ѱ�����ID���Ƿ����*/
	for (base_address = CALL_TABLE_START; base_address<ALL_TABLE_NUMBER; base_address++)	//32*32 =1024����־λ
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//��AT24C256�������32���ֽڵı�־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽڱ�־λ����û��0�ľ�OK
		{
			if (at24c64_buff[offset_address] == 0) //�����־λ����0  �����ҿ� ���ID���Ƿ�ע���
			{
				IRcvStr(I2C_ADDRESS, (CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);
				delay10ms();

				if ((*(eeprom_buff + 5) == *(buf + 5)) && (*(eeprom_buff + 6) == *(buf + 6)) && (*(eeprom_buff + 7) == *(buf + 7)))
				{
					//IF�������� �����ID��ע���
					Two_Menu_F1_E4[0] = *(eeprom_buff + 1);
					Two_Menu_F1_E4[1] = *(eeprom_buff + 2);
					Two_Menu_F1_E4[2] = *(eeprom_buff + 3);
					Two_Menu_F1_E4[3] = *(eeprom_buff + 4);
#ifdef DEBUG
					uart_printf("canceler register is old! \n\r");
#endif

					//						if (sound_table == 1)
					//						{
					//#if SOUND
					//							GD5800_select_chapter(SETERROR_POSITION);
					//#endif
					//							sound_table = 0;
					//						}
					return 0;
				}
			}
		}
	}
	//�������ִ�е���������ID��û��ע���
	for (base_address = CANCEL_TABLE_START; base_address<CANCEL_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//��AT24C64�������32���ֽڵı�־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽڱ�־λ����û��0�ľ�OK
		{
			if (at24c64_buff[offset_address] != 0)
			{
				ISendStr(I2C_ADDRESS, (CANCEL_DATA_START + ((base_address*PAGE_LENGTH) + offset_address) * 8), buf, 8);
				delay10ms();
				at24c64_buff[offset_address] = 0;
				ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
				delay10ms();
				//����־λ�Լ����ݷֱ�д����־����������
#ifdef DEBUG
				uart_printf("canceler register is new! \n\r");
#endif
				//					if (sound_table == 1)
				//					{
				//#if SOUND
				//						GD5800_select_chapter(SETSUCCESS_POSITION);
				//#endif
				//						sound_table = 0;
				//					}
				return 1;
				//��ʾ�ɹ�				
			}
		}
	}
	return 0;
}

bit delete_call_function(unsigned char *buf)//bufΪ���������ָ��
{
	unsigned char base_address = 0;
	unsigned char offset_address = 0;

	for (base_address = CALL_TABLE_START; base_address<CALL_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH); //ȡ��32����־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽ�����û��0�ľ�OK
		{
			if (*(buf) == 0 && *(buf + 1) == 0 && *(buf + 2) == 0 && *(buf + 3) == 0) //ȫ��ɾ��
			{
				at24c64_buff[offset_address] = 0xff;//��־λ����Ϊ��0
#ifdef DEBUG
				uart_printf("caller is delete success \r\n");
#endif		
			}
			else if (at24c64_buff[offset_address] == 0)//�������0000 ������ȫ��ɾ��
			{
				IRcvStr(I2C_ADDRESS, ( CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);//ȡ����Ӧ��־λ����Ӧ��������������
				delay10ms();
				if ((*(eeprom_buff + 1) == *(buf + 0)) && (*(eeprom_buff + 2) == *(buf + 1)) && (*(eeprom_buff + 3) == *(buf + 2)) && (*(eeprom_buff + 4) == *(buf + 3)))
				{
					at24c64_buff[offset_address] = 0xff;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//�����Ӧ�ı�־λ Ȼ���ٽ���־λд���־��
#ifdef DEBUG
					uart_printf("call is %02x %02x %02x .\r\n", (unsigned int)(*(eeprom_buff + 5)), (unsigned int)(*(eeprom_buff + 6)), (unsigned int)(*(eeprom_buff + 7)) ); //
					uart_printf("call is %02x %02x %02x %02x.\r\n", (unsigned int)(*(eeprom_buff + 1)), (unsigned int)(*(eeprom_buff + 2)), (unsigned int)(*(eeprom_buff + 3)), (unsigned int)(*(eeprom_buff + 4)) );
					uart_printf("caller is delete success \r\n");
#endif		
				}

			}
		}
		ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
		//���32����־λд���־��
		delay10ms();
	}
	return 1;
}

bit delete_host_function(unsigned char *buf)//bufΪ���������ָ��
{
	unsigned char base_address = 0;
	unsigned char offset_address = 0;

	for (base_address = HOST_TABLE_START; base_address<HOST_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH); //ȡ��32����־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽ�����û��0�ľ�OK
		{
			if (*(buf) == 0 && *(buf + 1) == 0 && *(buf + 2) == 0 && *(buf + 3) == 0) //ȫ��ɾ��
			{
				at24c64_buff[offset_address] = 0xff;//��־λ����Ϊ��0
#ifdef DEBUG
				uart_printf("hoster is delete success \r\n");
#endif		
			}
			else if (at24c64_buff[offset_address] == 0)//�������0000 ������ȫ��ɾ��
			{
				IRcvStr(I2C_ADDRESS, (HOST_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);//ȡ����Ӧ��־λ����Ӧ��������������
				delay10ms();
				if ((*(eeprom_buff + 1) == *(buf + 0)) && (*(eeprom_buff + 2) == *(buf + 1)) && (*(eeprom_buff + 3) == *(buf + 2)) && (*(eeprom_buff + 4) == *(buf + 3)))
				{
					at24c64_buff[offset_address] = 0xff;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//�����Ӧ�ı�־λ Ȼ���ٽ���־λд���־��
#ifdef DEBUG
					uart_printf("host is %02x %02x %02x .\r\n", (unsigned int)(*(eeprom_buff + 5)), (unsigned int)(*(eeprom_buff + 6)), (unsigned int)(*(eeprom_buff + 7))); //
					uart_printf("host is %02x %02x %02x %02x.\r\n", (unsigned int)(*(eeprom_buff + 1)), (unsigned int)(*(eeprom_buff + 2)), (unsigned int)(*(eeprom_buff + 3)), (unsigned int)(*(eeprom_buff + 4)));
					uart_printf("host is delete success \r\n");
#endif		
				}

			}
		}
		ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
		//���32����־λд���־��
		delay10ms();
	}
	return 1;
}

bit delete_alarm_function(unsigned char *buf)//bufΪ���������ָ��
{
	unsigned char base_address = 0;
	unsigned char offset_address = 0;

	for (base_address = ALARM_TABLE_START; base_address<ALARM_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH); //ȡ��32����־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽ�����û��0�ľ�OK
		{
			if (*(buf) == 0 && *(buf + 1) == 0 && *(buf + 2) == 0 && *(buf + 3) == 0) //ȫ��ɾ��
			{
				at24c64_buff[offset_address] = 0xff;//��־λ����Ϊ��0
#ifdef DEBUG
				uart_printf("alarmer is delete success \r\n");
#endif		
			}
			else if (at24c64_buff[offset_address] == 0)//�������0000 ������ȫ��ɾ��
			{
				IRcvStr(I2C_ADDRESS, (ALARM_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);//ȡ����Ӧ��־λ����Ӧ��������������
				delay10ms();
				if ((*(eeprom_buff + 1) == *(buf + 0)) && (*(eeprom_buff + 2) == *(buf + 1)) && (*(eeprom_buff + 3) == *(buf + 2)) && (*(eeprom_buff + 4) == *(buf + 3)))
				{
					at24c64_buff[offset_address] = 0xff;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//�����Ӧ�ı�־λ Ȼ���ٽ���־λд���־��
#ifdef DEBUG
					uart_printf("alarm is %02x %02x %02x .\r\n", (unsigned int)(*(eeprom_buff + 5)), (unsigned int)(*(eeprom_buff + 6)), (unsigned int)(*(eeprom_buff + 7))); //
					uart_printf("alarm is %02x %02x %02x %02x.\r\n", (unsigned int)(*(eeprom_buff + 1)), (unsigned int)(*(eeprom_buff + 2)), (unsigned int)(*(eeprom_buff + 3)), (unsigned int)(*(eeprom_buff + 4)));
					uart_printf("alarm is delete success \r\n");
#endif		
				}

			}
		}
		ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
		//���32����־λд���־��
		delay10ms();
	}
	return 1;
}

bit delete_cancel_function(unsigned char *buf)//bufΪ���������ָ��
{
	unsigned char base_address = 0;
	unsigned char offset_address = 0;

	for (base_address = CANCEL_TABLE_START; base_address<CANCEL_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH); //ȡ��32����־λ
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//��Ҫ32���ֽ�����û��0�ľ�OK
		{
			if (*(buf) == 0 && *(buf + 1) == 0 && *(buf + 2) == 0 && *(buf + 3) == 0) //ȫ��ɾ��
			{
				at24c64_buff[offset_address] = 0xff;//��־λ����Ϊ��0
#ifdef DEBUG
				uart_printf("canceler is delete success \r\n");
#endif		
			}
			else if (at24c64_buff[offset_address] == 0)//�������0000 ������ȫ��ɾ��
			{
				IRcvStr(I2C_ADDRESS, (CANCEL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);//ȡ����Ӧ��־λ����Ӧ��������������
				delay10ms();
				if ((*(eeprom_buff + 1) == *(buf + 0)) && (*(eeprom_buff + 2) == *(buf + 1)) && (*(eeprom_buff + 3) == *(buf + 2)) && (*(eeprom_buff + 4) == *(buf + 3)))
				{
					at24c64_buff[offset_address] = 0xff;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//�����Ӧ�ı�־λ Ȼ���ٽ���־λд���־��
#ifdef DEBUG
					uart_printf("cancel is %02x %02x %02x .\r\n", (unsigned int)(*(eeprom_buff + 5)), (unsigned int)(*(eeprom_buff + 6)), (unsigned int)(*(eeprom_buff + 7))); //
					uart_printf("cancel is %02x %02x %02x %02x.\r\n", (unsigned int)(*(eeprom_buff + 1)), (unsigned int)(*(eeprom_buff + 2)), (unsigned int)(*(eeprom_buff + 3)), (unsigned int)(*(eeprom_buff + 4)));
					uart_printf("cancel is delete success \r\n");
#endif		
				}

			}
		}
		ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
		//���32����־λд���־��
		delay10ms();
	}
	return 1;
}