
#if !defined(BSP_H)
#define BSP_H
#include "bsp_def.h"
#include "CommonMacro.h"
#include <Arduino.h>
#include <driver/adc.h>
#include "3thlib/filter/filter_function.h"
#include "fk_soft_typedef.h"

// 宏开关：开启调试
#define DEBUG_ENABLE 1

#if DEBUG_ENABLE
#define DEBUG_PRINT(fmt, ...) Serial.printf("[DEBUG] " fmt "\r\n", ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) // 什么也不做
#endif

namespace BSP
{

    void bsp_init();

    /*  ADC 电位器*/
    void bsp_adc_init();
    uint16_t bsp_adc_read(adc1_channel_t adc_channel);
    uint16_t bsp_adc_read_avg(adc1_channel_t adc_channel, uint8_t sample_num);

    /* 蓝牙音频  audio
     播放流程：

     第一步：
     1、init
     2. volume
     3.set_left_iir
     4.set_right_iir
     5.play


     如果是切换档位

     setleft    set的时候关闭eq
     setright

     注意状态矩阵以及结构体等等，在APP层实例传入 而非局部变量
    */
    void bsp_audio_init();
    void bsp_audio_play();
    void bsp_audio_end();
    void bsp_audio_setDSP_iir_left(
                                   uint8_t numStages,
                                   float32_t *pCoeffs,
                                   float32_t *pState);
    void bsp_audio_setDSP_iir_right(
                                    uint8_t numStages,
                                    float32_t *pCoeffs,
                                    float32_t *pState);
    void bsp_audio_volume(float vol);
}

#endif // BSP_H
