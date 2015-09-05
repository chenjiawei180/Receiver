#ifndef __AT24C256_H_
#define __AT24C256_H_

#include "STC15F2K60S2.H"
#include "intrins.h"

#define  _Nop()  _nop_()        //�����ָ��

sbit SDA = P3 ^ 4;            //ģ��I2C���ݴ���λ
sbit SCL = P3 ^ 5;            //ģ��I2Cʱ�ӿ���λ

#define  _Nop()  _nop_()        //�����ָ��
#define  I2C_ADDRESS 0xa0
#define  PAGE_LENGTH 32

extern void DelayUs2x(unsigned char t);//�������� 
extern void DelayMs(unsigned char t);
extern void Start_I2c();
extern void Stop_I2c();
extern void  SendByte(unsigned char c);
extern unsigned char  RcvByte();
extern void Ack_I2c(void);
extern void NoAck_I2c(void);

#endif