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

//F3�Ӳ˵�����
#define TWO_MENU_F3_E1_SET  82 //F3_E1����ѡ��
#define TWO_MENU_F3_E2_SET  83 //F3_E1����ѡ��

//F6�Ӳ˵�����
#define TWO_MENU_F6_E1_SET  84 //F6_E1����ѡ��
#define TWO_MENU_F6_E2_SET  85 //F6_E2������������
#define TWO_MENU_F6_E3_SET  86 //F6_E3ѭ���Ƿ񱨶�
#define TWO_MENU_F6_E4_SET  87 //F6_E4������С����
#define TWO_MENU_F6_E5_SET  88 //F6_E5��ʾ�����ȵ���

//F7�Ӳ˵�����
#define TWO_MENU_F7_E1_SET  89 //F7_E1 Ĭ�Ϲ���999*9
#define TWO_MENU_F7_E2_SET  90 //F7_E2 ���ӹ���9999*9
#define TWO_MENU_F7_E3_SET  91 //F7_E3 ���ӹ���999*99
#define TWO_MENU_F7_E4_SET  92 //F7_E4 ���ӹ���9999*99

//F8�Ӳ˵�����
#define TWO_MENU_F8_E1_SET  93 //F8_E1 �����̻��Ƕ����
#define TWO_MENU_F8_E2_SET  94 //F8_E2 ��λ��������

//����˵�
#define DECODER_MENU        95 //������ʾ�˵�



//����Ϊ��������
#define ZERO      0
#define ONE       1
#define TWO       2
#define THREE     3
#define FOUR      4
#define FIVE      5
#define SIX       6
#define SEVEN     7
#define EIGHT     8
#define NINE      9
#define TEN       10
#define ELEVEN    11
#define TWELVE    12
#define THIRTEEN  13
#define FOURTEEN  14
#define FIFTEEN   15
#define SIXTEEN   16
#define SEVENTEEN 17
#define EIGHTEEN  18
#define NINETEEN  19
#define TWENTY    20
#define THIRTY    21
#define FORTY     22
#define FIFTY     23
#define SIXTY     24
#define SEVENTY   25
#define EIGHTY    26
#define NINETTY   27
#define HUNDRED   28
#define AND       29
#define THOUSAND  30

#define ZIMU_A   32
#define ZIMU_B   33
#define ZIMU_C   34
#define ZIMU_D   35
#define ZIMU_E   36
#define ZIMU_F   37
#define ZIMU_G   38
#define ZIMU_H   39
#define ZIMU_I   40
#define ZIMU_J   41
#define ZIMU_K   42
#define ZIMU_L   43
#define ZIMU_M   44
#define ZIMU_N   45
#define ZIMU_O   46
#define ZIMU_P   47
#define ZIMU_Q   48
#define ZIMU_R   49
#define ZIMU_S   50
#define ZIMU_T   51
#define ZIMU_U   52
#define ZIMU_V   53
#define ZIMU_W   54
#define ZIMU_X   55
#define ZIMU_Y   56
#define ZIMU_Z   57

#define BAOJING   58
#define YINGYUEYI 59
#define YINGYUEER 60
#define DINGDONG  61
#define QING      62
#define DAO       63
#define GUITAI    64
#define QUXIAO    65
#define HUJIAO    66
#define JIUSHUI   67
#define JIEZHANG  68
#define MAIDANG   69
#define DIANDANG  70
#define JIASHUI   71
#define TIANDIAN  72
#define SHIYING   73
#define FUWU      74
#define PIJIU     75
#define JIU       76
#define CUFANG    77
#define CUSHI     78
#define BATAI     79
#define BANGZHU   80
#define YISHENG   81
#define HUSHI     82
#define ANMO      83
#define ZHILIAO   84
#define QIANTAI   85
#define BAOJIE    86
#define BANGONG   87
#define BAZHEN    88
#define HUANYAO   89
#define SHUYE     90
#define JINJI     91
#define JIASHI    92
#define QINGJIE   93
#define GUANLI    94
#define WEIXIU    95
#define GUZHANG   96
#define ZHUXI     97
#define ZHUREN    98
#define LICAI     99
#define SHOUQUAN  100
#define ANQUAN    101
#define BAOAN     102
#define HAO       103
#define FANG      104
#define CHUANG    105
#define ZHUO      106
#define LOU       107
#define DENGHOU   108

#define DI        126
#define CHENGGONG 127
#define SHIBAI    128
#define CHUSHIHUA 129



#endif