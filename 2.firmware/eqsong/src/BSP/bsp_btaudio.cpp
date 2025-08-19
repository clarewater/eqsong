
#include "bsp.h"
#include "3thlib/btaudio/btAudio.h"

static btAudio audio("eqSpeak");
#define MCK_GPIO_NUM 0
#define BCK_GPIO_NUM 26
#define WS_GPIO_NUM 25
#define DOUT_GPIO_NUM 27

void BSP::bsp_audio_init()
{

    audio.begin();
   audio.reconnect();
    BSP::bsp_audio_play();
}

void BSP::bsp_audio_end()
{
    esp_a2d_sink_deinit();
    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    btStop();
}

void BSP::bsp_audio_play()
{

    audio.I2S(MCK_GPIO_NUM, BCK_GPIO_NUM, DOUT_GPIO_NUM, WS_GPIO_NUM);
    audio.setPostProcess(NOTHING);
}

void BSP::bsp_audio_volume(float vol)
{
    audio.volume(vol);
}

/**
 * @brief 左声道传入IIR矩阵
 *
 * @param S
 * @param numStages
 * @param pCoeffs
 * @param pState
 */
void BSP::bsp_audio_setDSP_iir_left(
    uint8_t numStages,
    float32_t *pCoeffs,
    float32_t *pState)
{

    audio.setPostProcess(NOTHING);

    audio.setDSP_iir_left(numStages, pCoeffs, pState);

    audio.setPostProcess(EQ_FILTER);
}

/**
 * @brief 右声道 传入IIR矩阵
 *
 * @param S
 * @param numStages
 * @param pCoeffs
 * @param pState
 */
void BSP::bsp_audio_setDSP_iir_right(
    uint8_t numStages,
    float32_t *pCoeffs,
    float32_t *pState)
{

    audio.setPostProcess(NOTHING);

    audio.setDSP_iir_right(numStages, pCoeffs, pState);

    audio.setPostProcess(EQ_FILTER);
}
