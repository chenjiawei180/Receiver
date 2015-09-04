#include "timer.h"
#include "key.h"
#include "menu.h"

unsigned char await_number = 0;      //����ʱ�̼���50MS��������
unsigned char await_number_table = 0;//0.5���־����

unsigned char main_press_time_table = 0;//main����ʱ���������־λ
unsigned char main_press_time = 0;

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
	if (func_index_temp == MENU_STANDBY || func_index_temp == TWO_MENU_F0_YEAR || func_index_temp == TWO_MENU_F0_MOUTH 
		|| func_index_temp == TWO_MENU_F0_DAY || func_index_temp == TWO_MENU_F0_WEEK || func_index_temp == TWO_MENU_F0_HOUR
		|| func_index_temp == TWO_MENU_F0_MINUTE )
	{
		await_number++;
		if (await_number == 10)
		{
			await_number_table++;
			await_number = 0;
		}
		if (await_number_table >= 4)//ȷ��������ȷ��ʱ
		{
			await_number_table = 0;
		}		
	}

	if (main_press_time_table == 1) //�˵�������ʱ���־
	{
		main_press_time++;			//����˵�������ʱ�䳤�ȱ���
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

void set_main_press_time_table(unsigned char temp) //����main_press_time_table������ֵ
{
	main_press_time_table = temp;
}

unsigned char return_main_press_time(void)
{
	unsigned char temp = 0;
	temp = main_press_time;
	return temp;
}

void clear_main_press_time(void)
{
	main_press_time = 0;
}