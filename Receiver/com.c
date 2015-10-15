#include "com.h"
#include "ds1302.h"
#include "usart.h"

void mcu_to_computer(unsigned char call_type, unsigned char* number,unsigned char key_type)
{
	unsigned char temp1=0,temp2=0;

	unsigned char sec = 0, hour = 0, min = 0;
	Ds1302_Read_Time();

	Usart1_SendData(0xaa);
	Usart1_SendData(0x42);
	Usart1_SendData(0x01);
	Usart1_SendData(0x01);
	Usart1_SendData(0x01);
	Usart1_SendData(call_type);//呼叫或者取消  91 92
	Usart1_SendData(0xff);

	temp1 = *(number+1) << 4 | *(number + 2);
	temp2 = *(number+3) << 4 | *(number + 4);
	Usart1_SendData(temp1);//号码
	Usart1_SendData(temp2);//号码

	Usart1_SendData(key_type);//键值

	Usart1_SendData(0);
	Usart1_SendData(0);

	hour  =  (time_buf1[4] / 10) * 6 + time_buf1[4];
	min =  (time_buf1[5] / 10) * 6 + time_buf1[5];
	sec  =  (time_buf1[6] / 10) * 6 + time_buf1[6];

	Usart1_SendData(hour);
	Usart1_SendData(min);
	Usart1_SendData(sec);

	Usart1_SendData(0x1EE + call_type + temp1 + temp2 + key_type + hour + min + sec);//校验


	Usart1_SendData(0x55);
}




