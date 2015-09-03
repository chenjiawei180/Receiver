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


#define FOSC 12000000L          //系统频率
#define BAUD 9600             //串口波特率

#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验

#define PARITYBIT NONE_PARITY   //定义校验位


#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

void SendData(BYTE dat);
void SendString(char *s);
void UsartInit(void);
void uart_printf(const char *fmt, ...);
//void usart1_SendData(void);
//void usart2_SendData(void);

#endif

