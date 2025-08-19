#include "./filterapp.h"
#include "BSP/bsp.h"
#include "3thlib/btaudio/btAudio.h"
#define SOS_COEFFS_PER_GAIN 5

float left_state[4 * IIR];   /*历史状态缓冲区         */
float right_state[4 * IIR];  /*历史状态缓冲区         */
float left_coeffs[5 * IIR];  /*系数数组指针， 数组的长度为5*numStages. 根据档位赋值给矩阵 */
float right_coeffs[5 * IIR]; /*系数数组指针， 数组的长度为5*numStages. 根据档位赋值给矩阵*/
                             /*
                             fs 44100
                             增益 -6   -3    0     1     2     3     6
                             中心频率 100   400   3000    10000
                             带宽   50    200   2000   3000
                             */
float f100_sos[35] = {
    0.99750287f, -1.98978573f, 0.99248484f, 1.98978573f, -0.98998771f,
    0.99876885f, -1.99136688f, 0.99280016f, 1.99136688f, -0.99156901f,
    1.00000000f, 0.00000000f, 0.00000000f, -0.00000000f, -0.00000000f,
    1.00040893f, -1.99309491f, 0.99288828f, 1.99309491f, -0.99329722f,
    1.00081937f, -1.99346863f, 0.99285161f, 1.99346863f, -0.99367098f,
    1.00123267f, -1.99382158f, 0.99279129f, 1.99382158f, -0.99402396f,
    1.00250338f, -1.99476693f, 0.99246602f, 1.99476693f, -0.99496940f};

float f400_sos[35] = {
    0.99015866f, -1.95735800f, 0.97038229f, 1.95735800f, -0.96054095f,
    0.99513659f, -1.96350218f, 0.97155855f, 1.96350218f, -0.96669513f,
    1.00000000f, 0.00000000f, 0.00000000f, -0.00000000f, -0.00000000f,
    1.00161955f, -1.97025016f, 0.97183454f, 1.97025016f, -0.97345409f,
    1.00324687f, -1.97171412f, 0.97167355f, 1.97171412f, -0.97492042f,
    1.00488718f, -1.97309818f, 0.97141955f, 1.97309818f, -0.97630673f,
    1.00993915f, -1.97681248f, 0.97008792f, 1.97681248f, -0.98002707f};

float f3000_sos[35] = {
    0.91595709f, -1.51341407f, 0.74707113f, 1.51341407f, -0.66302822f,
    0.95746069f, -1.55496696f, 0.75122828f, 1.55496696f, -0.70868897f,
    1.00000000f, 0.00000000f, 0.00000000f, -0.00000000f, -0.00000000f,
    1.01455330f, -1.60298803f, 0.74690404f, 1.60298803f, -0.76145735f,
    1.02935067f, -1.61375490f, 0.74393794f, 1.61375490f, -0.77328861f,
    1.04442930f, -1.62405305f, 0.74017552f, 1.62405305f, -0.78460482f,
    1.09175420f, -1.65227617f, 0.72386385f, 1.65227617f, -0.81561805f};

float f10000_sos[35] = {
    0.88296279f, -0.22275163f, 0.64777412f, 0.22275163f, -0.53073690f,
    0.94011473f, -0.23136145f, 0.64978835f, 0.23136145f, -0.58990309f,
    1.00000000f, 0.00000000f, 0.00000000f, -0.00000000f, -0.00000000f,
    1.02074889f, -0.24154818f, 0.63915687f, 0.24154818f, -0.65990576f,
    1.04196574f, -0.24386792f, 0.63388116f, 0.24386792f, -0.67584689f,
    1.06369995f, -0.24609916f, 0.62747989f, 0.24609916f, -0.69117984f,
    1.13255056f, -0.25227749f, 0.60108638f, 0.25227749f, -0.73363694f};

static int gain_to_index(GainLevel gain)
{
    switch (gain)
    {
    case GAIN_MINUS_6DB:
        return 0;
    case GAIN_MINUS_3DB:
        return 1;
    case GAIN_0DB:
        return 2;
    case GAIN_PLUS_1DB:
        return 3;
    case GAIN_PLUS_2DB:
        return 4;
    case GAIN_PLUS_3DB:
        return 5;
    case GAIN_PLUS_6DB:
        return 6;
    default:
        return 2; // 默认返回 0 dB
    }
}

/**
 * @brief
 * 根据不同的增益类型跳转到不同的指针  方便dsp时候直接拿到指针就可以。
 * @param gain1
 * @param gain2
 * @param gain3
 * @param gain4
 */
void sos_filter_begin(GainLevel gain1, GainLevel gain2, GainLevel gain3, GainLevel gain4)
{
    int idx1 = gain_to_index(gain1);
    int idx2 = gain_to_index(gain2);
    int idx3 = gain_to_index(gain3);
    int idx4 = gain_to_index(gain4);

    float *f1filter_pt = f100_sos + idx1 * SOS_COEFFS_PER_GAIN;
    float *f2filter_pt = f400_sos + idx2 * SOS_COEFFS_PER_GAIN;
    float *f3filter_pt = f3000_sos + idx3 * SOS_COEFFS_PER_GAIN;
    float *f4filter_pt = f10000_sos + idx4 * SOS_COEFFS_PER_GAIN;

    // 赋值拼接数组
    memcpy(&left_coeffs[0], f1filter_pt, sizeof(float) * SOS_COEFFS_PER_GAIN);
    memcpy(&left_coeffs[5], f2filter_pt, sizeof(float) * SOS_COEFFS_PER_GAIN);
    memcpy(&left_coeffs[10], f3filter_pt, sizeof(float) * SOS_COEFFS_PER_GAIN);
    memcpy(&left_coeffs[15], f4filter_pt, sizeof(float) * SOS_COEFFS_PER_GAIN);

    memcpy(&right_coeffs[0], f1filter_pt, sizeof(float) * SOS_COEFFS_PER_GAIN);
    memcpy(&right_coeffs[5], f2filter_pt, sizeof(float) * SOS_COEFFS_PER_GAIN);
    memcpy(&right_coeffs[10], f3filter_pt, sizeof(float) * SOS_COEFFS_PER_GAIN);
    memcpy(&right_coeffs[15], f4filter_pt, sizeof(float) * SOS_COEFFS_PER_GAIN);

    BSP::bsp_audio_setDSP_iir_left(IIR, left_coeffs, left_state);
    BSP::bsp_audio_setDSP_iir_right(IIR, right_coeffs, right_state);
}

GainLevel get_gain_level(int value)
{
    if (value >= 0 && value < 350)
    {
        return GAIN_PLUS_6DB;
    }
    else if (value < 600)
    {
        return GAIN_PLUS_3DB;
    }
    else if (value < 1200)
    {
        return GAIN_PLUS_2DB;
    }
    else if (value < 2400)
    {
        return GAIN_PLUS_1DB;
    }
    else if (value < 3100)
    {
        return GAIN_0DB; // 
    }
    else if (value < 3800)
    {
        return GAIN_MINUS_3DB; // 
    }
    else if (value <= 4096)
    {
        return GAIN_MINUS_6DB;
    }
    else
    {
        return GAIN_0DB; // 默认
    }
}