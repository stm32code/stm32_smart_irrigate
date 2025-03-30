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

// ��������Ϣ
#define SSID "CMCC-KPUD"				 // ·����SSID����
#define PASS "p675eph9"			 // ·��������
#define ServerIP "183.230.40.39" // ������IP��ַ
#define ServerPort 6002			 // ������IP��ַ�˿ں�
// �豸��Ϣ
#define PROID "634589"	   // ��ƷID
#define DEVID "1215495965" // �豸ID
#define AUTH_INFO "7"	   // ��Ȩ��Ϣ
// MQTT����
#define S_TOPIC_NAME "app_topic" // ��Ҫ���ĵ�����
#define P_TOPIC_NAME "div_topic" // ��Ҫ����������

//  �豸ʹ�����趨��
#define OLED 1 // �Ƿ�ʹ��OLED

// �Զ���ִ�н������
typedef enum
{
	MY_SUCCESSFUL = 0x01, // �ɹ�
	MY_FAIL = 0x00		  // ʧ��

} mySta; // �ɹ���־λ

typedef enum
{
	OPEN = 0x01, // ��
	CLOSE = 0x00 // �ر�

} On_or_Off_TypeDef; // �ɹ���־λ

typedef enum
{
	DERVICE_SEND = 0x00, // �豸->ƽ̨
	PLATFORM_SEND = 0x01 // ƽ̨->�豸

} Send_directino; // ���ͷ���	

typedef struct
{
	U8 App;	  //
	U8 flage; //

	U16 light;		 // ����
	U16 somg;		 // ����Ũ��
	F32 temperatuer; // �¶�
	F32 humiditr;	 // ʪ��
	U16 humi_soil1;	 // ʪ��1
	U16 humi_soil2;	 // ʪ��2

} Data_TypeDef; // ���ݲ����ṹ��

typedef struct
{

	U8 threshold_temp;			 // �����¶�
	U8 threshold_humi;			 // ����ʪ��
 

} Threshold_Value_TypeDef; // ���ݲ����ṹ��

typedef struct
{

	U8 led_state;
	U8 hot_led_state;
	U8 fan_state;

} Device_Satte_Typedef; // ״̬�����ṹ��

// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data);
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state);
// ����OLED��ʾ��������
 

#endif
