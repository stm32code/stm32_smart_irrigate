#include "delay.h"
#include "led.h"
// Э���ļ�
#include "onenet.h"
// �����豸����
#include "esp8266.h"

// ��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��
// LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(Relay_GPIO_CLK, ENABLE); // ʹ�ܶ˿�ʱ��C

	GPIO_InitStructure.GPIO_Pin = Relay_GPIO_PIN;	  // �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	GPIO_Init(Relay_GPIO_PORT, &GPIO_InitStructure);  // ������� ��IO���ٶ�Ϊ50MHz

	LED = OFF;
}
/**
 * @brief  MQTT����LEDģ��
 * @param  ��
 * @retval ��
 */
void Mqtt_LED(int Cmd)
{

	ESP8266_Clear(); // ��ջ���
	switch (Cmd)
	{
	case 1:
		LED = ~LED;
		OneNet_SendMqtt(1); // �ϴ����ĵ�������
		break;
	case 0:
		LED = ~LED;
		OneNet_SendMqtt(1); // �ϴ����ĵ�����
		break;
	default:

		break;
	}
	return;
}
