#include "git.h"
#if OLED // OLED文件存在
#include "oled.h"
#endif
//  C库
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
// 添加设备配置文件
DHT11_Data_TypeDef DHT11_Data;				  // 温湿度结构体
Data_TypeDef Data_init;						  // 设备数据结构体
Threshold_Value_TypeDef threshold_value_init; // 设备阈值设置结构体
Device_Satte_Typedef device_state_init;		  // 设备状态
extern u8 ucDs18b20Id[8];

// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data)
{
	Read_DHT11(&DHT11_Data); // 获取温湿度数据
	Device_Data->temperatuer = DHT11_Data.temp_int + DHT11_Data.temp_deci * 0.01;
	Device_Data->humiditr = DHT11_Data.humi_int + DHT11_Data.humi_deci * 0.01;
	Device_Data->humi_soil1 = Soil1Humi_Value();
	Device_Data->humi_soil2 = Soil2Humi_Value();
	return MY_SUCCESSFUL;
}
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state)
{
	// 阈值定义
	threshold_value->threshold_soil2_humidity = 50;
	threshold_value->threshold_soil1_humidity = 50;
	return MY_SUCCESSFUL;
}
// 更新OLED显示屏中内容
mySta Update_oled_massage()
{
#if OLED // 是否打开
	char str[50];
	sprintf(str, "-temp:%.1f humi:%.1f-", Data_init.temperatuer, Data_init.humiditr);
	OLED_ShowString(0, 0, (unsigned char *)str, 12);
	sprintf(str, "soil1_h: %03d", Data_init.humi_soil1);
	OLED_ShowString(0, 16, (unsigned char *)str, 16);
	sprintf(str, "soil2_h: %03d", Data_init.humi_soil2);
	OLED_ShowString(0, 32, (unsigned char *)str, 16);
	sprintf(str, "light: %.3d", Data_init.light);
	OLED_ShowString(0, 48, (unsigned char *)str, 16);
	OLED_Refresh(); // 开启显示
#endif
	return MY_SUCCESSFUL;
}

// 更新设备状态
mySta Update_device_massage()
{

	if (Data_init.flage == 0)
	{
		// 水泵一
		if (pump1in == 0 && Data_init.humi_soil1 < threshold_value_init.threshold_soil1_humidity)
		{
			pump1out = 1;
		}
		else if (Data_init.humi_soil1 > threshold_value_init.threshold_soil1_humidity)
		{
			pump1out = 0;
		}
		// 水泵二
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
			OneNet_SendMqtt(2); // 发送数据到APP
			break;
		case 3:
			OneNet_SendMqtt(3); // 发送数据到APP
			break;
		case 4:
			OneNet_SendMqtt(4); // 发送数据到APP
			break;
		case 5:
			OneNet_SendMqtt(5); // 发送数据到APP
			break;
		case 6:
			OneNet_SendMqtt(6); // 发送数据到APP
			break;
		default:
			Data_init.App = 0;
			break;
		}
		Data_init.App = 0;
	}

	return MY_SUCCESSFUL;
}
// 解析json数据
mySta massage_parse_json(char *message)
{

	cJSON *cjson_test = NULL; // 检测json格式
	cJSON *cjson_data = NULL; // 数据
	const char *massage;
	// 定义数据类型
	u8 cjson_cmd; // 指令,方向

	/* 解析整段JSO数据 */
	cjson_test = cJSON_Parse(message);
	if (cjson_test == NULL)
	{
		// 解析失败
		printf("parse fail.\n");
		return MY_FAIL;
	}

	/* 依次根据名称提取JSON数据（键值对） */
	cjson_cmd = cJSON_GetObjectItem(cjson_test, "cmd")->valueint;
	/* 解析嵌套json数据 */
	cjson_data = cJSON_GetObjectItem(cjson_test, "data");

	switch (cjson_cmd)
	{
	case 0x01: // 消息包
		Data_init.flage = cJSON_GetObjectItem(cjson_data, "Flage")->valueint;
		Data_init.App = cjson_cmd + 1;

		break;
	case 0x02: // 消息包
		threshold_value_init.threshold_soil1_humidity = cJSON_GetObjectItem(cjson_data, "Soil1_v")->valueint;
		threshold_value_init.threshold_soil2_humidity = cJSON_GetObjectItem(cjson_data, "Soil2_v")->valueint;
		Data_init.App = cjson_cmd + 1;

		break;
	case 0x03: // 数据包
		Data_init.App = cjson_cmd + 1;
		pump1out = cJSON_GetObjectItem(cjson_data, "Water_Pump1")->valueint;
		break;
	case 0x04: // 数据包
		Data_init.App = cjson_cmd + 1;
		pump2out = cJSON_GetObjectItem(cjson_data, "Water_Pump2")->valueint;
		break;
	default:
		break;
	}

	/* 清空JSON对象(整条链表)的所有数据 */
	cJSON_Delete(cjson_test);
	// cJSON_Delete(cjson_data);

	return MY_SUCCESSFUL;
}
