#include "STC15F2K60S2.H"
#include "usart.h"
#include "key.h"

void main()
{	
	UsartInit();	//���ڳ�ʼ��
#ifdef DEBUG
	uart_printf("UsartInit Complete! \r\n");
#endif

	while (1)
	{
		KeyDecoder();
	}
}