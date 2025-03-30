#include "delay.h"
#include "led.h"
// 协议文件
#include "onenet.h"
// 网络设备驱动
#include "esp8266.h"

// 初始化PB5和PE5为输出口.并使能这两个口的时钟
// LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(Relay_GPIO_CLK, ENABLE); // 使能端口时钟C

	GPIO_InitStructure.GPIO_Pin = Relay_GPIO_PIN;	  // 端口配置, 推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
	GPIO_Init(Relay_GPIO_PORT, &GPIO_InitStructure);  // 推挽输出 ，IO口速度为50MHz

	LED = OFF;
}
/**
 * @brief  MQTT操作LED模块
 * @param  无
 * @retval 无
 */
void Mqtt_LED(int Cmd)
{

	ESP8266_Clear(); // 清空缓存
	switch (Cmd)
	{
	case 1:
		LED = ~LED;
		OneNet_SendMqtt(1); // 上传订阅的主题里
		break;
	case 0:
		LED = ~LED;
		OneNet_SendMqtt(1); // 上传订阅的主题
		break;
	default:

		break;
	}
	return;
}
