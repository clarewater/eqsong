

#include "./meter.h"
#include <Arduino.h>

/*
 *    ADC1_CHANNEL_4,     /*!< ADC1 channel 4 is GPIO32
 *     ADC1_CHANNEL_5,     /*!< ADC1 channel 5 is GPIO33
 *    ADC1_CHANNEL_6,     /*!< ADC1 channel 6 is GPIO34
 *    ADC1_CHANNEL_7,     /*!< ADC1 channel 7 is GPIO35
 */
static thread_mq_t g_queue; // 全局队列句柄

int Meter::meter_get_value(MeterBtn btn)
{

    switch (btn)
    {
    case MeterBtn100:
        return BSP::bsp_adc_read_avg(METER100_ADC1_CHANNEL, 3);
        break;
    case MeterBtn400:
        return BSP::bsp_adc_read_avg(METER400_ADC1_CHANNEL, 3);
        break;
    case MeterBtn3000:
        return BSP::bsp_adc_read_avg(METER3000_ADC1_CHANNEL, 3);
        break;
    case MeterBtn10000:
        return BSP::bsp_adc_read_avg(METER10000_ADC1_CHANNEL, 3);
        break;
    default:
        break;
    }

    return 0;
}

void Meter::meter_init()
{
    g_queue = thread_mq_init("my_queue", sizeof(MeterList), 10);
    if (g_queue == NULL)
    {
        while (1)
            ; // 死循环报错
    }
    else
    {
        DEBUG_PRINT("Meter::队列初始化成功");
    }
    // 初始化一些队列
}

BaseType_t Meter::push_event_meter_value(MeterList *meter_list){

    // 一次性发布4个数据
    return thread_mq_send(g_queue, meter_list, sizeof(MeterList), pdMS_TO_TICKS(100))}

BaseType_t Meter::listen_meter_value(MeterList *meter_list)
{

    // 一次性发布4个数据
    return thread_mq_recv(g_queue, meter_list, sizeof(MeterList), 0)
}
