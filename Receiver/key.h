#ifndef __KEY_H_
#define __KEY_H_

#include "STC15F2K60S2.h"
#include "intrins.h"

#define KEY_FUNC	0
#define KEY_UP     	1
#define KEY_DOWN	2
#define KEY_RETURN	3

#define HKeyPort  P1
#define LKeyPort  P2

typedef struct
{
	unsigned char index;
	unsigned char up;//Add index number
	unsigned char down;//Reduced index number
	unsigned char enter;//Enter the index number
	unsigned char esc;//Exit index number
	void(*index_operation)();//Function index
} key_table;

extern unsigned int KeyScan(void);//Keyboard scan function
extern void delay10ms(void);// Delay 10 ms function
extern unsigned char KeyDecoder(void);//解码函数
extern void KeyProcess(void);//按键处理函数

#endif

