#ifndef __AT24C256_H_
#define __AT24C256_H_

#include "STC15F2K60S2.H"
#include "intrins.h"

#define  _Nop()  _nop_()        //定义空指令

sbit SDA = P3 ^ 4;            //模拟I2C数据传送位
sbit SCL = P3 ^ 5;            //模拟I2C时钟控制位

typedef struct Env_struct{
	unsigned char Two_Menu_F3_E1;     //及时模式或者排队模式
	unsigned char Two_Menu_F3_E2;     //呼叫存储数量
	unsigned char Two_Menu_F4_E1;     //销号时间
	unsigned char Two_Menu_F5_E1;     //循环时间
	unsigned char Two_Menu_F6_E1;    //语音模式
	unsigned char Two_Menu_F6_E2;      //语音报读次数
	unsigned char Two_Menu_F6_E3;   //循环时候是否报读
	unsigned char Two_Menu_F6_E4;  //音量大小调整
	unsigned char Two_Menu_F6_E5;  //显示屏LED亮度调整
	unsigned char Two_Menu_F6_E6;    //语音导航
	unsigned char Two_Menu_F6_E7;     //中英文
	unsigned char Two_Menu_F7_E1;     //防区设置
	unsigned char Two_Menu_F8_E1;     //单键位还是多键位
	unsigned char Two_Menu_Fb_E1;   //主机是否有销号功能
	unsigned char Two_Menu_FC_E1;    //万年历待机与----
}Env_t;


#define  _Nop()  _nop_()        //定义空指令
#define  I2C_ADDRESS 0xa0
#define  PAGE_LENGTH 32

#define  CALL_TABLE_NUMBER   32
#define  HOST_TABLE_NUMBER   8
#define  ALARM_TABLE_NUMBER  8
#define  CANCEL_TABLE_NUMBER 8
#define  ALL_TABLE_NUMBER    56
#define  VARIA_NUMBER        15
//标志位地址
#define CALL_TABLE_START    0x0000
#define CALL_TABLE_END      0x03FF
#define ALARM_TABLE_START   0X0400
#define ALARM_TABLE_END     0x04FF
#define CANCEL_TABLE_START  0X0500
#define CANCEL_TABLE_END    0x05FF
#define HOST_TABLE_START    0X0600
#define HOST_TABLE_END      0x06FF

//数据存储地址
#define CALL_DATA_START    0x0700
#define CALL_DATA_END      0x26FF // 1024*8=8192 =2000H
#define ALARM_DATA_START   0X2700 
#define ALARM_DATA_END     0x2EFF // 256*8 =2048 =800H
#define CANCEL_DATA_START  0X2F00
#define CANCEL_DATA_END    0x36FF // 256*8 =2048 =800H
#define HOST_DATA_START    0X3700 
#define HOST_DATA_END      0x3EFF // 256*8 =2048 =800H
#define SIN_KEY            0X3F00
#define MUL_KEY            0X3F10
#define BACK               0X3F20

extern Env_t	EEPROM;

extern void DelayUs2x(unsigned char t);//函数声明 
extern void DelayMs(unsigned char t);
extern void Start_I2c();
extern void Stop_I2c();
extern void SendByte(unsigned char c);
extern unsigned char RcvByte();
extern bit ISendStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no);
extern bit IRcvStr(unsigned char sla, unsigned int suba, unsigned char *s, unsigned char no);
extern void Ack_I2c(void);
extern void NoAck_I2c(void);

extern bit register_call_function(unsigned char *buf);
extern bit delete_call_function(unsigned char *buf);
extern bit register_host_function(unsigned char *buf);
extern bit delete_host_function(unsigned char *buf);
extern bit register_alarm_function(unsigned char *buf);
extern bit delete_alarm_function(unsigned char *buf);
extern bit register_cancel_function(unsigned char *buf);
extern bit delete_cancel_function(unsigned char *buf);
extern void Delete_all_data(void);

#endif