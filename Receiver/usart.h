#ifndef __USART_H_
#define __USART_H_
/*************************************************
this file will define all  usart  function 
************************************************/
#define USART 
#define DEBUG
#include "STC15F2K60S2.h"
#include<stdio.h>  
#include<stdarg.h> 

#define  BYTE unsigned char 


#define FOSC 12000000L          //ϵͳƵ��
#define BAUD 9600             //���ڲ�����

#define NONE_PARITY     0       //��У��
#define ODD_PARITY      1       //��У��
#define EVEN_PARITY     2       //żУ��
#define MARK_PARITY     3       //���У��
#define SPACE_PARITY    4       //�հ�У��

#define PARITYBIT NONE_PARITY   //����У��λ


#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

void SendData(BYTE dat);
void SendString(char *s);
void UsartInit(void);
void uart_printf(const char *fmt, ...);
//void usart1_SendData(void);
//void usart2_SendData(void);

#endif

