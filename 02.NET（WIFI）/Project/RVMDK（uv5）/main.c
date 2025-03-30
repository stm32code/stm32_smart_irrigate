// C��
#include <string.h>
// ��Ƭ��ͷ�ļ�
#include "sys.h"
// ����Э���
#include "onenet.h"
// �����豸
#include "esp8266.h"
// ������
#include "task.h"
#include "timer.h"
// Ӳ������
#include "delay.h"
#include "usart.h"
#include "git.h"
#include "led.h"
#include "bh1750.h"
#include "ds18b20.h"
#include "dht11.h"
#include "relay.h"
#include "adc.h"
#if OLED // OLED�ļ�����
#include "oled.h"
#endif
// ����״̬

// �����ʱ���趨
static Timer task1_id;
static Timer task2_id;
static Timer task3_id;

// ��ȡȫ�ֱ���
extern Data_TypeDef Data_init;
extern Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
extern Device_Satte_Typedef device_state_init;       // �豸״̬
const char *topics[] = {S_TOPIC_NAME};
u8 ucDs18b20Id[8];

// Ӳ����ʼ��
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�

    delay_init(); // ��ʱ������ʼ��
    GENERAL_TIM_Init(TIM_2, 0, 2);
    Usart1_Init(115200); // ����1��ʼ��Ϊ115200
 
    LED_Init();
    RELAY_GPIO_Config();         // �̵���
   
    DHT11_GPIO_Config();         // dht11
  
#endif
}
// �����ʼ��
void Net_Init()
{

    ESP8266_Init(); // ��ʼ��ESP8266
    LED = ON;       // �����ɹ�
    delay_ms(1000);
    LED = OFF;
    while (Reset_Threshole_Value(&threshold_value_init, &device_state_init) != MY_SUCCESSFUL)
        delay_ms(5); // ��ʼ����ֵ
}

// ����1
void task1(void)
{
    if (!Connect_Net && Data_init.App == 0) {
        OneNet_SendMqtt(1); // �������ݵ�APP
    }
}
// ����2
void task2(void)
{

    while (Read_Data(&Data_init) != MY_SUCCESSFUL)
        delay_ms(5); // ���´���������
    while (Update_oled_massage() != MY_SUCCESSFUL)
        delay_ms(5); // ����OLED
     
}
// ����3
void task3(void)
{
    Data_init.light = getValue(); // ��ȡ���ǿ��
}
// �����ʼ��
void SoftWare_Init(void)
{
    // ��ʱ����ʼ��
    timer_init(&task1_id, task1, 5000, 1); // ÿ�����ϴ�һ���豸����
    timer_init(&task2_id, task2, 250, 1);  // �������ݰ�
 
 
}
// ������
int main(void)
{
    char str[50];
    u8 oeld_state_of_wifi = 0;
    unsigned char *dataPtr = NULL;
    SoftWare_Init();       // �����ʼ��
    Hardware_Init();       // Ӳ����ʼ��
    Net_Init();            // �����ʼ��
    TIM_Cmd(TIM2, ENABLE); // ʹ�ܼ�����
    while (1) {
        if (Connect_Net) {
            // ����δ����
            if (!oeld_state_of_wifi) {
                // ����
                OLED_Clear();
                oeld_state_of_wifi = 1;
  
        } else {
            if (oeld_state_of_wifi) {
                // ����
                OLED_Clear();
                oeld_state_of_wifi = 0;
            }
            // �߳�
            timer_loop(); // ��ʱ��ִ��
            // ���ڽ����ж�
            dataPtr = ESP8266_GetIPD(0);
            if (dataPtr != NULL) {
                if (massage_parse_json((char *)dataPtr) == MY_SUCCESSFUL) {
                    ESP8266_Clear();
                }
            }
        }
    }
}
