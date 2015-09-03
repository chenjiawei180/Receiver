#include "STC15F2K60S2.H"
#include "usart.h"
#include "key.h"
#include "tm1629.h"

void main()
{	
	UsartInit();	//´®¿Ú³õÊ¼»¯
#ifdef DEBUG
	uart_printf("UsartInit Complete! \r\n");
#endif
	tm1629_init();
#ifdef DEBUG
	uart_printf("tm1629_init Complete! \r\n");
#endif

	while (1)
	{
		KeyDecoder();
	}
}

