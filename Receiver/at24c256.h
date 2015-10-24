#ifndef __AT24C256_H_
#define __AT24C256_H_

#include "STC15F2K60S2.H"
#include "intrins.h"

#define  _Nop()  _nop_()        //�����ָ��

sbit SDA = P3 ^ 4;            //ģ��I2C���ݴ���λ
sbit SCL = P3 ^ 5;            //ģ��I2Cʱ�ӿ���λ

typedef struct Env_struct{
	unsigned char Two_Menu_F3_E1;     //��ʱģʽ�����Ŷ�ģʽ
	unsigned char Two_Menu_F3_E2;     //���д洢����
	unsigned char Two_Menu_F4_E1;     //����ʱ��
	unsigned char Two_Menu_F5_E1;     //ѭ��ʱ��
	unsigned char Two_Menu_F6_E1;    //����ģʽ
	unsigned char Two_Menu_F6_E2;      //������������
	unsigned char Two_Menu_F6_E3;   //ѭ��ʱ���Ƿ񱨶�
	unsigned char Two_Menu_F6_E4;  //������С����
	unsigned char Two_Menu_F6_E5;  //��ʾ��LED���ȵ���
	unsigned char Two_Menu_F6_E6;    //��������
	unsigned char Two_Menu_F6_E7;     //��Ӣ��
	unsigned char Two_Menu_F7_E1;     //��������
	unsigned char Two_Menu_F8_E1;     //����λ���Ƕ��λ
	unsigned char Two_Menu_Fb_E1;   //�����Ƿ������Ź���
	unsigned char Two_Menu_FC_E1;    //������������----
}Env_t;


#define  _Nop()  _nop_()        //�����ָ��
#define  I2C_ADDRESS 0xa0
#define  PAGE_LENGTH 32

#define  CALL_TABLE_NUMBER   16
#define  HOST_TABLE_NUMBER   1
#define  ALARM_TABLE_NUMBER  2
#define  CANCEL_TABLE_NUMBER 2
#define  ALL_TABLE_NUMBER    21
#define  VARIA_NUMBER        15
//��־λ��ַ
#define CALL_TABLE_START    0x0000
#define CALL_TABLE_END      0x01FF
#define ALARM_TABLE_START   0X0200
#define ALARM_TABLE_END     0x023F
#define CANCEL_TABLE_START  0X0240
#define CANCEL_TABLE_END    0x027F
#define HOST_TABLE_START    0X0280
#define HOST_TABLE_END      0x029F

//���ݴ洢��ַ
#define CALL_DATA_START    0x02A0
#define CALL_DATA_END      0x129F // 1024*8=8192 =2000H
#define ALARM_DATA_START   0X12A0
#define ALARM_DATA_END     0x159F // 256*8 =2048 =800H
#define CANCEL_DATA_START  0X14A0
#define CANCEL_DATA_END    0x179F // 256*8 =2048 =800H
#define HOST_DATA_START    0X16A0
#define HOST_DATA_END      0x179F // 256*8 =2048 =800H
#define SIN_KEY            0X17A0
#define MUL_KEY            0X17B0
#define BACK               0X17C0

extern Env_t	EEPROM;

extern void DelayUs2x(unsigned char t);//�������� 
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