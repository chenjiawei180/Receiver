#ifndef __MENU_H_
#define __MENU_H_

//����defineΪ�˵����ֶ���ΪӢ����ĸ������ʶ��

#define MENU_STANDBY 0   //����
#define ONE_MENU_F0  1   //һ���Ӳ˵�F0
#define ONE_MENU_F1  2
#define ONE_MENU_F2  3
#define ONE_MENU_F3  4
#define ONE_MENU_F4  5
#define ONE_MENU_F5  6
#define ONE_MENU_F6  7
#define ONE_MENU_F7  8
#define ONE_MENU_F8  9
#define ONE_MENU_F9  10
#define ONE_MENU_FA  11  //һ���Ӳ˵�FA
#define ONE_MENU_Fb  12  //һ���Ӳ˵�Fb
#define ONE_MENU_FC  13  //һ���Ӳ˵�FC
#define ONE_MENU_Fd  14  //һ���Ӳ˵�Fd

//�������˵�����
#define TWO_MENU_F0_YEAR    15  //������
#define TWO_MENU_F0_MOUTH   16	//������
#define TWO_MENU_F0_DAY     17	//������
#define TWO_MENU_F0_WEEK    18	//��������
#define TWO_MENU_F0_HOUR    19	//����Сʱ
#define TWO_MENU_F0_MINUTE  20	//���÷���

//F1�����˵�
#define TWO_MENU_F1_E1      21  //E1ע�������
#define TWO_MENU_F1_E2      22  //E2ע�������
#define TWO_MENU_F1_E3      23  //E3ע�ᱨ����
#define TWO_MENU_F1_E4      24  //E4ע��ȡ����

//F2�����˵�
#define TWO_MENU_F2_E1      25  //E1ɾ��������
#define TWO_MENU_F2_E2      26  //E2ɾ��������
#define TWO_MENU_F2_E3      27  //E3ɾ��������
#define TWO_MENU_F2_E4      28  //E4ɾ��ȡ����

//F3�����˵�
#define TWO_MENU_F3_E1      29  //E1ɾ��������
#define TWO_MENU_F3_E2      30  //E2ɾ��������

//F4 F5�����˵�
#define TWO_MENU_F4_SET     31  //F4��������
#define TWO_MENU_F5_SET     32  //F5��������

//F6�����Ӳ˵�
#define TWO_MENU_F6_E1      33  //E1����ѡ��
#define TWO_MENU_F6_E2      34  //E2������������
#define TWO_MENU_F6_E3      35  //ѭ��ʱ�Ƿ񱨶�
#define TWO_MENU_F6_E4		36  //������С����
#define TWO_MENU_F6_E5      37  //���ȵ���

//F7�����Ӳ˵�
#define TWO_MENU_F7_E1      38  //E1 ԭʼ����
#define TWO_MENU_F7_E2      39  //E2 ԭʼ����
#define TWO_MENU_F7_E3      40  //E3 ԭʼ����
#define TWO_MENU_F7_E4      41  //E4 ԭʼ����

//F8�����Ӳ˵�
#define TWO_MENU_F8_E1      42   //E1 ��������л�
#define TWO_MENU_F8_E2      43   //E2��λ����

//F9�����˵�
#define TWO_MENU_F9_E1      44  //E1�ָ����ܺͺ���ȫ������
#define TWO_MENU_F9_E2      45  //E2�ָ����ܳ���

//FA Fb FC�����˵�
#define TWO_MENU_FA_SET     46  //FA��������
#define TWO_MENU_Fb_SET     47  //FB��������
#define TWO_MENU_FC_SET     48  //FC��������
#define TWO_MENU_Fd_SET     49  //Fd��������

//F1_E1�Ӳ˵�����
#define TWO_MENU_F1_E1_D1   50  //ע�������ǧλ
#define TWO_MENU_F1_E1_D2   51  //ע���������λ
#define TWO_MENU_F1_E1_D3   52  //ע�������ʮλ
#define TWO_MENU_F1_E1_D4   53  //ע���������λ

//F1_E2�Ӳ˵�����
#define TWO_MENU_F1_E2_D1   54  //ע�������ǧλ
#define TWO_MENU_F1_E2_D2   55  //ע���������λ
#define TWO_MENU_F1_E2_D3   56  //ע�������ʮλ
#define TWO_MENU_F1_E2_D4   57  //ע���������λ

//F1_E3�Ӳ˵�����
#define TWO_MENU_F1_E3_D1   58  //ע�ᱨ����ǧλ
#define TWO_MENU_F1_E3_D2   59  //ע�ᱨ������λ
#define TWO_MENU_F1_E3_D3   60  //ע�ᱨ����ʮλ
#define TWO_MENU_F1_E3_D4   61  //ע�ᱨ������λ

//F1_E4�Ӳ˵�����
#define TWO_MENU_F1_E4_D1   62  //ע��ȡ����ǧλ
#define TWO_MENU_F1_E4_D2   63  //ע��ȡ������λ
#define TWO_MENU_F1_E4_D3   64  //ע��ȡ����ʮλ
#define TWO_MENU_F1_E4_D4   65  //ע��ȡ������λ

//F2_E1�Ӳ˵�����
#define TWO_MENU_F2_E1_D1   66  //ɾ��������ǧλ
#define TWO_MENU_F2_E1_D2   67  //ɾ����������λ
#define TWO_MENU_F2_E1_D3   68  //ɾ��������ʮλ
#define TWO_MENU_F2_E1_D4   69  //ɾ����������λ

//F2_E2�Ӳ˵�����
#define TWO_MENU_F2_E2_D1   70  //ɾ��������ǧλ
#define TWO_MENU_F2_E2_D2   71  //ɾ����������λ
#define TWO_MENU_F2_E2_D3   72  //ɾ��������ʮλ
#define TWO_MENU_F2_E2_D4   73  //ɾ����������λ

//F2_E3�Ӳ˵�����
#define TWO_MENU_F2_E3_D1   74  //ɾ��������ǧλ
#define TWO_MENU_F2_E3_D2   75  //ɾ����������λ
#define TWO_MENU_F2_E3_D3   76  //ɾ��������ʮλ
#define TWO_MENU_F2_E3_D4   77  //ɾ����������λ

//F2_E4�Ӳ˵�����
#define TWO_MENU_F2_E4_D1   78  //ɾ��ȡ����ǧλ
#define TWO_MENU_F2_E4_D2   79  //ɾ��ȡ������λ
#define TWO_MENU_F2_E4_D3   80  //ɾ��ȡ����ʮλ
#define TWO_MENU_F2_E4_D4   81  //ɾ��ȡ������λ

//F3_E1�Ӳ˵�����
#define TWO_MENU_F3_E1_SET  82 //F3_E1����ѡ��
#define TWO_MENU_F3_E2_SET  83 //F3_E1����ѡ��


#endif