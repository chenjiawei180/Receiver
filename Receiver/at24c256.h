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

extern void DelayUs2x(unsigned char t);//函数声明 
extern void DelayMs(unsigned char t);
extern void Start_I2c();
extern void Stop_I2c();
extern void  SendByte(unsigned char c);
extern unsigned char  RcvByte();
extern void Ack_I2c(void);
extern void NoAck_I2c(void);

#endif