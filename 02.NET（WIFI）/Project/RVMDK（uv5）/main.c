// C库
#include <string.h>
// 单片机头文件
#include "sys.h"
// 网络协议层
#include "onenet.h"
// 网络设备
#include "esp8266.h"
// 任务栏
#include "task.h"
#include "timer.h"
// 硬件驱动
#include "delay.h"
#include "usart.h"
#include "git.h"
#include "led.h"
#include "bh1750.h"
#include "ds18b20.h"
#include "dht11.h"
#include "relay.h"
#include "adc.h"
#if OLED // OLED文件存在
#include "oled.h"
#endif
// 联网状态

// 软件定时器设定
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;

// 获取全局变量
extern Data_TypeDef Data_init;
extern Threshold_Value_TypeDef threshold_value_init; // 设备阈值设置结构体
extern Device_Satte_Typedef device_state_init;       // 设备状态
const char *topics[] = {S_TOPIC_NAME};
u8 ucDs18b20Id[8];

// 硬件初始化
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组为组2：2位抢占优先级，2位响应优先级

    delay_init(); // 延时函数初始化
    GENERAL_TIM_Init(TIM_2, 0, 2);
    Usart1_Init(115200); // 串口1初始化为115200
 
    LED_Init();
    RELAY_GPIO_Config();         // 继电器
   
    DHT11_GPIO_Config();         // dht11
  
#endif
}
// 网络初始化
void Net_Init()
{

    ESP8266_Init(); // 初始化ESP8266
    LED = ON;       // 入网成功
    delay_ms(1000);
    LED = OFF;
    while (Reset_Threshole_Value(&threshold_value_init, &device_state_init) != MY_SUCCESSFUL)
        delay_ms(5); // 初始化阈值
}

// 任务1
void task1(void)
{
    if (!Connect_Net && Data_init.App == 0) {
        OneNet_SendMqtt(1); // 发送数据到APP
    }
}
// 任务2
void task2(void)
{

    while (Read_Data(&Data_init) != MY_SUCCESSFUL)
        delay_ms(5); // 更新传感器数据
    while (Update_oled_massage() != MY_SUCCESSFUL)
        delay_ms(5); // 更新OLED
     
}
// 任务3
void task3(void)
{
    Data_init.light = getValue(); // 获取光感强度
}
// 软件初始化
void SoftWare_Init(void)
{
    // 定时器初始化
    timer_init(&task1_id, task1, 5000, 1); // 每八秒上传一次设备数据
    timer_init(&task2_id, task2, 250, 1);  // 跟新数据包
 
 
}
// 主函数
int main(void)
{
    char str[50];
    u8 oeld_state_of_wifi = 0;
    unsigned char *dataPtr = NULL;
    SoftWare_Init();       // 软件初始化
    Hardware_Init();       // 硬件初始化
    Net_Init();            // 网络初始化
    TIM_Cmd(TIM2, ENABLE); // 使能计数器
    while (1) {
        if (Connect_Net) {
            // 网络未连接
            if (!oeld_state_of_wifi) {
                // 清屏
                OLED_Clear();
                oeld_state_of_wifi = 1;
  
        } else {
            if (oeld_state_of_wifi) {
                // 清屏
                OLED_Clear();
                oeld_state_of_wifi = 0;
            }
            // 线程
            timer_loop(); // 定时器执行
            // 串口接收判断
            dataPtr = ESP8266_GetIPD(0);
            if (dataPtr != NULL) {
                if (massage_parse_json((char *)dataPtr) == MY_SUCCESSFUL) {
                    ESP8266_Clear();
                }
            }
        }
    }
}
