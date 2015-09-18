#include "key.h"
#include "usart.h"
#include "tm1629.h"
#include "menu.h"
#include "timer.h"
#include "ds1302.h"
#include "at24c256.h"

unsigned char func_index = 0; //�༶�˵���������
void(*current_operation_index)();// �༶�˵�����ָ��

unsigned char Two_Menu_F1_E1[4] = { 0, 0, 0, 1 }; //F1_E1
unsigned char Two_Menu_F1_E2[4] = { 0, 0, 0, 1 }; //F1_E2
unsigned char Two_Menu_F1_E3[4] = { 0, 0, 0, 1 }; //F1_E3
unsigned char Two_Menu_F1_E4[4] = { 0, 0, 0, 1 }; //F1_E4
unsigned char Two_Menu_F2_E1[4] = { 0, 0, 0, 1 }; //F2_E1
unsigned char Two_Menu_F2_E2[4] = { 0, 0, 0, 1 }; //F2_E2
unsigned char Two_Menu_F2_E3[4] = { 0, 0, 0, 1 }; //F2_E3
unsigned char Two_Menu_F2_E4[4] = { 0, 0, 0, 1 }; //F2_E4

unsigned char Two_Menu_F3_E1 = 1; //��ʱģʽ�����Ŷ���ʾ
unsigned char Two_Menu_F3_E2 = 7; //����ʱ��洢����

unsigned char Two_Menu_F4_E1 = 0; //����ʱ��
unsigned char Two_Menu_F5_E1 = 0; //ѭ�����ʱ��

unsigned char Two_Menu_F6_E1 = 0; //�򵥱���
unsigned char Two_Menu_F6_E2 = 1; //������������
unsigned char Two_Menu_F6_E3 = 0; //ѭ��ʱ���Ƿ񱨶�
unsigned char Two_Menu_F6_E4 = 0; //������С����
unsigned char Two_Menu_F6_E5 = 0; //��ʾ��LED���ȵ���

unsigned char Two_Menu_F7_E1 = 0; // E1Ĭ�ϼ��̹��� 999*9
unsigned char Two_Menu_F7_E2 = 0; // E2�������̹��� 9999*9
unsigned char Two_Menu_F7_E3 = 0; // E3�������̹��� 999*99
unsigned char Two_Menu_F7_E4 = 0; // E4�������̹��� 9999*99

unsigned char Two_Menu_F8_E1 = 2; // �������� �ఴ���л�
unsigned char Two_Menu_F8_E2 = 0; // ��λ����

unsigned char Two_Menu_Fb_E1 = 0; // ����������û�����Ź���
unsigned char Two_Menu_FC_E1 = 0; // ����������������----�������л�
unsigned char Two_Menu_Fd_E1 = 0; // E1 E2 E3 E4 E5 E6

