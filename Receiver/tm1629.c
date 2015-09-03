#include "tm1629.h"

unsigned char const CODE[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x76, 0x38, 0x5c, 0x73, 0x3e };//0-9 abcdef ��ʾ��������
unsigned char const INIT_CODE[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };//��ε������������
unsigned char const SHANGSHUO[] = { 0x40, 0x00 }; // ����������м���Լ���
unsigned char buf_display[6][8] = { 0 }; //3��TM1629�Դ�����
unsigned char display_ram[240] = { 0 }; //��������ʱ��¼��ʾ���ݵ��ڴ� 

void writeDataTo1629(unsigned char p) //д���ݸ���һ��TM1629
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

void writeDataTo1629_2(unsigned char p) //д���ݸ��ڶ���TM1629
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

void writeDataTo1629_3(unsigned char p) //д���ݸ�������TM1629
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

void send_command(unsigned char word) //д�������һ��TM1629
{
	TM1629_STB = 1;
	nop;
	TM1629_STB = 0;
	writeDataTo1629(word);
}

void send_command_2(unsigned char word) //д������ڶ���TM1629
{
	TM1629_STB2 = 1;
	nop;
	TM1629_STB2 = 0;
	writeDataTo1629_2(word);
}

void send_command_3(unsigned char word) //д�����������TM1629
{
	TM1629_STB3 = 1;
	nop;
	TM1629_STB3 = 0;
	writeDataTo1629_3(word);
}

void display(void) //3��TM1629��ʾ����
{
	unsigned char i;
	send_command(0x40);	//������������:��ͨģʽ����ַ����1��д���ݵ��Դ�
	send_command(0xc0);	//������ʾ��ַ�����00H��ʼ
	for (i = 0; i<8; i++)	//����16�ֽڵ��Դ�����
	{
		writeDataTo1629(buf_display[0][i]);
		writeDataTo1629(buf_display[1][i]);
	}
	send_command(0x8C);	//������ʾ�����������ʾ��������Ϊ11/16.
	TM1629_STB = 1;

	send_command_2(0x40);	//������������:��ͨģʽ����ַ����1��д���ݵ��Դ�
	send_command_2(0xc0);	//������ʾ��ַ�����00H��ʼ
	for (i = 0; i<8; i++)	//����16�ֽڵ��Դ�����
	{
		writeDataTo1629_2(buf_display[2][i]);
		writeDataTo1629_2(buf_display[3][i]);
	}
	send_command_2(0x8C);	//������ʾ�����������ʾ��������Ϊ11/16.
	TM1629_STB2 = 1;

	send_command_3(0x40);	//������������:��ͨģʽ����ַ����1��д���ݵ��Դ�
	send_command_3(0xc0);	//������ʾ��ַ�����00H��ʼ
	for (i = 0; i<8; i++)	//����16�ֽڵ��Դ�����
	{
		writeDataTo1629_3(buf_display[4][i]);
		writeDataTo1629_3(buf_display[5][i]);
	}
	send_command_3(0x8C);	//������ʾ�����������ʾ��������Ϊ11/16.
	TM1629_STB3 = 1;
}

void tm1629_init(void) //TM1629������ʼ������
{
	unsigned char i, j, k;		//k������ʾ�ľ������֣�i��j����buf_display��ˢ��
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


void Tm1629_delay(unsigned char k) //��ʱ����
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
