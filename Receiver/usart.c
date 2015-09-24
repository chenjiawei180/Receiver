/***********************************************************
**************************************************************/
#include "usart.h"
bit busy;

/*----------------------------
UART �жϷ������
-----------------------------*/
void Uart() interrupt 4 using 1
{
	unsigned char temp=0;
    if (RI)
    {
        RI = 0;                 //���RIλ
	  //  computer_send_data_to_mcu();
      //  P0 = SBUF;              //P0��ʾ��������
      //  P22 = RB8;              //P2.2��ʾУ��λ
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy = 0;               //��æ��־
    }
}

/*----------------------------
UART2 �жϷ������
-----------------------------*/
void Uart2() interrupt 8 using 1
{
	if (S2CON & S2RI)
	{
		S2CON &= ~S2RI;         //���S2RIλ
		//P0 = S2BUF;             //P0��ʾ��������
		//P2 = (S2CON & S2RB8);   //P2.2��ʾУ��λ
	}
	if (S2CON & S2TI)
	{
		S2CON &= ~S2TI;         //���S2TIλ
		busy = 0;               //��æ��־
	}
}

/*----------------------------
���ʹ�������with 8 bits
----------------------------*/
void Usart1_SendData(BYTE dat)
{
    while (busy);               //�ȴ�ǰ������ݷ������
    ACC = dat;                  //��ȡУ��λP (PSW.0)
    if (P)                      //����P������У��λ
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;                //����У��λΪ0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;                //����У��λΪ1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;                //����У��λΪ1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;                //����У��λΪ0
#endif
    }
    busy = 1;
    SBUF = ACC;                 //д���ݵ�UART���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
void Usart1_SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
		Usart1_SendData(*s++);         //���͵�ǰ�ַ�
    }
}

/*----------------------------
���ʹ�������
----------------------------*/
void Usart2_SendData(BYTE dat)
{
	while (busy);               //�ȴ�ǰ������ݷ������
	ACC = dat;                  //��ȡУ��λP (PSW.0)
	if (P)                      //����P������У��λ
	{
#if (PARITYBIT == ODD_PARITY)
		S2CON &= ~S2TB8;        //����У��λΪ0
#elif (PARITYBIT == EVEN_PARITY)
		S2CON |= S2TB8;         //����У��λΪ1
#endif
	}
	else
	{
#if (PARITYBIT == ODD_PARITY)
		S2CON |= S2TB8;         //����У��λΪ1
#elif (PARITYBIT == EVEN_PARITY)
		S2CON &= ~S2TB8;        //����У��λΪ0
#endif
	}
	busy = 1;
	S2BUF = ACC;                //д���ݵ�UART2���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
void Usart2_SendString(char *s)
{
	while (*s)                  //����ַ���������־
	{
		Usart2_SendData(*s++);         //���͵�ǰ�ַ�
	}
}


/*uart init with baud settings----transfer mode-----open serial port*/
void UsartInit(void)
{
    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
    
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
//  ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
//  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
//  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC;  

	P_SW2 &= ~S2_S0;            //S2_S0=0 (P1.0/RxD2, P1.1/TxD2)
	//  P_SW2 |= S2_S0;             //S2_S0=1 (P4.6/RxD2_2, P4.7/TxD2_2)

#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;                //8λ�ɱ䲨����
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;                //9λ�ɱ䲨����,У��λ��ʼΪ1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;                //9λ�ɱ䲨����,У��λ��ʼΪ0
#endif

#if (PARITYBIT == NONE_PARITY)
	S2CON = 0x50;               //8λ�ɱ䲨����
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
	S2CON = 0xda;               //9λ�ɱ䲨����,У��λ��ʼΪ1
#elif (PARITYBIT == SPACE_PARITY)
	S2CON = 0xd2;               //9λ�ɱ䲨����,У��λ��ʼΪ0
#endif


    T2L = (65536 - (FOSC/4/BAUD));   //���ò�������װֵ
    T2H = (65536 - (FOSC/4/BAUD))>>8;
    AUXR = 0x14;                //T2Ϊ1Tģʽ, ��������ʱ��2
    AUXR |= 0x01;               //ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    ES = 1;                     //ʹ�ܴ����ж�
	IE2 = 0x01;                 //ʹ�ܴ���2�ж�
    EA = 1;

	//Usart1_SendString("STC15F2K60S2\r\nUart Test !\r\n");
	//Usart2_SendString("STC15F2K60S2\r\nUart2 Test !\r\n");
	
}

void uart_printf(const char *fmt, ...)
{
	va_list ap;
	char xdata string[1024];//�����ڲ���չRAM���Ƿ����ⲿRAM�����ܳ����ڲ���չRAM��С(�˴�Ϊ1024)  

	va_start(ap, fmt);
	vsprintf(string, fmt, ap);//�˴�Ҳ����ʹ��sprintf�������÷���࣬�Լ��޸ļ��ɣ��˴���ȥ  
	Usart1_SendString(string);
	va_end(ap);
}

///*Switch to uart2 port by hardware*/
//void usart2_SendData(void)
//{
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
//  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC; 
//}
//
///*Switch to uart1 port by hardware*/
//void usart1_SendData(void)
//{
//    ACC = P_SW1;
//    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
//    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
//}