key_table code table[100] =
{	// Ŀ������		    ��				��          ȷ��		 �˳�         ����
	{ MENU_STANDBY, MENU_STANDBY, MENU_STANDBY, MENU_STANDBY, MENU_STANDBY, (*fun0) }, //����

	{ ONE_MENU_F0, ONE_MENU_F1, ONE_MENU_Fd, TWO_MENU_F0_YEAR , MENU_STANDBY, (*fun1) }, //F0-F9
	{ ONE_MENU_F1, ONE_MENU_F2, ONE_MENU_F0, TWO_MENU_F1_E1   , MENU_STANDBY, (*fun2) },
	{ ONE_MENU_F2, ONE_MENU_F3, ONE_MENU_F1, TWO_MENU_F2_E1   , MENU_STANDBY, (*fun3) },
	{ ONE_MENU_F3, ONE_MENU_F4, ONE_MENU_F2, TWO_MENU_F3_E1   , MENU_STANDBY, (*fun4) },
	{ ONE_MENU_F4, ONE_MENU_F5, ONE_MENU_F3, TWO_MENU_F4_SET  , MENU_STANDBY, (*fun5) },
	{ ONE_MENU_F5, ONE_MENU_F6, ONE_MENU_F4, TWO_MENU_F5_SET  , MENU_STANDBY, (*fun6) },
	{ ONE_MENU_F6, ONE_MENU_F7, ONE_MENU_F5, TWO_MENU_F6_E1   , MENU_STANDBY, (*fun7) },
	{ ONE_MENU_F7, ONE_MENU_F8, ONE_MENU_F6, TWO_MENU_F7_E1   , MENU_STANDBY, (*fun8) },
	{ ONE_MENU_F8, ONE_MENU_F9, ONE_MENU_F7, TWO_MENU_F8_E1   , MENU_STANDBY, (*fun9) },
	{ ONE_MENU_F9, ONE_MENU_FA, ONE_MENU_F8, TWO_MENU_F9_E1   , MENU_STANDBY, (*fun10) },
	{ ONE_MENU_FA, ONE_MENU_Fb, ONE_MENU_F9, TWO_MENU_FA_SET  , MENU_STANDBY, (*fun11) }, //FA
	{ ONE_MENU_Fb, ONE_MENU_FC, ONE_MENU_FA, TWO_MENU_Fb_SET  , MENU_STANDBY, (*fun12) }, //Fb
	{ ONE_MENU_FC, ONE_MENU_Fd, ONE_MENU_Fb, TWO_MENU_FC_SET  , MENU_STANDBY, (*fun13) }, //FC
	{ ONE_MENU_Fd, ONE_MENU_F0, ONE_MENU_FC, TWO_MENU_Fd_SET  , MENU_STANDBY, (*fun14) }, //Fd

	{ TWO_MENU_F0_YEAR  , TWO_MENU_F0_YEAR  , TWO_MENU_F0_YEAR  , TWO_MENU_F0_MOUTH , ONE_MENU_F0, (*fun15) }, //������ ��
	{ TWO_MENU_F0_MOUTH , TWO_MENU_F0_MOUTH , TWO_MENU_F0_MOUTH , TWO_MENU_F0_DAY   , ONE_MENU_F0, (*fun16) }, //������ ��
	{ TWO_MENU_F0_DAY   , TWO_MENU_F0_DAY   , TWO_MENU_F0_DAY   , TWO_MENU_F0_WEEK  , ONE_MENU_F0, (*fun17) }, //������ ��
	{ TWO_MENU_F0_WEEK  , TWO_MENU_F0_WEEK  , TWO_MENU_F0_WEEK  , TWO_MENU_F0_HOUR  , ONE_MENU_F0, (*fun18) }, //������ ��
	{ TWO_MENU_F0_HOUR  , TWO_MENU_F0_HOUR  , TWO_MENU_F0_HOUR  , TWO_MENU_F0_MINUTE, ONE_MENU_F0, (*fun19) }, //������ Сʱ
	{ TWO_MENU_F0_MINUTE, TWO_MENU_F0_MINUTE, TWO_MENU_F0_MINUTE, TWO_MENU_F0_YEAR  , ONE_MENU_F0, (*fun20) }, //������ ����

	{ TWO_MENU_F1_E1, TWO_MENU_F1_E2, TWO_MENU_F1_E4, TWO_MENU_F1_E1_D4, ONE_MENU_F1, (*fun21) }, //F1�Ӳ˵�E1
	{ TWO_MENU_F1_E2, TWO_MENU_F1_E3, TWO_MENU_F1_E1, TWO_MENU_F1_E2_D4, ONE_MENU_F1, (*fun22) }, //F1�Ӳ˵�E2
	{ TWO_MENU_F1_E3, TWO_MENU_F1_E4, TWO_MENU_F1_E2, TWO_MENU_F1_E3_D4, ONE_MENU_F1, (*fun23) }, //F1�Ӳ˵�E3
	{ TWO_MENU_F1_E4, TWO_MENU_F1_E1, TWO_MENU_F1_E3, TWO_MENU_F1_E4_D4, ONE_MENU_F1, (*fun24) }, //F1�Ӳ˵�E4

	{ TWO_MENU_F2_E1, TWO_MENU_F2_E2, TWO_MENU_F2_E4, TWO_MENU_F2_E1_D4, ONE_MENU_F2, (*fun25) }, //F2�Ӳ˵�E1
	{ TWO_MENU_F2_E2, TWO_MENU_F2_E3, TWO_MENU_F2_E1, TWO_MENU_F2_E2_D4, ONE_MENU_F2, (*fun26) }, //F2�Ӳ˵�E2
	{ TWO_MENU_F2_E3, TWO_MENU_F2_E4, TWO_MENU_F2_E2, TWO_MENU_F2_E3_D4, ONE_MENU_F2, (*fun27) }, //F2�Ӳ˵�E3
	{ TWO_MENU_F2_E4, TWO_MENU_F2_E1, TWO_MENU_F2_E3, TWO_MENU_F2_E4_D4, ONE_MENU_F2, (*fun28) }, //F2�Ӳ˵�E4

	{ TWO_MENU_F3_E1, TWO_MENU_F3_E2, TWO_MENU_F3_E2, TWO_MENU_F3_E1_SET, ONE_MENU_F3, (*fun29) }, //F3�Ӳ˵�E1
	{ TWO_MENU_F3_E2, TWO_MENU_F3_E1, TWO_MENU_F3_E1, TWO_MENU_F3_E2_SET, ONE_MENU_F3, (*fun30) }, //F3�Ӳ˵�E2

	{ TWO_MENU_F4_SET, TWO_MENU_F4_SET, TWO_MENU_F4_SET, TWO_MENU_F4_SET, ONE_MENU_F4, (*fun31) }, //F4�Ӳ˵�
	{ TWO_MENU_F5_SET, TWO_MENU_F5_SET, TWO_MENU_F5_SET, TWO_MENU_F5_SET, ONE_MENU_F5, (*fun32) }, //F5�Ӳ˵�

	{ TWO_MENU_F6_E1, TWO_MENU_F6_E2, TWO_MENU_F6_E5, TWO_MENU_F6_E1_SET, ONE_MENU_F6, (*fun33) }, //F6�Ӳ˵�E1
	{ TWO_MENU_F6_E2, TWO_MENU_F6_E3, TWO_MENU_F6_E1, TWO_MENU_F6_E2_SET, ONE_MENU_F6, (*fun34) }, //F6�Ӳ˵�E2
	{ TWO_MENU_F6_E3, TWO_MENU_F6_E4, TWO_MENU_F6_E2, TWO_MENU_F6_E3_SET, ONE_MENU_F6, (*fun35) }, //F6�Ӳ˵�E3
	{ TWO_MENU_F6_E4, TWO_MENU_F6_E5, TWO_MENU_F6_E3, TWO_MENU_F6_E4_SET, ONE_MENU_F6, (*fun36) }, //F6�Ӳ˵�E4
	{ TWO_MENU_F6_E5, TWO_MENU_F6_E1, TWO_MENU_F6_E4, TWO_MENU_F6_E5_SET, ONE_MENU_F6, (*fun37) }, //F6�Ӳ˵�E5

	{ TWO_MENU_F7_E1, TWO_MENU_F7_E2, TWO_MENU_F7_E4, TWO_MENU_F7_E1_SET, ONE_MENU_F7, (*fun38) }, //F7�Ӳ˵�E1
	{ TWO_MENU_F7_E2, TWO_MENU_F7_E3, TWO_MENU_F7_E1, TWO_MENU_F7_E2_SET, ONE_MENU_F7, (*fun39) }, //F7�Ӳ˵�E2
	{ TWO_MENU_F7_E3, TWO_MENU_F7_E4, TWO_MENU_F7_E2, TWO_MENU_F7_E3_SET, ONE_MENU_F7, (*fun40) }, //F7�Ӳ˵�E3
	{ TWO_MENU_F7_E4, TWO_MENU_F7_E1, TWO_MENU_F7_E3, TWO_MENU_F7_E4_SET, ONE_MENU_F7, (*fun41) }, //F7�Ӳ˵�E4	

	{ TWO_MENU_F8_E1, TWO_MENU_F8_E2, TWO_MENU_F8_E2, TWO_MENU_F8_E1_SET, ONE_MENU_F8, (*fun42) }, //F8�Ӳ˵�E1
	{ TWO_MENU_F8_E2, TWO_MENU_F8_E1, TWO_MENU_F8_E1, TWO_MENU_F8_E2_SET, ONE_MENU_F8, (*fun43) }, //F8�Ӳ˵�E2

	{ TWO_MENU_F9_E1, TWO_MENU_F9_E2, TWO_MENU_F9_E2, 0, ONE_MENU_F9, (*fun44) }, //F9�Ӳ˵�E1
	{ TWO_MENU_F9_E2, TWO_MENU_F9_E1, TWO_MENU_F9_E1, 0, ONE_MENU_F9, (*fun45) }, //F9�Ӳ˵�E2

	{ TWO_MENU_FA_SET, TWO_MENU_FA_SET, TWO_MENU_FA_SET, 0, ONE_MENU_FA, (*fun46) }, //FA�Ӳ˵�
	{ TWO_MENU_Fb_SET, TWO_MENU_Fb_SET, TWO_MENU_Fb_SET, 0, ONE_MENU_Fb, (*fun47) }, //Fb�Ӳ˵�
	{ TWO_MENU_FC_SET, TWO_MENU_FC_SET, TWO_MENU_FC_SET, 0, ONE_MENU_FC, (*fun48) }, //FC�Ӳ˵�
	{ TWO_MENU_Fd_SET, TWO_MENU_Fd_SET, TWO_MENU_Fd_SET, 0, ONE_MENU_Fd, (*fun49) }, //Fd�Ӳ˵�

	{ TWO_MENU_F1_E1_D1, TWO_MENU_F1_E1_D1, TWO_MENU_F1_E1_D1, TWO_MENU_F1_E1_D4, TWO_MENU_F1_E1, (*fun50) }, //F1_E1ע�������
	{ TWO_MENU_F1_E1_D2, TWO_MENU_F1_E1_D2, TWO_MENU_F1_E1_D2, TWO_MENU_F1_E1_D1, TWO_MENU_F1_E1, (*fun51) }, //F1_E1ע�������
	{ TWO_MENU_F1_E1_D3, TWO_MENU_F1_E1_D3, TWO_MENU_F1_E1_D3, TWO_MENU_F1_E1_D2, TWO_MENU_F1_E1, (*fun52) }, //F1_E1ע�������
	{ TWO_MENU_F1_E1_D4, TWO_MENU_F1_E1_D4, TWO_MENU_F1_E1_D4, TWO_MENU_F1_E1_D3, TWO_MENU_F1_E1, (*fun53) }, //F1_E1ע�������

	{ TWO_MENU_F1_E2_D1, TWO_MENU_F1_E2_D1, TWO_MENU_F1_E2_D1, TWO_MENU_F1_E2_D4, TWO_MENU_F1_E2, (*fun54) }, //F1_E2ע�������
	{ TWO_MENU_F1_E2_D2, TWO_MENU_F1_E2_D2, TWO_MENU_F1_E2_D2, TWO_MENU_F1_E2_D1, TWO_MENU_F1_E2, (*fun55) }, //F1_E2ע�������
	{ TWO_MENU_F1_E2_D3, TWO_MENU_F1_E2_D3, TWO_MENU_F1_E2_D3, TWO_MENU_F1_E2_D2, TWO_MENU_F1_E2, (*fun56) }, //F1_E2ע�������
	{ TWO_MENU_F1_E2_D4, TWO_MENU_F1_E2_D4, TWO_MENU_F1_E2_D4, TWO_MENU_F1_E2_D3, TWO_MENU_F1_E2, (*fun57) }, //F1_E2ע�������

	{ TWO_MENU_F1_E3_D1, TWO_MENU_F1_E3_D1, TWO_MENU_F1_E3_D1, TWO_MENU_F1_E3_D4, TWO_MENU_F1_E3, (*fun58) }, //F1_E3ע�ᱨ����
	{ TWO_MENU_F1_E3_D2, TWO_MENU_F1_E3_D2, TWO_MENU_F1_E3_D2, TWO_MENU_F1_E3_D1, TWO_MENU_F1_E3, (*fun59) }, //F1_E3ע�ᱨ����
	{ TWO_MENU_F1_E3_D3, TWO_MENU_F1_E3_D3, TWO_MENU_F1_E3_D3, TWO_MENU_F1_E3_D2, TWO_MENU_F1_E3, (*fun60) }, //F1_E3ע�ᱨ����
	{ TWO_MENU_F1_E3_D4, TWO_MENU_F1_E3_D4, TWO_MENU_F1_E3_D4, TWO_MENU_F1_E3_D3, TWO_MENU_F1_E3, (*fun61) }, //F1_E3ע�ᱨ����

	{ TWO_MENU_F1_E4_D1, TWO_MENU_F1_E4_D1, TWO_MENU_F1_E4_D1, TWO_MENU_F1_E4_D4, TWO_MENU_F1_E4, (*fun62) }, //F1_E3ע��ȡ����
	{ TWO_MENU_F1_E4_D2, TWO_MENU_F1_E4_D2, TWO_MENU_F1_E4_D2, TWO_MENU_F1_E4_D1, TWO_MENU_F1_E4, (*fun63) }, //F1_E3ע��ȡ����
	{ TWO_MENU_F1_E4_D3, TWO_MENU_F1_E4_D3, TWO_MENU_F1_E4_D3, TWO_MENU_F1_E4_D2, TWO_MENU_F1_E4, (*fun64) }, //F1_E3ע��ȡ����
	{ TWO_MENU_F1_E4_D4, TWO_MENU_F1_E4_D4, TWO_MENU_F1_E4_D4, TWO_MENU_F1_E4_D3, TWO_MENU_F1_E4, (*fun65) }, //F1_E3ע��ȡ����

	{ TWO_MENU_F2_E1_D1, TWO_MENU_F2_E1_D1, TWO_MENU_F2_E1_D1, TWO_MENU_F2_E1_D4, TWO_MENU_F2_E1, (*fun66) }, //F2_E1ɾ��������
	{ TWO_MENU_F2_E1_D2, TWO_MENU_F2_E1_D2, TWO_MENU_F2_E1_D2, TWO_MENU_F2_E1_D1, TWO_MENU_F2_E1, (*fun67) }, //F2_E1ɾ��������
	{ TWO_MENU_F2_E1_D3, TWO_MENU_F2_E1_D3, TWO_MENU_F2_E1_D3, TWO_MENU_F2_E1_D2, TWO_MENU_F2_E1, (*fun68) }, //F2_E1ɾ��������
	{ TWO_MENU_F2_E1_D4, TWO_MENU_F2_E1_D4, TWO_MENU_F2_E1_D4, TWO_MENU_F2_E1_D3, TWO_MENU_F2_E1, (*fun69) }, //F2_E1ɾ��������

	{ TWO_MENU_F2_E2_D1, TWO_MENU_F2_E2_D1, TWO_MENU_F2_E2_D1, TWO_MENU_F2_E2_D4, TWO_MENU_F2_E2, (*fun70) }, //F2_E2ɾ��������
	{ TWO_MENU_F2_E2_D2, TWO_MENU_F2_E2_D2, TWO_MENU_F2_E2_D2, TWO_MENU_F2_E2_D1, TWO_MENU_F2_E2, (*fun71) }, //F2_E2ɾ��������
	{ TWO_MENU_F2_E2_D3, TWO_MENU_F2_E2_D3, TWO_MENU_F2_E2_D3, TWO_MENU_F2_E2_D2, TWO_MENU_F2_E2, (*fun72) }, //F2_E2ɾ��������
	{ TWO_MENU_F2_E2_D4, TWO_MENU_F2_E2_D4, TWO_MENU_F2_E2_D4, TWO_MENU_F2_E2_D3, TWO_MENU_F2_E2, (*fun73) }, //F2_E2ɾ��������

	{ TWO_MENU_F2_E3_D1, TWO_MENU_F2_E3_D1, TWO_MENU_F2_E3_D1, TWO_MENU_F2_E3_D4, TWO_MENU_F2_E3, (*fun74) }, //F2_E3ɾ��������
	{ TWO_MENU_F2_E3_D2, TWO_MENU_F2_E3_D2, TWO_MENU_F2_E3_D2, TWO_MENU_F2_E3_D1, TWO_MENU_F2_E3, (*fun75) }, //F2_E3ɾ��������
	{ TWO_MENU_F2_E3_D3, TWO_MENU_F2_E3_D3, TWO_MENU_F2_E3_D3, TWO_MENU_F2_E3_D2, TWO_MENU_F2_E3, (*fun76) }, //F2_E3ɾ��������
	{ TWO_MENU_F2_E3_D4, TWO_MENU_F2_E3_D4, TWO_MENU_F2_E3_D4, TWO_MENU_F2_E3_D3, TWO_MENU_F2_E3, (*fun77) }, //F2_E3ɾ��������

	{ TWO_MENU_F2_E4_D1, TWO_MENU_F2_E4_D1, TWO_MENU_F2_E4_D1, TWO_MENU_F2_E4_D4, TWO_MENU_F2_E4, (*fun78) }, //F2_E4ɾ��ȡ����
	{ TWO_MENU_F2_E4_D2, TWO_MENU_F2_E4_D2, TWO_MENU_F2_E4_D2, TWO_MENU_F2_E4_D1, TWO_MENU_F2_E4, (*fun79) }, //F2_E4ɾ��ȡ����
	{ TWO_MENU_F2_E4_D3, TWO_MENU_F2_E4_D3, TWO_MENU_F2_E4_D3, TWO_MENU_F2_E4_D2, TWO_MENU_F2_E4, (*fun80) }, //F2_E4ɾ��ȡ����
	{ TWO_MENU_F2_E4_D4, TWO_MENU_F2_E4_D4, TWO_MENU_F2_E4_D4, TWO_MENU_F2_E4_D3, TWO_MENU_F2_E4, (*fun81) }, //F2_E4ɾ��ȡ����

	{ TWO_MENU_F3_E1_SET, TWO_MENU_F3_E1_SET, TWO_MENU_F3_E1_SET, TWO_MENU_F3_E1_SET, TWO_MENU_F3_E1, (*fun82) }, //F3_E1�����Ŷ���ʾ����ѭ����ʾ
	{ TWO_MENU_F3_E2_SET, TWO_MENU_F3_E2_SET, TWO_MENU_F3_E2_SET, TWO_MENU_F3_E2_SET, TWO_MENU_F3_E2, (*fun83) }, //F3_E2���ö��и���

	{ TWO_MENU_F6_E1_SET, TWO_MENU_F6_E1_SET, TWO_MENU_F6_E1_SET, TWO_MENU_F6_E1_SET, TWO_MENU_F6_E1, (*fun84) }, //F6_E1����ѡ��
	{ TWO_MENU_F6_E2_SET, TWO_MENU_F6_E2_SET, TWO_MENU_F6_E2_SET, TWO_MENU_F6_E2_SET, TWO_MENU_F6_E2, (*fun85) }, //F6_E2������������
	{ TWO_MENU_F6_E3_SET, TWO_MENU_F6_E3_SET, TWO_MENU_F6_E3_SET, TWO_MENU_F6_E3_SET, TWO_MENU_F6_E3, (*fun86) }, //F6_E3ѭ��ʱ���Ƿ񱨶�
	{ TWO_MENU_F6_E4_SET, TWO_MENU_F6_E4_SET, TWO_MENU_F6_E4_SET, TWO_MENU_F6_E4_SET, TWO_MENU_F6_E4, (*fun87) }, //F6_E4������С����
	{ TWO_MENU_F6_E5_SET, TWO_MENU_F6_E5_SET, TWO_MENU_F6_E5_SET, TWO_MENU_F6_E5_SET, TWO_MENU_F6_E5, (*fun88) }, //F6_E5��Ļ���ȵ���

	{ TWO_MENU_F7_E1_SET, TWO_MENU_F7_E1_SET, TWO_MENU_F7_E1_SET, TWO_MENU_F7_E1_SET, TWO_MENU_F7_E1, (*fun89) }, //F7_E1 999*9
	{ TWO_MENU_F7_E2_SET, TWO_MENU_F7_E2_SET, TWO_MENU_F7_E2_SET, TWO_MENU_F7_E2_SET, TWO_MENU_F7_E2, (*fun90) }, //F7_E2 9999*9
	{ TWO_MENU_F7_E3_SET, TWO_MENU_F7_E3_SET, TWO_MENU_F7_E3_SET, TWO_MENU_F7_E3_SET, TWO_MENU_F7_E3, (*fun91) }, //F7_E3 999*99
	{ TWO_MENU_F7_E4_SET, TWO_MENU_F7_E4_SET, TWO_MENU_F7_E4_SET, TWO_MENU_F7_E4_SET, TWO_MENU_F7_E4, (*fun92) }, //F7_E4 9999*99

	{ TWO_MENU_F8_E1_SET, TWO_MENU_F8_E1_SET, TWO_MENU_F8_E1_SET, TWO_MENU_F8_E1_SET, TWO_MENU_F8_E1, (*fun93) }, //F8_E1 ����λ�����λ�л�
	{ TWO_MENU_F8_E2_SET, TWO_MENU_F8_E2_SET, TWO_MENU_F8_E2_SET, TWO_MENU_F8_E2_SET, TWO_MENU_F8_E2, (*fun94) }, //F8_E2 ��λ����

	{ DECODER_MENU, DECODER_MENU, DECODER_MENU, DECODER_MENU, DECODER_MENU, (*fun95)},
};

