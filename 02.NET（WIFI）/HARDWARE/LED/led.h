#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "git.h"
/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define Relay_GPIO_PORT    	GPIOC          /* GPIO端口 */
#define Relay_GPIO_CLK 	    RCC_APB2Periph_GPIOC	/* GPIO端口时钟 */
#define Relay_GPIO_PIN			GPIO_Pin_13

#define Read_State    GPIO_ReadInputDataBit(Relay_GPIO_PORT,Relay_GPIO_PIN)
#define LED PCout(13)	// LED接口	
#define OFF	  1
#define ON		0

void LED_Init(void);//初始化
/**
  * @brief  MQTT操作LED模块
  * @param  无
  * @retval 无
  */
void Mqtt_LED( int Cmd);		    
#endif
