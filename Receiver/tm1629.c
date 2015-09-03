#include "tm1629.h"

unsigned char const CODE[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x76, 0x38, 0x5c, 0x73, 0x3e };//0-9 abcdef 显示器码数组
unsigned char const INIT_CODE[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };//逐段点亮数码管数组
unsigned char const SHANGSHUO[] = { 0x40, 0x00 }; // 点亮数码管中间段以及灭
unsigned char buf_display[6][8] = { 0 }; //3个TM1629显存数组
unsigned char display_ram[240] = { 0 }; //程序运行时记录显示数据的内存 

void writeDataTo1629(unsigned char p) //写数据给第一个TM1629
{
	unsigned int i;
	TM1629_STB = 0;
	for (i = 0; i<8; i++)
	{
		TM1629_CLK = 0;
		if (p & 0x01)
			TM1629_DIO = 1;
		else
			TM1629_DIO = 0;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		TM1629_CLK = 1;
		p = p >> 1;
	}
	TM1629_CLK = 0;
	TM1629_DIO = 0;
}

void writeDataTo1629_2(unsigned char p) //写数据给第二个TM1629
{
	unsigned int i;
	TM1629_STB2 = 0;
	for (i = 0; i<8; i++)
	{
		TM1629_CLK = 0;
		if (p & 0x01)
			TM1629_DIO = 1;
		else
			TM1629_DIO = 0;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		TM1629_CLK = 1;
		p = p >> 1;
	}
	TM1629_CLK = 0;
	TM1629_DIO = 0;
}

void writeDataTo1629_3(unsigned char p) //写数据给第三个TM1629
{
	unsigned int i;
	TM1629_STB3 = 0;
	for (i = 0; i<8; i++)
	{
		TM1629_CLK = 0;
		if (p & 0x01)
			TM1629_DIO = 1;
		else
			TM1629_DIO = 0;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		TM1629_CLK = 1;
		p = p >> 1;
	}
	TM1629_CLK = 0;
	TM1629_DIO = 0;
}

void send_command(unsigned char word) //写命令给第一个TM1629
{
	TM1629_STB = 1;
	nop;
	TM1629_STB = 0;
	writeDataTo1629(word);
}

void send_command_2(unsigned char word) //写命令给第二个TM1629
{
	TM1629_STB2 = 1;
	nop;
	TM1629_STB2 = 0;
	writeDataTo1629_2(word);
}

void send_command_3(unsigned char word) //写命令给第三个TM1629
{
	TM1629_STB3 = 1;
	nop;
	TM1629_STB3 = 0;
	writeDataTo1629_3(word);
}

void display(void) //3个TM1629显示函数
{
	unsigned char i;
	send_command(0x40);	//设置数据命令:普通模式、地址自增1，写数据到显存
	send_command(0xc0);	//设置显示地址命令：从00H开始
	for (i = 0; i<8; i++)	//发送16字节的显存数据
	{
		writeDataTo1629(buf_display[0][i]);
		writeDataTo1629(buf_display[1][i]);
	}
	send_command(0x8C);	//设置显示控制命令：打开显示，并设置为11/16.
	TM1629_STB = 1;

	send_command_2(0x40);	//设置数据命令:普通模式、地址自增1，写数据到显存
	send_command_2(0xc0);	//设置显示地址命令：从00H开始
	for (i = 0; i<8; i++)	//发送16字节的显存数据
	{
		writeDataTo1629_2(buf_display[2][i]);
		writeDataTo1629_2(buf_display[3][i]);
	}
	send_command_2(0x8C);	//设置显示控制命令：打开显示，并设置为11/16.
	TM1629_STB2 = 1;

	send_command_3(0x40);	//设置数据命令:普通模式、地址自增1，写数据到显存
	send_command_3(0xc0);	//设置显示地址命令：从00H开始
	for (i = 0; i<8; i++)	//发送16字节的显存数据
	{
		writeDataTo1629_3(buf_display[4][i]);
		writeDataTo1629_3(buf_display[5][i]);
	}
	send_command_3(0x8C);	//设置显示控制命令：打开显示，并设置为11/16.
	TM1629_STB3 = 1;
}

void tm1629_init(void) //TM1629开机初始化函数
{
	unsigned char i, j, k;		//k控制显示的具体数字，i和j控制buf_display的刷新
	for (k = 0; k<8; k++)
	{
		for (i = 0; i<6; i++)
		{
			for (j = 0; j<8; j++)
			{
				buf_display[i][j] = INIT_CODE[k];
			}
		}
		display();
		Tm1629_delay(30);
	}
}


void Tm1629_delay(unsigned char k) //延时函数
{
	unsigned char i, j;
	for (; k>0; k--)
	{
		for (i = 255; i>0; i--)
		{
			for (j = 255; j>0; j--)
			{
				;

			}
		}
	}
}
