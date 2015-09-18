#ifndef __AT24C256_H_
#define __AT24C256_H_

#include "STC15F2K60S2.H"
#include "intrins.h"

#define  _Nop()  _nop_()        //定义空指令

sbit SDA = P3 ^ 4;            //模拟I2C数据传送位
sbit SCL = P3 ^ 5;            //模拟I2C时钟控制位

#define  _Nop()  _nop_()        //定义空指令
#define  I2C_ADDRESS 0xa0
#define  PAGE_LENGTH 32

#define  CALL_TABLE_NUMBER   32
#define  HOST_TABLE_NUMBER   8
#define  ALARM_TABLE_NUMBER  8
#define  CANCEL_TABLE_NUMBER 8
#define  ALL_TABLE_NUMBER    56

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