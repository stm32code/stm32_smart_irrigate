#ifndef __GIT_H
#define __GIT_H
#include "stdio.h"

typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned int U32;
typedef signed int S32;
typedef float F32;

// 服务器信息
#define SSID "CMCC-KPUD"				 // 路由器SSID名称
#define PASS "p675eph9"			 // 路由器密码
#define ServerIP "183.230.40.39" // 服务器IP地址
#define ServerPort 6002			 // 服务器IP地址端口号
// 设备信息
#define PROID "634589"	   // 产品ID
#define DEVID "1215495965" // 设备ID
#define AUTH_INFO "7"	   // 鉴权信息
// MQTT主题
#define S_TOPIC_NAME "app_topic" // 需要订阅的主题
#define P_TOPIC_NAME "div_topic" // 需要发布的主题

//  设备使用外设定义
#define OLED 1 // 是否使用OLED

// 自定义执行结果类型
typedef enum
{
	MY_SUCCESSFUL = 0x01, // 成功
	MY_FAIL = 0x00		  // 失败

} mySta; // 成功标志位

typedef enum
{
	OPEN = 0x01, // 打开
	CLOSE = 0x00 // 关闭

} On_or_Off_TypeDef; // 成功标志位

typedef enum
{
	DERVICE_SEND = 0x00, // 设备->平台
	PLATFORM_SEND = 0x01 // 平台->设备

} Send_directino; // 发送方向	

typedef struct
{
	U8 App;	  //
	U8 flage; //

	U16 light;		 // 光照
	U16 somg;		 // 烟雾浓度
	F32 temperatuer; // 温度
	F32 humiditr;	 // 湿度
	U16 humi_soil1;	 // 湿度1
	U16 humi_soil2;	 // 湿度2

} Data_TypeDef; // 数据参数结构体

typedef struct
{

	U8 threshold_temp;			 // 室内温度
	U8 threshold_humi;			 // 室内湿度
 

} Threshold_Value_TypeDef; // 数据参数结构体

typedef struct
{

	U8 led_state;
	U8 hot_led_state;
	U8 fan_state;

} Device_Satte_Typedef; // 状态参数结构体

// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data);
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state);
// 更新OLED显示屏中内容
 

#endif