unsigned int KeyScan(void)  //Keyboard scan function
{
	unsigned int Val = 0;
	HKeyPort |= 0x1f;//Row height
	LKeyPort &= 0x07;
	if ((HKeyPort & 0x1f) != 0x1f)//Press button
	{
		delay10ms();  //Remove jitter
		if ((HKeyPort & 0x1f) != 0x1f)   //Press button
		{
			HKeyPort |= 0x1f; //����һ��
			LKeyPort |= 0xf8;
			LKeyPort &= 0x7f;
			if ((HKeyPort & 0x1f) != 0x1f)
			{
				//	return_standby_time=0;
				//	sound_table=1;
				Val = HKeyPort & 0x1f;
				Val <<= 8;
				Val += (LKeyPort & 0xf8);
				if (Val == 0x1e78)
				{
					clear_main_press_time();	//����˵������µ�ʱ��������
					set_main_press_time_table(1); //������Ӧ�ı�־λ����ʼ����ʱ��
				}
				while ((HKeyPort & 0x1f) != 0x1f);
				delay10ms();
				while ((HKeyPort & 0x1f) != 0x1f);
				set_main_press_time_table(0);//�����ͷţ������Ӧ�ı�־λ
				return Val;
			}
		}
	}
	return 0x0fff;
}

