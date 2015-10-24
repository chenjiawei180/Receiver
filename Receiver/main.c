#include "STC15F2K60S2.H"
#include "usart.h"
#include "key.h"
#include "tm1629.h"
#include "timer.h"
#include "ds1302.h"
#include "ev1527.h"
#include "decoder.h"
#include "gd5800.h"
#include "menu.h"


void main()
{	

	UsartInit();	//���ڳ�ʼ��
#ifdef DEBUG
	uart_printf("UsartInit Complete! \r\n");
#endif
	tm1629_init();
#ifdef DEBUG
	uart_printf("tm1629_init Complete! \r\n");
#endif
	Init_Timer0();
#ifdef DEBUG
	uart_printf("Init_Timer0 Complete! \r\n");
#endif
	Init_Timer1();
#ifdef DEBUG
	uart_printf("Init_Timer1 Complete! \r\n");
#endif
	exint0_init();
#ifdef DEBUG
	uart_printf("exint0_init Complete! \r\n");
#endif
	Ds1302_Init();
#ifdef DEBUG
	uart_printf("Ds1302_Init Complete! \r\n");
#endif
	GD5800_initialization();
	delay10ms();
#ifdef DEBUG
	uart_printf("GD5800_initialization Complete! \r\n");
#endif
	//Ds1302_Write_Time();

	P3M1 = 0X0C; //IO������Ϊ���� ������� �����޷�����
	P3M0 = 0X00;



	GD5800_select_chapter(CHUSHIHUA);
	env_init();

	while (1)
	{
		KeyProcess();
		DecoderProcess();
	}
}



