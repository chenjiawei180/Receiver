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

extern unsigned char Two_Menu_F1_E1[4];
extern unsigned char Two_Menu_F1_E2[4];
extern unsigned char Two_Menu_F1_E3[4];
extern unsigned char Two_Menu_F1_E4[4];
extern unsigned char Two_Menu_F2_E1[4];
extern unsigned char Two_Menu_F2_E2[4];
extern unsigned char Two_Menu_F2_E3[4];
extern unsigned char Two_Menu_F2_E4[4];

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
extern unsigned char KeyDecoder(void);//���뺯��
extern void KeyProcess(void);//����������

extern unsigned char return_func_index(void);
extern unsigned char return_Two_Menu_F3_E1(void);
extern unsigned char return_Two_Menu_F3_E2(void);
extern unsigned char return_Two_Menu_F4_E1(void);
extern unsigned char return_Two_Menu_F5_E1(void);
extern unsigned char return_Two_Menu_F6_E1(void);
extern unsigned char return_Two_Menu_F6_E2(void);
extern unsigned char return_Two_Menu_F6_E3(void);
extern unsigned char return_Two_Menu_F6_E4(void);
extern unsigned char return_Two_Menu_F6_E5(void);
extern unsigned char return_Two_Menu_F7_E1(void);
extern unsigned char return_Two_Menu_F7_E2(void);
extern unsigned char return_Two_Menu_F7_E3(void);
extern unsigned char return_Two_Menu_F7_E4(void);
extern unsigned char return_Two_Menu_F8_E1(void);
extern unsigned char return_Two_Menu_F8_E2(void);
#endif

