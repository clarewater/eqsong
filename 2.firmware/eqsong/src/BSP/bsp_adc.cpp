
#include "bsp.h"
#include <driver/adc.h>

void BSP::bsp_adc_init()
{
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_0db);
}
uint16_t BSP::bsp_adc_read(adc1_channel_t adc_channel)
{

    return adc1_get_raw(adc_channel);
}
uint16_t BSP::bsp_adc_read_avg(adc1_channel_t adc_channel, uint8_t sample_num)
{

    int sum = 0;
    for (uint8_t i = 0; i < sample_num; i++)
    {
        sum += adc1_get_raw(adc_channel);
        delayMicroseconds(100);
    }
    return sum / sample_num;
}