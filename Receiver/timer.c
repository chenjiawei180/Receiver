#include "timer.h"
#include "key.h"
#include "menu.h"

unsigned char await_number = 0;      //����ʱ�̼���50MS��������
unsigned char await_number_table = 0;//0.5���־����

void Init_Timer0(void)
{
	TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��	
	TH0 = (65536 - TIMER50MS) >> 8; //���¸�ֵ 5ms
	TL0 = (65536 - TIMER50MS) & 0xff;
	EA = 1;            //���жϴ�
	ET0 = 1;           //��ʱ���жϴ�
	TR0 = 1;           //��ʱ�����ش�
}

void Timer0_isr(void) interrupt 1
{
	unsigned char func_index_temp = 0;
	TF0 = 0;
	TH0 = (65536 - TIMER50MS) >> 8;		  //���¸�ֵ 50ms
	TL0 = (65536 - TIMER50MS) & 0xff;
	func_index_temp = return_func_index();
	if (func_index_temp == MENU_STANDBY)
	{
		await_number++;
		if (await_number == 10)
		{
			await_number_table++;
			await_number = 0;
		}
	}

}

unsigned char return_await_number_table(void)	//����await_number_table������ֵ
{
	unsigned char await_number_table_temp=0;
	await_number_table_temp = await_number_table;
	return await_number_table_temp;
}

void set_await_number_table(unsigned char temp)	//����await_number_table������ֵ
{
	await_number_table = temp;
}