#include "at24c256.h"
#include "key.h"
#include "usart.h"

bit ack;	              //应答标志位
unsigned char xdata at24c64_buff[PAGE_LENGTH] = { 0 }; //24C256页面存储缓存
unsigned char xdata eeprom_buff[8] = { 0 };			   //注册数组缓存

void DelayUs2x(unsigned char t)
{
	while (--t);
}
/*------------------------------------------------
mS延时函数，含有输入参数 unsigned char t，无返回值
unsigned char 是定义无符号字符变量，其值的范围是
0~255 这里使用晶振12M，精确延时请使用汇编
------------------------------------------------*/
void DelayMs(unsigned char t)
{
	while (t--)
	{
		//大致延时1mS
		DelayUs2x(245);
		DelayUs2x(245);
	}
}


/*------------------------------------------------
启动总线
------------------------------------------------*/
void Start_I2c()
{
	SDA = 1;   //发送起始条件的数据信号
	_Nop();
	SCL = 1;
	_Nop();    //起始条件建立时间大于4.7us,延时
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SDA = 0;     //发送起始信号
	_Nop();    //起始条件锁定时间大于4μ
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SCL = 0;    //钳住I2C总线，准备发送或接收数据
	_Nop();
	_Nop();
}
/*------------------------------------------------
结束总线
------------------------------------------------*/
void Stop_I2c()
{
	SDA = 0;    //发送结束条件的数据信号
	_Nop();   //发送结束条件的时钟信号
	SCL = 1;    //结束条件建立时间大于4μ
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	_Nop();
	SDA = 1;    //发送I2C总线结束信号
	_Nop();
	_Nop();
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
字节数据传送函数
函数原型: void  SendByte(unsigned char c);
功能:  将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
此状态位进行操作.(不应答或非应答都使ack=0 假)
发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
------------------------------------------------------------------*/
void  SendByte(unsigned char c)
{
	unsigned char BitCnt;
	for (BitCnt = 0; BitCnt<8; BitCnt++)  //要传送的数据长度为8位
	{
		if ((c << BitCnt) & 0x80)
			SDA = 1;   //判断发送位
		else
			SDA = 0;
		_Nop();
		SCL = 1;               //置时钟线为高，通知被控器开始接收数据位
		_Nop();
		_Nop();             //保证时钟高电平周期大于4μ
		_Nop();
		_Nop();
		_Nop();
		SCL = 0;
	}
	_Nop();
	_Nop();
	SDA = 1;               //8位发送完后释放数据线，准备接收应答位
	_Nop();
	_Nop();
	SCL = 1;
	_Nop();
	_Nop();
	_Nop();
	if (SDA == 1)ack = 0;
	else ack = 1;        //判断是否接收到应答信号
	SCL = 0;
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
字节数据传送函数
函数原型: unsigned char  RcvByte();
功能:  用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
发完后请用应答函数。
------------------------------------------------------------------*/
unsigned char  RcvByte()
{
	unsigned char retc;
	unsigned char BitCnt;
	retc = 0;
	SDA = 1;             //置数据线为输入方式
	for (BitCnt = 0; BitCnt<8; BitCnt++)
	{
		_Nop();
		SCL = 0;       //置时钟线为低，准备接收数据位
		_Nop();
		_Nop();      //时钟低电平周期大于4.7us
		_Nop();
		_Nop();
		_Nop();
		SCL = 1;       //置时钟线为高使数据线上数据有效
		_Nop();
		_Nop();
		retc = retc << 1;
		if (SDA == 1)retc = retc + 1; //读数据位,接收的数据位放入retc中
		_Nop();
		_Nop();
	}
	SCL = 0;
	_Nop();
	_Nop();
	return(retc);
}


/*----------------------------------------------------------------
应答子函数
原型:  void Ack_I2c(void);

----------------------------------------------------------------*/
void Ack_I2c(void)
{
	SDA = 0;
	_Nop();
	_Nop();
	_Nop();
	SCL = 1;
	_Nop();
	_Nop();              //时钟低电平周期大于4μ
	_Nop();
	_Nop();
	_Nop();
	SCL = 0;               //清时钟线，钳住I2C总线以便继续接收
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
非应答子函数
原型:  void NoAck_I2c(void);

----------------------------------------------------------------*/
void NoAck_I2c(void)
{
	SDA = 1;
	_Nop();
	_Nop();
	_Nop();
	SCL = 1;
	_Nop();
	_Nop();              //时钟低电平周期大于4μ
	_Nop();
	_Nop();
	_Nop();
	SCL = 0;                //清时钟线，钳住I2C总线以便继续接收
	_Nop();
	_Nop();
}

/*----------------------------------------------------------------
向有子地址器件发送多字节数据函数
函数原型: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
----------------------------------------------------------------*/
bit ISendStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no)
{
	unsigned char i;
	Start_I2c();               //启动总线
	SendByte(sla);             //发送器件地址
	if (ack == 0)return(0);
	SendByte(suba >> 8);            //发送器件子地址
	if (ack == 0)return(0);
	SendByte(suba);
	if (ack == 0)return(0);
	for (i = 0; i<no; i++)
	{
		SendByte(*s);            //发送数据
		if (ack == 0)return(0);
		s++;
	}
	Stop_I2c();                  //结束总线
	return(1);
}


/*----------------------------------------------------------------
向有子地址器件读取多字节数据函数
函数原型: bit  ISendStr(unsigned char sla,unsigned char suba,ucahr *s,unsigned char no);
功能:     从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
地址sla，子地址suba，读出的内容放入s指向的存储区，读no个字节。
如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
----------------------------------------------------------------*/
bit IRcvStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no)
{
	unsigned char i;
	Start_I2c();               //启动总线
	SendByte(sla);             //发送器件地址
	if (ack == 0)return(0);
	SendByte(suba >> 8);            //发送器件子地址
	if (ack == 0)return(0);
	SendByte(suba);            //发送器件子地址
	if (ack == 0)return(0);
	Start_I2c();
	SendByte(sla + 1);
	if (ack == 0)return(0);
	for (i = 0; i<no - 1; i++)
	{
		*s = RcvByte();              //发送数据
		Ack_I2c();                //发送就答位 
		s++;
	}
	*s = RcvByte();
	NoAck_I2c();                 //发送非应位
	Stop_I2c();                    //结束总线
	return(1);
}

bit register_call_function(unsigned char *buf)
{
		unsigned char base_address = 0;//基准地址   
		unsigned char offset_address = 0;//偏移地址
		//地址=  基准地址*32 +偏移地址
		/*先寻找这个ID码是否存在*/
		for (base_address = CALL_TABLE_START; base_address<ALL_TABLE_NUMBER; base_address++)	//32*32 =1024个标志位
		{
			IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//从AT24C256里面读出32个字节的标志位
			delay10ms();
			for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节标志位里面没有0的就OK
			{
				if (at24c64_buff[offset_address] == 0) //如果标志位等于0  则找找看 这个ID码是否注册过
				{
					IRcvStr(I2C_ADDRESS, (CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);
					delay10ms();

					if ((*(eeprom_buff + 5) == *(buf + 5)) && (*(eeprom_buff + 6) == *(buf + 6)) && (*(eeprom_buff + 7) == *(buf + 7)))
					{
						//IF条件成立 则这个ID码注册过
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
		//如果程序执行到这里，则代表ID码没有注册过
		for (base_address = CALL_TABLE_START; base_address<CALL_TABLE_NUMBER; base_address++)
		{
			IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//从AT24C64里面读出32个字节的标志位
			delay10ms();
			for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节标志位里面没有0的就OK
			{
				if (at24c64_buff[offset_address] != 0)
				{
					ISendStr(I2C_ADDRESS, (CALL_DATA_START + ((base_address*PAGE_LENGTH) + offset_address) * 8), buf, 8);
					delay10ms();
					at24c64_buff[offset_address] = 0;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//将标志位以及数据分别写到标志区跟数据区
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
					//提示成功				
				}
			}
		}
		return 0;
}

bit register_host_function(unsigned char *buf)
{
	unsigned char base_address = 0;//基准地址   
	unsigned char offset_address = 0;//偏移地址
	//地址=  基准地址*32 +偏移地址
	/*先寻找这个ID码是否存在*/
	for (base_address = CALL_TABLE_START; base_address<ALL_TABLE_NUMBER; base_address++)	//32*32 =1024个标志位
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//从AT24C256里面读出32个字节的标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节标志位里面没有0的就OK
		{
			if (at24c64_buff[offset_address] == 0) //如果标志位等于0  则找找看 这个ID码是否注册过
			{
				IRcvStr(I2C_ADDRESS, (CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);
				delay10ms();

				if ((*(eeprom_buff + 5) == *(buf + 5)) && (*(eeprom_buff + 6) == *(buf + 6)) && (*(eeprom_buff + 7) == *(buf + 7)))
				{
					//IF条件成立 则这个ID码注册过
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
	//如果程序执行到这里，则代表ID码没有注册过
	for (base_address = HOST_TABLE_START; base_address<HOST_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//从AT24C64里面读出32个字节的标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节标志位里面没有0的就OK
		{
			if (at24c64_buff[offset_address] != 0)
			{
				ISendStr(I2C_ADDRESS, (HOST_DATA_START + ((base_address*PAGE_LENGTH) + offset_address) * 8), buf, 8);
				delay10ms();
				at24c64_buff[offset_address] = 0;
				ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
				delay10ms();
				//将标志位以及数据分别写到标志区跟数据区
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
				//提示成功				
			}
		}
	}
	return 0;
}

bit register_alarm_function(unsigned char *buf)
{
	unsigned char base_address = 0;//基准地址   
	unsigned char offset_address = 0;//偏移地址
	//地址=  基准地址*32 +偏移地址
	/*先寻找这个ID码是否存在*/
	for (base_address = CALL_TABLE_START; base_address<ALL_TABLE_NUMBER; base_address++)	//32*32 =1024个标志位
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//从AT24C256里面读出32个字节的标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节标志位里面没有0的就OK
		{
			if (at24c64_buff[offset_address] == 0) //如果标志位等于0  则找找看 这个ID码是否注册过
			{
				IRcvStr(I2C_ADDRESS, (CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);
				delay10ms();

				if ((*(eeprom_buff + 5) == *(buf + 5)) && (*(eeprom_buff + 6) == *(buf + 6)) && (*(eeprom_buff + 7) == *(buf + 7)))
				{
					//IF条件成立 则这个ID码注册过
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
	//如果程序执行到这里，则代表ID码没有注册过
	for (base_address = ALARM_TABLE_START; base_address<ALARM_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//从AT24C64里面读出32个字节的标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节标志位里面没有0的就OK
		{
			if (at24c64_buff[offset_address] != 0)
			{
				ISendStr(I2C_ADDRESS, (ALARM_DATA_START + ((base_address*PAGE_LENGTH) + offset_address) * 8), buf, 8);
				delay10ms();
				at24c64_buff[offset_address] = 0;
				ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
				delay10ms();
				//将标志位以及数据分别写到标志区跟数据区
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
				//提示成功				
			}
		}
	}
	return 0;
}

bit register_cancel_function(unsigned char *buf)
{
	unsigned char base_address = 0;//基准地址   
	unsigned char offset_address = 0;//偏移地址
	//地址=  基准地址*32 +偏移地址
	/*先寻找这个ID码是否存在*/
	for (base_address = CALL_TABLE_START; base_address<ALL_TABLE_NUMBER; base_address++)	//32*32 =1024个标志位
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//从AT24C256里面读出32个字节的标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节标志位里面没有0的就OK
		{
			if (at24c64_buff[offset_address] == 0) //如果标志位等于0  则找找看 这个ID码是否注册过
			{
				IRcvStr(I2C_ADDRESS, (CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);
				delay10ms();

				if ((*(eeprom_buff + 5) == *(buf + 5)) && (*(eeprom_buff + 6) == *(buf + 6)) && (*(eeprom_buff + 7) == *(buf + 7)))
				{
					//IF条件成立 则这个ID码注册过
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
	//如果程序执行到这里，则代表ID码没有注册过
	for (base_address = CANCEL_TABLE_START; base_address<CANCEL_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);//从AT24C64里面读出32个字节的标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节标志位里面没有0的就OK
		{
			if (at24c64_buff[offset_address] != 0)
			{
				ISendStr(I2C_ADDRESS, (CANCEL_DATA_START + ((base_address*PAGE_LENGTH) + offset_address) * 8), buf, 8);
				delay10ms();
				at24c64_buff[offset_address] = 0;
				ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
				delay10ms();
				//将标志位以及数据分别写到标志区跟数据区
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
				//提示成功				
			}
		}
	}
	return 0;
}

bit delete_call_function(unsigned char *buf)//buf为组码数组的指针
{
	unsigned char base_address = 0;
	unsigned char offset_address = 0;

	for (base_address = CALL_TABLE_START; base_address<CALL_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH); //取出32个标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节里面没有0的就OK
		{
			if (*(buf) == 0 && *(buf + 1) == 0 && *(buf + 2) == 0 && *(buf + 3) == 0) //全部删除
			{
				at24c64_buff[offset_address] = 0xff;//标志位设置为非0
#ifdef DEBUG
				uart_printf("caller is delete success \r\n");
#endif		
			}
			else if (at24c64_buff[offset_address] == 0)//如果不是0000 则无需全部删除
			{
				IRcvStr(I2C_ADDRESS, ( CALL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);//取出对应标志位所对应的数据区的数据
				delay10ms();
				if ((*(eeprom_buff + 1) == *(buf + 0)) && (*(eeprom_buff + 2) == *(buf + 1)) && (*(eeprom_buff + 3) == *(buf + 2)) && (*(eeprom_buff + 4) == *(buf + 3)))
				{
					at24c64_buff[offset_address] = 0xff;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//清除对应的标志位 然后再将标志位写入标志区
#ifdef DEBUG
					uart_printf("call is %02x %02x %02x .\r\n", (unsigned int)(*(eeprom_buff + 5)), (unsigned int)(*(eeprom_buff + 6)), (unsigned int)(*(eeprom_buff + 7)) ); //
					uart_printf("call is %02x %02x %02x %02x.\r\n", (unsigned int)(*(eeprom_buff + 1)), (unsigned int)(*(eeprom_buff + 2)), (unsigned int)(*(eeprom_buff + 3)), (unsigned int)(*(eeprom_buff + 4)) );
					uart_printf("caller is delete success \r\n");
#endif		
				}

			}
		}
		ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
		//最后将32个标志位写入标志区
		delay10ms();
	}
	return 1;
}

bit delete_host_function(unsigned char *buf)//buf为组码数组的指针
{
	unsigned char base_address = 0;
	unsigned char offset_address = 0;

	for (base_address = HOST_TABLE_START; base_address<HOST_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH); //取出32个标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节里面没有0的就OK
		{
			if (*(buf) == 0 && *(buf + 1) == 0 && *(buf + 2) == 0 && *(buf + 3) == 0) //全部删除
			{
				at24c64_buff[offset_address] = 0xff;//标志位设置为非0
#ifdef DEBUG
				uart_printf("hoster is delete success \r\n");
#endif		
			}
			else if (at24c64_buff[offset_address] == 0)//如果不是0000 则无需全部删除
			{
				IRcvStr(I2C_ADDRESS, (HOST_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);//取出对应标志位所对应的数据区的数据
				delay10ms();
				if ((*(eeprom_buff + 1) == *(buf + 0)) && (*(eeprom_buff + 2) == *(buf + 1)) && (*(eeprom_buff + 3) == *(buf + 2)) && (*(eeprom_buff + 4) == *(buf + 3)))
				{
					at24c64_buff[offset_address] = 0xff;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//清除对应的标志位 然后再将标志位写入标志区
#ifdef DEBUG
					uart_printf("host is %02x %02x %02x .\r\n", (unsigned int)(*(eeprom_buff + 5)), (unsigned int)(*(eeprom_buff + 6)), (unsigned int)(*(eeprom_buff + 7))); //
					uart_printf("host is %02x %02x %02x %02x.\r\n", (unsigned int)(*(eeprom_buff + 1)), (unsigned int)(*(eeprom_buff + 2)), (unsigned int)(*(eeprom_buff + 3)), (unsigned int)(*(eeprom_buff + 4)));
					uart_printf("host is delete success \r\n");
#endif		
				}

			}
		}
		ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
		//最后将32个标志位写入标志区
		delay10ms();
	}
	return 1;
}

bit delete_alarm_function(unsigned char *buf)//buf为组码数组的指针
{
	unsigned char base_address = 0;
	unsigned char offset_address = 0;

	for (base_address = ALARM_TABLE_START; base_address<ALARM_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH); //取出32个标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节里面没有0的就OK
		{
			if (*(buf) == 0 && *(buf + 1) == 0 && *(buf + 2) == 0 && *(buf + 3) == 0) //全部删除
			{
				at24c64_buff[offset_address] = 0xff;//标志位设置为非0
#ifdef DEBUG
				uart_printf("alarmer is delete success \r\n");
#endif		
			}
			else if (at24c64_buff[offset_address] == 0)//如果不是0000 则无需全部删除
			{
				IRcvStr(I2C_ADDRESS, (ALARM_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);//取出对应标志位所对应的数据区的数据
				delay10ms();
				if ((*(eeprom_buff + 1) == *(buf + 0)) && (*(eeprom_buff + 2) == *(buf + 1)) && (*(eeprom_buff + 3) == *(buf + 2)) && (*(eeprom_buff + 4) == *(buf + 3)))
				{
					at24c64_buff[offset_address] = 0xff;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//清除对应的标志位 然后再将标志位写入标志区
#ifdef DEBUG
					uart_printf("alarm is %02x %02x %02x .\r\n", (unsigned int)(*(eeprom_buff + 5)), (unsigned int)(*(eeprom_buff + 6)), (unsigned int)(*(eeprom_buff + 7))); //
					uart_printf("alarm is %02x %02x %02x %02x.\r\n", (unsigned int)(*(eeprom_buff + 1)), (unsigned int)(*(eeprom_buff + 2)), (unsigned int)(*(eeprom_buff + 3)), (unsigned int)(*(eeprom_buff + 4)));
					uart_printf("alarm is delete success \r\n");
#endif		
				}

			}
		}
		ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
		//最后将32个标志位写入标志区
		delay10ms();
	}
	return 1;
}

bit delete_cancel_function(unsigned char *buf)//buf为组码数组的指针
{
	unsigned char base_address = 0;
	unsigned char offset_address = 0;

	for (base_address = CANCEL_TABLE_START; base_address<CANCEL_TABLE_NUMBER; base_address++)
	{
		IRcvStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH); //取出32个标志位
		delay10ms();
		for (offset_address = 0; offset_address<PAGE_LENGTH; offset_address++)//主要32个字节里面没有0的就OK
		{
			if (*(buf) == 0 && *(buf + 1) == 0 && *(buf + 2) == 0 && *(buf + 3) == 0) //全部删除
			{
				at24c64_buff[offset_address] = 0xff;//标志位设置为非0
#ifdef DEBUG
				uart_printf("canceler is delete success \r\n");
#endif		
			}
			else if (at24c64_buff[offset_address] == 0)//如果不是0000 则无需全部删除
			{
				IRcvStr(I2C_ADDRESS, (CANCEL_DATA_START + (base_address*PAGE_LENGTH + offset_address) * 8), eeprom_buff, 8);//取出对应标志位所对应的数据区的数据
				delay10ms();
				if ((*(eeprom_buff + 1) == *(buf + 0)) && (*(eeprom_buff + 2) == *(buf + 1)) && (*(eeprom_buff + 3) == *(buf + 2)) && (*(eeprom_buff + 4) == *(buf + 3)))
				{
					at24c64_buff[offset_address] = 0xff;
					ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
					delay10ms();
					//清除对应的标志位 然后再将标志位写入标志区
#ifdef DEBUG
					uart_printf("cancel is %02x %02x %02x .\r\n", (unsigned int)(*(eeprom_buff + 5)), (unsigned int)(*(eeprom_buff + 6)), (unsigned int)(*(eeprom_buff + 7))); //
					uart_printf("cancel is %02x %02x %02x %02x.\r\n", (unsigned int)(*(eeprom_buff + 1)), (unsigned int)(*(eeprom_buff + 2)), (unsigned int)(*(eeprom_buff + 3)), (unsigned int)(*(eeprom_buff + 4)));
					uart_printf("cancel is delete success \r\n");
#endif		
				}

			}
		}
		ISendStr(I2C_ADDRESS, base_address*PAGE_LENGTH, at24c64_buff, PAGE_LENGTH);
		//最后将32个标志位写入标志区
		delay10ms();
	}
	return 1;
}