unsigned char KeyDecoder(void)
{
	unsigned int key_val = 0;

	key_val = KeyScan();
#if 0	  
	if (key_val == 0x0fff) /*do not key press*/
	{
		/*sjz ��ע���������־*/
		if (register_manager == 1)
		{

			if (func_index == FUNC_STANDBY)
			{
				if ((old2_RF_RECE_REG[2] & 0x0f) == 0x01)
				{
					accumulate_decoder++;
				}
				else
				{
					accumulate_decoder = 0;
				}
				if (accumulate_decoder>10)
				{
					accumulate_50ms = 20;
					accumulate_decoder = 0;
				}
			}
			register_manager = 0;
			switch (old2_RF_RECE_REG[2] & 0x0f)
			{
			case 0x01:key_val = 0x1778; break;
			case 0x02:key_val = 0x1b78; break;
			case 0x04:key_val = 0x1d78; break;
			case 0x08:key_val = 0x1e78; break;
			default:break;
			}
		}
	}
#endif 	 

#ifdef DEBUG
	if (key_val != 0x0fff)
	uart_printf("key_val is %x .\r\n", key_val); //���԰�����ֵ
#endif
	switch (key_val)
	{
	case 0x1778:return KEY_RETURN; break;//1 ������Ӧ�ļ���ʾ���Ӧ����ֵ
	case 0x1d78:return KEY_DOWN; break;//2  
	case 0x1b78:return KEY_UP; break;//4
	case 0x1e78:return KEY_FUNC; break;//5 ������Ӧ�ļ���ʾ���Ӧ����ֵ
	default:return 0xff; break;
	}
}

