#ifndef __MENU_H_
#define __MENU_H_

//以下define为菜单数字定义为英文字母，方便识别

#define MENU_STANDBY 0   //待机
#define ONE_MENU_F0  1   //一级子菜单F0
#define ONE_MENU_F1  2
#define ONE_MENU_F2  3
#define ONE_MENU_F3  4
#define ONE_MENU_F4  5
#define ONE_MENU_F5  6
#define ONE_MENU_F6  7
#define ONE_MENU_F7  8
#define ONE_MENU_F8  9
#define ONE_MENU_F9  10
#define ONE_MENU_FA  11  //一级子菜单FA
#define ONE_MENU_Fb  12  //一级子菜单Fb
#define ONE_MENU_FC  13  //一级子菜单FC

//万年历菜单定义
#define TWO_MENU_F0_YEAR    14  //设置年
#define TWO_MENU_F0_MOUTH   15	//设置月
#define TWO_MENU_F0_DAY     16	//设置日
#define TWO_MENU_F0_WEEK    17	//设置星期
#define TWO_MENU_F0_HOUR    18	//设置小时
#define TWO_MENU_F0_MINUTE  19	//设置分钟

//F1二级菜单
#define TWO_MENU_F1_E1      20  //E1注册呼叫器
#define TWO_MENU_F1_E2      21  //E2注册管理器
#define TWO_MENU_F1_E3      22  //E3注册报警器
#define TWO_MENU_F1_E4      23  //E4注册取消器

//F2二级菜单
#define TWO_MENU_F2_E1      24  //E1删除呼叫器
#define TWO_MENU_F2_E2      25  //E2删除管理器
#define TWO_MENU_F2_E3      26  //E3删除报警器
#define TWO_MENU_F2_E4      27  //E4删除取消器

//F3二级菜单
#define TWO_MENU_F3_E1      28  //E1删除呼叫器
#define TWO_MENU_F3_E2      29  //E2删除管理器

//F4 F5二级菜单
#define TWO_MENU_F4_SET     30  //F4功能设置
#define TWO_MENU_F5_SET     31  //F5功能设置

//F6二级子菜单
#define TWO_MENU_F6_E1      32  //E1语音选择
#define TWO_MENU_F6_E2      33  //E2语音报读次数
#define TWO_MENU_F6_E3      34  //循环时是否报读
#define TWO_MENU_F6_E4		35  //音量大小调整
#define TWO_MENU_F6_E5      36  //亮度调整

//F7 F8二级菜单
#define TWO_MENU_F7_SET     37  //F5功能设置
#define TWO_MENU_F8_SET     38  //F5功能设置

//F9二级菜单
#define TWO_MENU_F9_E1      39  //E1恢复功能和号码全部出厂
#define TWO_MENU_F9_E2      40  //E2恢复功能出厂

//FA二级菜单
#define TWO_MENU_FA_E1      41  //E1速率测试
#define TWO_MENU_FA_E2      42  //E2键位调整

//Fb FC二级菜单
#define TWO_MENU_Fb_SET     43  //F5功能设置
#define TWO_MENU_FC_SET     44  //F5功能设置


#endif