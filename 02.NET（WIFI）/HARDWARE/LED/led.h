#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "git.h"
/* ����LED���ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define Relay_GPIO_PORT    	GPIOC          /* GPIO�˿� */
#define Relay_GPIO_CLK 	    RCC_APB2Periph_GPIOC	/* GPIO�˿�ʱ�� */
#define Relay_GPIO_PIN			GPIO_Pin_13

#define Read_State    GPIO_ReadInputDataBit(Relay_GPIO_PORT,Relay_GPIO_PIN)
#define LED PCout(13)	// LED�ӿ�	
#define OFF	  1
#define ON		0

void LED_Init(void);//��ʼ��
/**
  * @brief  MQTT����LEDģ��
  * @param  ��
  * @retval ��
  */
void Mqtt_LED( int Cmd);		    
#endif