void KeyProcess(void)
{
	unsigned char key_value = 0, main_press_time_temp = 0;
	key_value = KeyDecoder();
	switch (key_value)
	{
		case KEY_FUNC:
			if (func_index == MENU_STANDBY )  //�������==0  ��Ҫ1�����Ͻ���˵�
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 20)
				{
					func_index = ONE_MENU_F0;
					clear_main_press_time();
				}
			}
			else if (func_index == TWO_MENU_F2_E1_D1 || func_index == TWO_MENU_F2_E1_D2 || func_index == TWO_MENU_F2_E1_D3 || func_index == TWO_MENU_F2_E1_D4 )
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 10)
				{
					delete_call_function(Two_Menu_F2_E1);
					delay10ms();
					clear_main_press_time();
//					if (sound_table == 1)
//					{
//#if SOUND
//						GD5800_select_chapter(SETSUCCESS_POSITION);
//#endif
//						sound_table = 0;
//					}
				}
				else
					func_index = table[func_index].enter;
			}
			else if (func_index == TWO_MENU_F2_E2_D1 || func_index == TWO_MENU_F2_E2_D2 || func_index == TWO_MENU_F2_E2_D3 || func_index == TWO_MENU_F2_E2_D4)
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 10)
				{
					delete_host_function(Two_Menu_F2_E2);
					delay10ms();
					clear_main_press_time();
					//					if (sound_table == 1)
					//					{
					//#if SOUND
					//						GD5800_select_chapter(SETSUCCESS_POSITION);
					//#endif
					//						sound_table = 0;
					//					}
				}
				else
					func_index = table[func_index].enter;
			}
			else if (func_index == TWO_MENU_F2_E3_D1 || func_index == TWO_MENU_F2_E3_D2 || func_index == TWO_MENU_F2_E3_D3 || func_index == TWO_MENU_F2_E3_D4)
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 10)
				{
					delete_alarm_function(Two_Menu_F2_E3);
					delay10ms();
					clear_main_press_time();
					//					if (sound_table == 1)
					//					{
					//#if SOUND
					//						GD5800_select_chapter(SETSUCCESS_POSITION);
					//#endif
					//						sound_table = 0;
					//					}
				}
				else
					func_index = table[func_index].enter;
			}
			else if (func_index == TWO_MENU_F2_E4_D1 || func_index == TWO_MENU_F2_E4_D2 || func_index == TWO_MENU_F2_E4_D3 || func_index == TWO_MENU_F2_E4_D4)
			{
				main_press_time_temp = return_main_press_time();
				if (main_press_time_temp >= 10)
				{
					delete_cancel_function(Two_Menu_F2_E4);
					delay10ms();
					clear_main_press_time();
					//					if (sound_table == 1)
					//					{
					//#if SOUND
					//						GD5800_select_chapter(SETSUCCESS_POSITION);
					//#endif
					//						sound_table = 0;
					//					}
				}
				else
					func_index = table[func_index].enter;
			}
			else
			{
				func_index = table[func_index].enter;
				clear_main_press_time();
			}
			 break;
		case KEY_RETURN:	func_index = table[func_index].esc; break;
		case KEY_UP:
			switch (func_index)
			{
			case TWO_MENU_F0_YEAR: 
				if (time_buf1[1] == 99) time_buf1[1] = 0;	//���������� ��
								   else time_buf1[1]++;
				break;
			case TWO_MENU_F0_MOUTH:
				if (time_buf1[2] == 12) time_buf1[2] = 1;	//���������� ��
				else time_buf1[2]++;
				break;
			case TWO_MENU_F0_DAY:
				if (time_buf1[3] == 31) time_buf1[3] = 1;	//���������� ��
				else time_buf1[3]++;
				break;
			case TWO_MENU_F0_WEEK:
				if (time_buf1[7] == 7) time_buf1[7] = 1;	//���������� ����
				else time_buf1[7]++;
				break;
			case TWO_MENU_F0_HOUR:
				if (time_buf1[4] == 23) time_buf1[4] = 0;	//���������� Сʱ
				else time_buf1[4]++;
				break;
			case TWO_MENU_F0_MINUTE:
				if (time_buf1[5] == 59) time_buf1[5] = 0;	//���������� ����
				else time_buf1[5]++;
				break;
			case TWO_MENU_F1_E1_D1:
				if (Two_Menu_F1_E1[0] == 20 ) Two_Menu_F1_E1[0] = 0;	//����F1_E1ǧλ
				else Two_Menu_F1_E1[0]++;
				break;
			case TWO_MENU_F1_E1_D2:
				if (Two_Menu_F1_E1[1] == 9) Two_Menu_F1_E1[1] = 0;	//����F1_E1��λ
				else Two_Menu_F1_E1[1]++;
				break;
			case TWO_MENU_F1_E1_D3:
				if (Two_Menu_F1_E1[2] == 9) Two_Menu_F1_E1[2] = 0;	//����F1_E1ʮλ
				else Two_Menu_F1_E1[2]++;
				break;
			case TWO_MENU_F1_E1_D4:
				if (Two_Menu_F1_E1[3] == 9) Two_Menu_F1_E1[3] = 0;	//����F1_E1��λ
				else Two_Menu_F1_E1[3]++;
				break;
			case TWO_MENU_F1_E2_D1:
				if (Two_Menu_F1_E2[0] == 20) Two_Menu_F1_E2[0] = 0;	//����F1_E2ǧλ
				else Two_Menu_F1_E2[0]++;
				break;
			case TWO_MENU_F1_E2_D2:
				if (Two_Menu_F1_E2[1] == 9) Two_Menu_F1_E2[1] = 0;	//����F1_E2��λ
				else Two_Menu_F1_E2[1]++;
				break;
			case TWO_MENU_F1_E2_D3:
				if (Two_Menu_F1_E2[2] == 9) Two_Menu_F1_E2[2] = 0;	//����F1_E2ʮλ
				else Two_Menu_F1_E2[2]++;
				break;
			case TWO_MENU_F1_E2_D4:
				if (Two_Menu_F1_E2[3] == 9) Two_Menu_F1_E2[3] = 0;	//����F1_E2��λ
				else Two_Menu_F1_E2[3]++;
				break;
			case TWO_MENU_F1_E3_D1:
				if (Two_Menu_F1_E3[0] == 20) Two_Menu_F1_E3[0] = 0;	//����F1_E3ǧλ
				else Two_Menu_F1_E3[0]++;
				break;
			case TWO_MENU_F1_E3_D2:
				if (Two_Menu_F1_E3[1] == 9) Two_Menu_F1_E3[1] = 0;	//����F1_E3��λ
				else Two_Menu_F1_E3[1]++;
				break;
			case TWO_MENU_F1_E3_D3:
				if (Two_Menu_F1_E3[2] == 9) Two_Menu_F1_E3[2] = 0;	//����F1_E3ʮλ
				else Two_Menu_F1_E3[2]++;
				break;
			case TWO_MENU_F1_E3_D4:
				if (Two_Menu_F1_E3[3] == 9) Two_Menu_F1_E3[3] = 0;	//����F1_E3��λ
				else Two_Menu_F1_E3[3]++;
				break;
			case TWO_MENU_F1_E4_D1:
				if (Two_Menu_F1_E4[0] == 20) Two_Menu_F1_E4[0] = 0;	//����F1_E4ǧλ
				else Two_Menu_F1_E4[0]++;
				break;
			case TWO_MENU_F1_E4_D2:
				if (Two_Menu_F1_E4[1] == 9) Two_Menu_F1_E4[1] = 0;	//����F1_E4��λ
				else Two_Menu_F1_E4[1]++;
				break;
			case TWO_MENU_F1_E4_D3:
				if (Two_Menu_F1_E4[2] == 9) Two_Menu_F1_E4[2] = 0;	//����F1_E4ʮλ
				else Two_Menu_F1_E4[2]++;
				break;
			case TWO_MENU_F1_E4_D4:
				if (Two_Menu_F1_E4[3] == 9) Two_Menu_F1_E4[3] = 0;	//����F1_E4��λ
				else Two_Menu_F1_E4[3]++;
				break;
			case TWO_MENU_F2_E1_D1:
				if (Two_Menu_F2_E1[0] == 20) Two_Menu_F2_E1[0] = 0;	//����F2_E1ǧλ
				else Two_Menu_F2_E1[0]++;
				break;
			case TWO_MENU_F2_E1_D2:
				if (Two_Menu_F2_E1[1] == 9) Two_Menu_F2_E1[1] = 0;	//����F2_E1��λ
				else Two_Menu_F2_E1[1]++;
				break;
			case TWO_MENU_F2_E1_D3:
				if (Two_Menu_F2_E1[2] == 9) Two_Menu_F2_E1[2] = 0;	//����F2_E1ʮλ
				else Two_Menu_F2_E1[2]++;
				break;
			case TWO_MENU_F2_E1_D4:
				if (Two_Menu_F2_E1[3] == 9) Two_Menu_F2_E1[3] = 0;	//����F2_E1��λ
				else Two_Menu_F2_E1[3]++;
				break;
			case TWO_MENU_F2_E2_D1:
				if (Two_Menu_F2_E2[0] == 20) Two_Menu_F2_E2[0] = 0;	//����F2_E2ǧλ
				else Two_Menu_F2_E2[0]++;
				break;
			case TWO_MENU_F2_E2_D2:
				if (Two_Menu_F2_E2[1] == 9) Two_Menu_F2_E2[1] = 0;	//����F2_E2��λ
				else Two_Menu_F2_E2[1]++;
				break;
			case TWO_MENU_F2_E2_D3:
				if (Two_Menu_F2_E2[2] == 9) Two_Menu_F2_E2[2] = 0;	//����F2_E2ʮλ
				else Two_Menu_F2_E2[2]++;
				break;
			case TWO_MENU_F2_E2_D4:
				if (Two_Menu_F2_E2[3] == 9) Two_Menu_F2_E2[3] = 0;	//����F2_E2��λ
				else Two_Menu_F2_E2[3]++;
				break;
			case TWO_MENU_F2_E3_D1:
				if (Two_Menu_F2_E3[0] == 20) Two_Menu_F2_E3[0] = 0;	//����F2_E3ǧλ
				else Two_Menu_F2_E3[0]++;
				break;
			case TWO_MENU_F2_E3_D2:
				if (Two_Menu_F2_E3[1] == 9) Two_Menu_F2_E3[1] = 0;	//����F2_E3��λ
				else Two_Menu_F2_E3[1]++;
				break;
			case TWO_MENU_F2_E3_D3:
				if (Two_Menu_F2_E3[2] == 9) Two_Menu_F2_E3[2] = 0;	//����F2_E3ʮλ
				else Two_Menu_F2_E3[2]++;
				break;
			case TWO_MENU_F2_E3_D4:
				if (Two_Menu_F2_E3[3] == 9) Two_Menu_F2_E3[3] = 0;	//����F2_E3��λ
				else Two_Menu_F2_E3[3]++;
				break;
			case TWO_MENU_F2_E4_D1:
				if (Two_Menu_F2_E4[0] == 20) Two_Menu_F2_E4[0] = 0;	//����F2_E4ǧλ
				else Two_Menu_F2_E4[0]++;
				break;
			case TWO_MENU_F2_E4_D2:
				if (Two_Menu_F2_E4[1] == 9) Two_Menu_F2_E4[1] = 0;	//����F2_E4��λ
				else Two_Menu_F2_E4[1]++;
				break;
			case TWO_MENU_F2_E4_D3:
				if (Two_Menu_F2_E4[2] == 9) Two_Menu_F2_E4[2] = 0;	//����F2_E4ʮλ
				else Two_Menu_F2_E4[2]++;
				break;
			case TWO_MENU_F2_E4_D4:
				if (Two_Menu_F2_E4[3] == 9) Two_Menu_F2_E4[3] = 0;	//����F2_E4��λ
				else Two_Menu_F2_E4[3]++;
			case TWO_MENU_F3_E1_SET:
				if (Two_Menu_F3_E1 == 1) Two_Menu_F3_E1 = 2;	//�Ŷ���ʾ ���� ѭ����ʾ
				else Two_Menu_F3_E1 = 1;
				break;
			case TWO_MENU_F3_E2_SET:
				if (Two_Menu_F3_E2 == 20) Two_Menu_F3_E2 = 1;	//�洢���и���
				else Two_Menu_F3_E2 ++;
				break;
			case TWO_MENU_F4_SET:
				if (Two_Menu_F4_E1 == 99) Two_Menu_F4_E1 = 0;	//����ʱ������
				else Two_Menu_F4_E1++;
				break;
			case TWO_MENU_F5_SET:
				if (Two_Menu_F5_E1 == 99) Two_Menu_F5_E1 = 0;	//ѭ��ʱ������
				else Two_Menu_F5_E1++;
				break;
			case TWO_MENU_F6_E1_SET:
				if (Two_Menu_F6_E1 == 5) Two_Menu_F6_E1 = 0;	//F6_E1����ѡ��
				else Two_Menu_F6_E1++;
				break;
			case TWO_MENU_F6_E2_SET:
				if (Two_Menu_F6_E2 == 9) Two_Menu_F6_E2 = 1;	//F6_E2������������
				else Two_Menu_F6_E2++;
				break;
			case TWO_MENU_F6_E3_SET:
				if (Two_Menu_F6_E3 == 0) Two_Menu_F6_E3 = 1;	//F6_E3ѭ��ʱ���Ƿ񱨶�
				else Two_Menu_F6_E3 = 0;
				break;
			case TWO_MENU_F6_E4_SET:
				if (Two_Menu_F6_E4 == 9) Two_Menu_F6_E4 = 0;	//F6_E4������С����
				else Two_Menu_F6_E4++;
				break;
			case TWO_MENU_F6_E5_SET:
				if (Two_Menu_F6_E5 == 9) Two_Menu_F6_E5 = 0;	//F6_E5��Ļ���ȵ���
				else Two_Menu_F6_E5++;
				break;
			case TWO_MENU_F7_E1_SET:
				if (Two_Menu_F7_E1 == 11) Two_Menu_F7_E1 = 0;	//F7_E1ԭʼ���� 999*9
				else Two_Menu_F7_E1++;
				break;
			case TWO_MENU_F7_E2_SET:
				if (Two_Menu_F7_E2 == 11) Two_Menu_F7_E2 = 0;	//F7_E2���ӹ��� 9999*9
				else Two_Menu_F7_E2++;
				break;
			case TWO_MENU_F7_E3_SET:
				if (Two_Menu_F7_E3 == 11) Two_Menu_F7_E3 = 0;	//F7_E3���ӹ��� 999*99
				else Two_Menu_F7_E3++;
				break;
			case TWO_MENU_F7_E4_SET:
				if (Two_Menu_F7_E4 == 11) Two_Menu_F7_E4 = 0;	//F7_E3���ӹ��� 9999*99
				else Two_Menu_F7_E4++;
				break;
			case TWO_MENU_F8_E1_SET:
				if (Two_Menu_F8_E1 == 1) Two_Menu_F8_E1 = 0;	//F8_E1����λ�����λ�л�
				else Two_Menu_F8_E1 = 1 ;
				break;
			case TWO_MENU_F8_E2_SET:
				if (Two_Menu_F8_E2 == 50) Two_Menu_F8_E2 = 0;	//F8_E2��λ����
				else Two_Menu_F8_E2++;
				break;

				default:break;
			}	
			func_index = table[func_index].up; break;
		case KEY_DOWN:
			switch (func_index)
			{
			case TWO_MENU_F0_YEAR:
				if (time_buf1[1] == 0) time_buf1[1] = 99;	//���������� ��
				else time_buf1[1]--;
				break;
			case TWO_MENU_F0_MOUTH:
				if (time_buf1[2] == 1) time_buf1[2] = 12;	//���������� ��
				else time_buf1[2]--;
				break;
			case TWO_MENU_F0_DAY:
				if (time_buf1[3] == 1) time_buf1[3] = 31;	//���������� ��
				else time_buf1[3]--;
				break;
			case TWO_MENU_F0_WEEK:
				if (time_buf1[7] == 1) time_buf1[7] = 7;	//���������� ����
				else time_buf1[7]--;
				break;
			case TWO_MENU_F0_HOUR:
				if (time_buf1[4] == 0) time_buf1[4] = 23;	//���������� Сʱ
				else time_buf1[4]--;
				break;
			case TWO_MENU_F0_MINUTE:
				if (time_buf1[5] == 0) time_buf1[5] = 59;	//���������� ����
				else time_buf1[5]--;
				break;
			case TWO_MENU_F1_E1_D1:
				if (Two_Menu_F1_E1[0] == 0) Two_Menu_F1_E1[0] = 20;	//����F1_E1ǧλ
				else Two_Menu_F1_E1[0]--;
				break;
			case TWO_MENU_F1_E1_D2:
				if (Two_Menu_F1_E1[1] == 0) Two_Menu_F1_E1[1] = 9;	//����F1_E1��λ
				else Two_Menu_F1_E1[1]--;
				break;
			case TWO_MENU_F1_E1_D3:
				if (Two_Menu_F1_E1[2] == 0) Two_Menu_F1_E1[2] = 9;	//����F1_E1ʮλ
				else Two_Menu_F1_E1[2]--;
				break;
			case TWO_MENU_F1_E1_D4:
				if (Two_Menu_F1_E1[3] == 0) Two_Menu_F1_E1[3] = 9;	//����F1_E1��λ
				else Two_Menu_F1_E1[3]--;
				break;
			case TWO_MENU_F1_E2_D1:
				if (Two_Menu_F1_E2[0] == 0) Two_Menu_F1_E2[0] = 20;	//����F1_E2ǧλ
				else Two_Menu_F1_E2[0]--;
				break;
			case TWO_MENU_F1_E2_D2:
				if (Two_Menu_F1_E2[1] == 0) Two_Menu_F1_E2[1] = 9;	//����F1_E2��λ
				else Two_Menu_F1_E2[1]--;
				break;
			case TWO_MENU_F1_E2_D3:
				if (Two_Menu_F1_E2[2] == 0) Two_Menu_F1_E2[2] = 9;	//����F1_E2ʮλ
				else Two_Menu_F1_E2[2]--;
				break;
			case TWO_MENU_F1_E2_D4:
				if (Two_Menu_F1_E2[3] == 0) Two_Menu_F1_E2[3] = 9;	//����F1_E2��λ
				else Two_Menu_F1_E2[3]--;
				break;
			case TWO_MENU_F1_E3_D1:
				if (Two_Menu_F1_E3[0] == 0) Two_Menu_F1_E3[0] = 20;	//����F1_E3ǧλ
				else Two_Menu_F1_E3[0]--;
				break;
			case TWO_MENU_F1_E3_D2:
				if (Two_Menu_F1_E3[1] == 0) Two_Menu_F1_E3[1] = 9;	//����F1_E3��λ
				else Two_Menu_F1_E3[1]--;
				break;
			case TWO_MENU_F1_E3_D3:
				if (Two_Menu_F1_E3[2] == 0) Two_Menu_F1_E3[2] = 9;	//����F1_E3ʮλ
				else Two_Menu_F1_E3[2]--;
				break;
			case TWO_MENU_F1_E3_D4:
				if (Two_Menu_F1_E3[3] == 0) Two_Menu_F1_E3[3] = 9;	//����F1_E3��λ
				else Two_Menu_F1_E3[3]--;
				break;
			case TWO_MENU_F1_E4_D1:
				if (Two_Menu_F1_E4[0] == 0) Two_Menu_F1_E4[0] = 20;	//����F1_E4ǧλ
				else Two_Menu_F1_E4[0]--;
				break;
			case TWO_MENU_F1_E4_D2:
				if (Two_Menu_F1_E4[1] == 0) Two_Menu_F1_E4[1] = 9;	//����F1_E4��λ
				else Two_Menu_F1_E4[1]--;
				break;
			case TWO_MENU_F1_E4_D3:
				if (Two_Menu_F1_E4[2] == 0) Two_Menu_F1_E4[2] = 9;	//����F1_E4ʮλ
				else Two_Menu_F1_E4[2]--;
				break;
			case TWO_MENU_F1_E4_D4:
				if (Two_Menu_F1_E4[3] == 0) Two_Menu_F1_E4[3] = 9;	//����F1_E4��λ
				else Two_Menu_F1_E4[3]--;
				break;
			case TWO_MENU_F2_E1_D1:
				if (Two_Menu_F2_E1[0] == 0) Two_Menu_F2_E1[0] = 20;	//����F2_E1ǧλ
				else Two_Menu_F2_E1[0]--;
				break;
			case TWO_MENU_F2_E1_D2:
				if (Two_Menu_F2_E1[1] == 0) Two_Menu_F2_E1[1] = 9;	//����F2_E1��λ
				else Two_Menu_F2_E1[1]--;
				break;
			case TWO_MENU_F2_E1_D3:
				if (Two_Menu_F2_E1[2] == 0) Two_Menu_F2_E1[2] = 9;	//����F2_E1ʮλ
				else Two_Menu_F2_E1[2]--;
				break;
			case TWO_MENU_F2_E1_D4:
				if (Two_Menu_F2_E1[3] == 0) Two_Menu_F2_E1[3] = 9;	//����F2_E1��λ
				else Two_Menu_F2_E1[3]--;
				break;
			case TWO_MENU_F2_E2_D1:
				if (Two_Menu_F2_E2[0] == 0) Two_Menu_F2_E2[0] = 20;	//����F2_E2ǧλ
				else Two_Menu_F2_E2[0]--;
				break;
			case TWO_MENU_F2_E2_D2:
				if (Two_Menu_F2_E2[1] == 0) Two_Menu_F2_E2[1] = 9;	//����F2_E2��λ
				else Two_Menu_F2_E2[1]--;
				break;
			case TWO_MENU_F2_E2_D3:
				if (Two_Menu_F2_E2[2] == 0) Two_Menu_F2_E2[2] = 9;	//����F2_E2ʮλ
				else Two_Menu_F2_E2[2]--;
				break;
			case TWO_MENU_F2_E2_D4:
				if (Two_Menu_F2_E2[3] == 0) Two_Menu_F2_E2[3] = 9;	//����F2_E2��λ
				else Two_Menu_F2_E2[3]--;
				break;
			case TWO_MENU_F2_E3_D1:
				if (Two_Menu_F2_E3[0] == 0) Two_Menu_F2_E3[0] = 20;	//����F2_E3ǧλ
				else Two_Menu_F2_E3[0]--;
				break;
			case TWO_MENU_F2_E3_D2:
				if (Two_Menu_F2_E3[1] == 0) Two_Menu_F2_E3[1] = 9;	//����F2_E3��λ
				else Two_Menu_F2_E3[1]--;
				break;
			case TWO_MENU_F2_E3_D3:
				if (Two_Menu_F2_E3[2] == 0) Two_Menu_F2_E3[2] = 9;	//����F2_E3ʮλ
				else Two_Menu_F2_E3[2]--;
				break;
			case TWO_MENU_F2_E3_D4:
				if (Two_Menu_F2_E3[3] == 0) Two_Menu_F2_E3[3] = 9;	//����F2_E3��λ
				else Two_Menu_F2_E3[3]--;
				break;
			case TWO_MENU_F2_E4_D1:
				if (Two_Menu_F2_E4[0] == 0) Two_Menu_F2_E4[0] = 20;	//����F2_E4ǧλ
				else Two_Menu_F2_E4[0]--;
				break;
			case TWO_MENU_F2_E4_D2:
				if (Two_Menu_F2_E4[1] == 0) Two_Menu_F2_E4[1] = 9;	//����F2_E4��λ
				else Two_Menu_F2_E4[1]--;
				break;
			case TWO_MENU_F2_E4_D3:
				if (Two_Menu_F2_E4[2] == 0) Two_Menu_F2_E4[2] = 9;	//����F2_E4ʮλ
				else Two_Menu_F2_E4[2]--;
				break;
			case TWO_MENU_F2_E4_D4:
				if (Two_Menu_F2_E4[3] == 0) Two_Menu_F2_E4[3] = 9;	//����F2_E4��λ
				else Two_Menu_F2_E4[3]--;
				break;
			case TWO_MENU_F3_E1_SET:
				if (Two_Menu_F3_E1 == 1) Two_Menu_F3_E1 = 2;	//�Ŷ���ʾ ���� ѭ����ʾ
				else Two_Menu_F3_E1 = 1;
				break;
			case TWO_MENU_F3_E2_SET:
				if (Two_Menu_F3_E2 == 1) Two_Menu_F3_E2 = 20;	//�洢���и���
				else Two_Menu_F3_E2--;
				break;
			case TWO_MENU_F4_SET:
				if (Two_Menu_F4_E1 == 0) Two_Menu_F4_E1 = 99;	//����ʱ������
				else Two_Menu_F4_E1--;
				break;
			case TWO_MENU_F5_SET:
				if (Two_Menu_F5_E1 == 0) Two_Menu_F5_E1 = 99;	//����ʱ������
				else Two_Menu_F5_E1--;
				break;
			case TWO_MENU_F6_E1_SET:
				if (Two_Menu_F6_E1 == 0) Two_Menu_F6_E1 = 5;	//F6_E1������ѡ��
				else Two_Menu_F6_E1--;
				break;
			case TWO_MENU_F6_E2_SET:
				if (Two_Menu_F6_E2 == 1) Two_Menu_F6_E2 = 9;	//F6_E2����������������
				else Two_Menu_F6_E2--;
				break;
			case TWO_MENU_F6_E3_SET:
				if (Two_Menu_F6_E3 == 0) Two_Menu_F6_E3 = 1;	//F6_E3ѭ��ʱ���Ƿ񱨶�
				else Two_Menu_F6_E3 = 0;
				break;
			case TWO_MENU_F6_E4_SET:
				if (Two_Menu_F6_E4 == 0) Two_Menu_F6_E4 = 9;	//F6_E4������С����
				else Two_Menu_F6_E4--;
				break;
			case TWO_MENU_F6_E5_SET:
				if (Two_Menu_F6_E5 == 0) Two_Menu_F6_E5 = 9;	//F6_E5��Ļ���ȵ���
				else Two_Menu_F6_E5--;
				break;
			case TWO_MENU_F7_E1_SET:
				if (Two_Menu_F7_E1 == 0) Two_Menu_F7_E1 = 11;	//F7_E1ԭʼ���� 999*9
				else Two_Menu_F7_E1--;
				break;
			case TWO_MENU_F7_E2_SET:
				if (Two_Menu_F7_E2 == 0) Two_Menu_F7_E2 = 11;	//F7_E2���ӹ��� 9999*9
				else Two_Menu_F7_E2--;
				break;
			case TWO_MENU_F7_E3_SET:
				if (Two_Menu_F7_E3 == 0) Two_Menu_F7_E3 = 11;	//F7_E3���ӹ��� 999*99
				else Two_Menu_F7_E3--;
				break;
			case TWO_MENU_F7_E4_SET:
				if (Two_Menu_F7_E4 == 0) Two_Menu_F7_E4 = 11;	//F7_E3���ӹ��� 9999*99
				else Two_Menu_F7_E4--;
				break;
			case TWO_MENU_F8_E1_SET:
				if (Two_Menu_F8_E1 == 1) Two_Menu_F8_E1 = 0;	//F8_E1����λ�����λ�л�
				else Two_Menu_F8_E1 = 1;
				break;
			case TWO_MENU_F8_E2_SET:
				if (Two_Menu_F8_E2 == 0) Two_Menu_F8_E2 = 50;	//F8_E2��λ����
				else Two_Menu_F8_E2--;
				break;

				default:break;
			}
			func_index = table[func_index].down; break;
		default:break;
	}
	current_operation_index = table[func_index].index_operation;
	(*current_operation_index)();//ִ�е�ǰ��������
}

