//
//  filtering_functions.h
//  eqmodule
//
//  Created by clarewater on 2025/7/6.
//

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef FIlTER_FUNCTIONS_H
#define FIlTER_FUNCTIONS_H
#include <stdint.h> // 定义精确宽度整数类型（如 int32_t）
#include <string.h> // C99及之后的标准

    typedef struct
    {
        unsigned int numStages; /*2阶节的个数，应为2*numStages.        */
        float *pState;          /*状态系数数组指针，数组长度为4*numStages*/
        float *pCoeffs;         /*系数数组指针， 数组的长度为5*numStages.*/
    } arm_biquad_casd_df1_inst_f32;
    typedef float float32_t; // 手动定义

    extern void arm_biquad_cascade_df1_f32(
        const arm_biquad_casd_df1_inst_f32 *S,
        float *pSrc,
        float *pDst,
        unsigned int blockSize);
    extern void arm_biquad_cascade_df1_init_f32(
        arm_biquad_casd_df1_inst_f32 *S,
        uint8_t numStages,
        float32_t *pCoeffs,
        float32_t *pState);

    extern void arm_biquad_cascade_df1_int16(
        const arm_biquad_casd_df1_inst_f32 *S,
        int16_t *pSrc,
        int16_t *pDst,
        unsigned int blockSize);
    extern void arm_biquad_cascade_df1_init_int16(
        arm_biquad_casd_df1_inst_f32 *S,
        uint8_t numStages,
        float32_t *pCoeffs,
        float32_t *pState);
#endif

#ifdef __cplusplus
}
#endif
