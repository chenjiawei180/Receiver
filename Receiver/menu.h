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
#define ONE_MENU_Fd  14  //一级子菜单Fd

//万年历菜单定义
#define TWO_MENU_F0_YEAR    15  //设置年
#define TWO_MENU_F0_MOUTH   16	//设置月
#define TWO_MENU_F0_DAY     17	//设置日
#define TWO_MENU_F0_WEEK    18	//设置星期
#define TWO_MENU_F0_HOUR    19	//设置小时
#define TWO_MENU_F0_MINUTE  20	//设置分钟

//F1二级菜单
#define TWO_MENU_F1_E1      21  //E1注册呼叫器
#define TWO_MENU_F1_E2      22  //E2注册管理器
#define TWO_MENU_F1_E3      23  //E3注册报警器
#define TWO_MENU_F1_E4      24  //E4注册取消器

//F2二级菜单
#define TWO_MENU_F2_E1      25  //E1删除呼叫器
#define TWO_MENU_F2_E2      26  //E2删除管理器
#define TWO_MENU_F2_E3      27  //E3删除报警器
#define TWO_MENU_F2_E4      28  //E4删除取消器

//F3二级菜单
#define TWO_MENU_F3_E1      29  //E1删除呼叫器
#define TWO_MENU_F3_E2      30  //E2删除管理器

//F4 F5二级菜单
#define TWO_MENU_F4_SET     31  //F4功能设置
#define TWO_MENU_F5_SET     32  //F5功能设置

//F6二级子菜单
#define TWO_MENU_F6_E1      33  //E1语音选择
#define TWO_MENU_F6_E2      34  //E2语音报读次数
#define TWO_MENU_F6_E3      35  //循环时是否报读
#define TWO_MENU_F6_E4		36  //音量大小调整
#define TWO_MENU_F6_E5      37  //亮度调整

//F7二级子菜单
#define TWO_MENU_F7_E1      38  //E1 原始规则
#define TWO_MENU_F7_E2      39  //E2 原始规则
#define TWO_MENU_F7_E3      40  //E3 原始规则
#define TWO_MENU_F7_E4      41  //E4 原始规则

//F8二级子菜单
#define TWO_MENU_F8_E1      42   //E1 单键多键切换
#define TWO_MENU_F8_E2      43   //E2键位设置

//F9二级菜单
#define TWO_MENU_F9_E1      44  //E1恢复功能和号码全部出厂
#define TWO_MENU_F9_E2      45  //E2恢复功能出厂

//FA Fb FC二级菜单
#define TWO_MENU_FA_SET     46  //FA功能设置
#define TWO_MENU_Fb_SET     47  //FB功能设置
#define TWO_MENU_FC_SET     48  //FC功能设置
#define TWO_MENU_Fd_SET     49  //Fd功能设置

//F1_E1子菜单设置
#define TWO_MENU_F1_E1_D1   50  //注册呼叫器千位
#define TWO_MENU_F1_E1_D2   51  //注册呼叫器百位
#define TWO_MENU_F1_E1_D3   52  //注册呼叫器十位
#define TWO_MENU_F1_E1_D4   53  //注册呼叫器个位

//F1_E2子菜单设置
#define TWO_MENU_F1_E2_D1   54  //注册管理器千位
#define TWO_MENU_F1_E2_D2   55  //注册管理器百位
#define TWO_MENU_F1_E2_D3   56  //注册管理器十位
#define TWO_MENU_F1_E2_D4   57  //注册管理器个位

//F1_E3子菜单设置
#define TWO_MENU_F1_E3_D1   58  //注册报警器千位
#define TWO_MENU_F1_E3_D2   59  //注册报警器百位
#define TWO_MENU_F1_E3_D3   60  //注册报警器十位
#define TWO_MENU_F1_E3_D4   61  //注册报警器个位

//F1_E4子菜单设置
#define TWO_MENU_F1_E4_D1   62  //注册取消器千位
#define TWO_MENU_F1_E4_D2   63  //注册取消器百位
#define TWO_MENU_F1_E4_D3   64  //注册取消器十位
#define TWO_MENU_F1_E4_D4   65  //注册取消器个位

//F2_E1子菜单设置
#define TWO_MENU_F2_E1_D1   66  //删除呼叫器千位
#define TWO_MENU_F2_E1_D2   67  //删除呼叫器百位
#define TWO_MENU_F2_E1_D3   68  //删除呼叫器十位
#define TWO_MENU_F2_E1_D4   69  //删除呼叫器个位

//F2_E2子菜单设置
#define TWO_MENU_F2_E2_D1   70  //删除管理器千位
#define TWO_MENU_F2_E2_D2   71  //删除管理器百位
#define TWO_MENU_F2_E2_D3   72  //删除管理器十位
#define TWO_MENU_F2_E2_D4   73  //删除管理器个位

//F2_E3子菜单设置
#define TWO_MENU_F2_E3_D1   74  //删除报警器千位
#define TWO_MENU_F2_E3_D2   75  //删除报警器百位
#define TWO_MENU_F2_E3_D3   76  //删除报警器十位
#define TWO_MENU_F2_E3_D4   77  //删除报警器个位

//F2_E4子菜单设置
#define TWO_MENU_F2_E4_D1   78  //删除取消器千位
#define TWO_MENU_F2_E4_D2   79  //删除取消器百位
#define TWO_MENU_F2_E4_D3   80  //删除取消器十位
#define TWO_MENU_F2_E4_D4   81  //删除取消器个位

//F3子菜单设置
#define TWO_MENU_F3_E1_SET  82 //F3_E1设置选择
#define TWO_MENU_F3_E2_SET  83 //F3_E1设置选择

//F6子菜单设置
#define TWO_MENU_F6_E1_SET  84 //F6_E1语音选择
#define TWO_MENU_F6_E2_SET  85 //F6_E2语音报读次数
#define TWO_MENU_F6_E3_SET  86 //F6_E3循环是否报读
#define TWO_MENU_F6_E4_SET  87 //F6_E4音量大小调整
#define TWO_MENU_F6_E5_SET  88 //F6_E5显示屏亮度调整

//F7子菜单设置
#define TWO_MENU_F7_E1_SET  89 //F7_E1 默认规则999*9
#define TWO_MENU_F7_E2_SET  90 //F7_E2 附加规则9999*9
#define TWO_MENU_F7_E3_SET  91 //F7_E3 附加规则999*99
#define TWO_MENU_F7_E4_SET  92 //F7_E4 附加规则9999*99

//F8子菜单设置
#define TWO_MENU_F8_E1_SET  93 //F8_E1 单键盘还是多键盘
#define TWO_MENU_F8_E2_SET  94 //F8_E2 键位更改设置

//解码菜单
#define DECODER_MENU        95 //解码显示菜单



//以下为声音定义
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