void delay10ms(void)   //��� -0.054253472222us
{
	unsigned char a, b, c;
	for (c = 4; c>0; c--)
	for (b = 52; b>0; b--)
	for (a = 220; a>0; a--);
	_nop_();  //if Keil,require use intrins.h
}

unsigned char return_func_index(void)
{
	unsigned char func_index_temp = 0;
	func_index_temp = func_index;
	return func_index_temp;
}

void set_func_index(unsigned char temp)
{
	func_index = temp;
}

unsigned char return_Two_Menu_F3_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F3_E1;
	return temp;
}

unsigned char return_Two_Menu_F3_E2(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F3_E2;
	return temp;
}

unsigned char return_Two_Menu_F4_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F4_E1;
	return temp;
}

unsigned char return_Two_Menu_F5_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F5_E1;
	return temp;
}

unsigned char return_Two_Menu_F6_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E1;
	return temp;
}

unsigned char return_Two_Menu_F6_E2(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E2;
	return temp;
}

unsigned char return_Two_Menu_F6_E3(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E3;
	return temp;
}

unsigned char return_Two_Menu_F6_E4(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E4;
	return temp;
}

unsigned char return_Two_Menu_F6_E5(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F6_E5;
	return temp;
}

unsigned char return_Two_Menu_F7_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F7_E1;
	return temp;
}

unsigned char return_Two_Menu_F7_E2(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F7_E2;
	return temp;
}

unsigned char return_Two_Menu_F7_E3(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F7_E3;
	return temp;
}

unsigned char return_Two_Menu_F7_E4(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F7_E4;
	return temp;
}

unsigned char return_Two_Menu_F8_E1(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F8_E1;
	return temp;
}

unsigned char return_Two_Menu_F8_E2(void)
{
	unsigned char temp = 0;
	temp = Two_Menu_F8_E2;
	return temp;
}