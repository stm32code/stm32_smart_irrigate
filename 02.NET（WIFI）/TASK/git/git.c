#include "git.h"
#if OLED // OLED�ļ�����
#include "oled.h"
#endif
//  C��
#include "onenet.h"
#include "cjson.h"
#include <string.h>
#include <stdio.h>
#include "bh1750.h"
#include "ds18b20.h"
#include "oled.h"
#include "relay.h"
#include "dht11.h"
#include "adc.h"
// ����豸�����ļ�
DHT11_Data_TypeDef DHT11_Data;				  // ��ʪ�Ƚṹ��
Data_TypeDef Data_init;						  // �豸���ݽṹ��
Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
Device_Satte_Typedef device_state_init;		  // �豸״̬
extern u8 ucDs18b20Id[8];

// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data)
{
	Read_DHT11(&DHT11_Data); // ��ȡ��ʪ������
	Device_Data->temperatuer = DHT11_Data.temp_int + DHT11_Data.temp_deci * 0.01;
	Device_Data->humiditr = DHT11_Data.humi_int + DHT11_Data.humi_deci * 0.01;
	Device_Data->humi_soil1 = Soil1Humi_Value();
	Device_Data->humi_soil2 = Soil2Humi_Value();
	return MY_SUCCESSFUL;
}
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state)
{
	// ��ֵ����
	threshold_value->threshold_soil2_humidity = 50;
	threshold_value->threshold_soil1_humidity = 50;
	return MY_SUCCESSFUL;
}
// ����OLED��ʾ��������
mySta Update_oled_massage()
{
#if OLED // �Ƿ��
	char str[50];
	sprintf(str, "-temp:%.1f humi:%.1f-", Data_init.temperatuer, Data_init.humiditr);
	OLED_ShowString(0, 0, (unsigned char *)str, 12);
	sprintf(str, "soil1_h: %03d", Data_init.humi_soil1);
	OLED_ShowString(0, 16, (unsigned char *)str, 16);
	sprintf(str, "soil2_h: %03d", Data_init.humi_soil2);
	OLED_ShowString(0, 32, (unsigned char *)str, 16);
	sprintf(str, "light: %.3d", Data_init.light);
	OLED_ShowString(0, 48, (unsigned char *)str, 16);
	OLED_Refresh(); // ������ʾ
#endif
	return MY_SUCCESSFUL;
}

// �����豸״̬
mySta Update_device_massage()
{

	if (Data_init.flage == 0)
	{
		// ˮ��һ
		if (pump1in == 0 && Data_init.humi_soil1 < threshold_value_init.threshold_soil1_humidity)
		{
			pump1out = 1;
		}
		else if (Data_init.humi_soil1 > threshold_value_init.threshold_soil1_humidity)
		{
			pump1out = 0;
		}
		// ˮ�ö�
		if (pump2in == 0 && Data_init.humi_soil2 < threshold_value_init.threshold_soil2_humidity)
		{
			pump2out = 1;
		}
		else if (Data_init.humi_soil2 > threshold_value_init.threshold_soil2_humidity)
		{
			pump2out = 0;
		}
	}

	if (Data_init.App)
	{
		switch (Data_init.App)
		{
		case 2:
			OneNet_SendMqtt(2); // �������ݵ�APP
			break;
		case 3:
			OneNet_SendMqtt(3); // �������ݵ�APP
			break;
		case 4:
			OneNet_SendMqtt(4); // �������ݵ�APP
			break;
		case 5:
			OneNet_SendMqtt(5); // �������ݵ�APP
			break;
		case 6:
			OneNet_SendMqtt(6); // �������ݵ�APP
			break;
		default:
			Data_init.App = 0;
			break;
		}
		Data_init.App = 0;
	}

	return MY_SUCCESSFUL;
}
// ����json����
mySta massage_parse_json(char *message)
{

	cJSON *cjson_test = NULL; // ���json��ʽ
	cJSON *cjson_data = NULL; // ����
	const char *massage;
	// ������������
	u8 cjson_cmd; // ָ��,����

	/* ��������JSO���� */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// ����ʧ��
		printf("parse fail.\n");
		return MY_FAIL;
	}

	/* ���θ���������ȡJSON���ݣ���ֵ�ԣ� */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* ����Ƕ��json���� */
	cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{
	case 0x01: // ��Ϣ��
		Data_init.flage = cJSON_GetObjectItem(cjson_data, "Flage")->valueint;
		Data_init.App = cjson_cmd + 1;

		break;
	case 0x02: // ��Ϣ��
		threshold_value_init.threshold_soil1_humidity = cJSON_GetObjectItem(cjson_data, "Soil1_v")->valueint;
		threshold_value_init.threshold_soil2_humidity = cJSON_GetObjectItem(cjson_data, "Soil2_v")->valueint;
		Data_init.App = cjson_cmd + 1;

		break;
	case 0x03: // ���ݰ�
		Data_init.App = cjson_cmd + 1;
		pump1out = cJSON_GetObjectItem(cjson_data, "Water_Pump1")->valueint;
		break;
	case 0x04: // ���ݰ�
		Data_init.App = cjson_cmd + 1;
		pump2out = cJSON_GetObjectItem(cjson_data, "Water_Pump2")->valueint;
		break;
	default:
		break;
	}

	/* ���JSON����(��������)���������� */
	cJSON_Delete(cjson_test);
	// cJSON_Delete(cjson_data);

	return MY_SUCCESSFUL;
}
