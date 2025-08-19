//
//  filtering_functions.cpp
//  eqmodule
//
//  Created by clarewater on 2025/7/6.
//

#include "./filter_function.h"

/*
 *作用      :初始化滤波器
 *S        :指向浮点SOS级联结构的实例。
 *numStages:滤波器中二阶SOS的数量
 *pCoeffs  :滤波器参数指针,参数按下列顺序存储
 *          {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22, ...}
 *pState   :历史状态缓冲区指针
 */
void arm_biquad_cascade_df1_init_f32(
    arm_biquad_casd_df1_inst_f32 *S,
    uint8_t numStages,
    float32_t *pCoeffs,
    float32_t *pState)
{
  /* Assign filter stages */
  S->numStages = numStages;

  /* Assign coefficient pointer */
  S->pCoeffs = pCoeffs;

  /* Clear state buffer and size is always 4 * numStages */
  memset(pState, 0, (4U * (uint32_t)numStages) * sizeof(float32_t));

  /* Assign state pointer */
  S->pState = pState;
}

/**
 *  *S       ：指向浮点Biquad级联结构的实例.
 *  *pSrc    ：指向输入数据块。
 *  *pDst    ：指向输出数据块。
 *  blockSize：每次调用要处理的样本数。
 *  返回值    ：无.
 */
void arm_biquad_cascade_df1_f32(
    const arm_biquad_casd_df1_inst_f32 *S,
    float32_t *pSrc,
    float32_t *pDst,
    unsigned int blockSize)
{

  float *pIn = pSrc;                         /*源指针     */
  float *pOut = pDst;                        /*目的指针    */
  float *pState = S->pState;                 /*状态指针    */
  float *pCoeffs = S->pCoeffs;               /*参数指针    */
  float acc;                                 /*累加器      */
  float b0, b1, b2, a1, a2;                  /*滤波器参数   */
  float Xn1, Xn2, Yn1, Yn2;                  /*滤波器状态变量*/
  float Xn;                                  /*临时输入     */
  unsigned int sample, stage = S->numStages; /*循环计数     */

  do
  {
    /* Reading the coefficients */
    b0 = *pCoeffs++;
    b1 = *pCoeffs++;
    b2 = *pCoeffs++;
    a1 = *pCoeffs++;
    a2 = *pCoeffs++;

    /* Reading the pState values */
    Xn1 = pState[0];
    Xn2 = pState[1];
    Yn1 = pState[2];
    Yn2 = pState[3];

#if defined(ARM_MATH_LOOPUNROLL) && !defined(ARM_MATH_AUTOVECTORIZE)

    /* Apply loop unrolling and compute 4 output values simultaneously. */
    /* Variable acc hold output values that are being computed:
     *
     * acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
     * acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
     * acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
     * acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2]
     */

    /* Loop unrolling: Compute 4 outputs at a time */
    sample = blockSize >> 2U;

    while (sample > 0U)
    {
      /* Read the first input */
      Xn = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      Yn2 = (b0 * Xn) + (b1 * Xn1) + (b2 * Xn2) + (a1 * Yn1) + (a2 * Yn2);

      /* Store output in destination buffer. */
      *pOut++ = Yn2;

      /* Every time after the output is computed state should be updated. */
      /* The states should be updated as: */
      /* Xn2 = Xn1 */
      /* Xn1 = Xn  */
      /* Yn2 = Yn1 */
      /* Yn1 = acc */

      /* Read the second input */
      Xn2 = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      Yn1 = (b0 * Xn2) + (b1 * Xn) + (b2 * Xn1) + (a1 * Yn2) + (a2 * Yn1);

      /* Store output in destination buffer. */
      *pOut++ = Yn1;

      /* Every time after the output is computed state should be updated. */
      /* The states should be updated as: */
      /* Xn2 = Xn1 */
      /* Xn1 = Xn  */
      /* Yn2 = Yn1 */
      /* Yn1 = acc */

      /* Read the third input */
      Xn1 = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      Yn2 = (b0 * Xn1) + (b1 * Xn2) + (b2 * Xn) + (a1 * Yn1) + (a2 * Yn2);

      /* Store output in destination buffer. */
      *pOut++ = Yn2;

      /* Every time after the output is computed state should be updated. */
      /* The states should be updated as: */
      /* Xn2 = Xn1 */
      /* Xn1 = Xn  */
      /* Yn2 = Yn1 */
      /* Yn1 = acc */

      /* Read the forth input */
      Xn = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      Yn1 = (b0 * Xn) + (b1 * Xn1) + (b2 * Xn2) + (a1 * Yn2) + (a2 * Yn1);

      /* Store output in destination buffer. */
      *pOut++ = Yn1;

      /* Every time after the output is computed state should be updated. */
      /* The states should be updated as: */
      /* Xn2 = Xn1 */
      /* Xn1 = Xn  */
      /* Yn2 = Yn1 */
      /* Yn1 = acc */
      Xn2 = Xn1;
      Xn1 = Xn;

      /* decrement loop counter */
      sample--;
    }

    /* Loop unrolling: Compute remaining outputs */
    sample = blockSize & 0x3U;

#else

    /* Initialize blkCnt with number of samples */
    sample = blockSize;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

    while (sample > 0U)
    {
      /* Read the input */
      Xn = *pIn++;

      /* acc =  b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + a1 * y[n-1] + a2 * y[n-2] */
      acc = (b0 * Xn) + (b1 * Xn1) + (b2 * Xn2) + (a1 * Yn1) + (a2 * Yn2);

      /* Store output in destination buffer. */
      *pOut++ = acc;

      /* Every time after the output is computed state should be updated. */
      /* The states should be updated as: */
      /* Xn2 = Xn1 */
      /* Xn1 = Xn  */
      /* Yn2 = Yn1 */
      /* Yn1 = acc */
      Xn2 = Xn1;
      Xn1 = Xn;
      Yn2 = Yn1;
      Yn1 = acc;

      /* decrement loop counter */
      sample--;
    }

    /* Store the updated state variables back into the pState array */
    *pState++ = Xn1;
    *pState++ = Xn2;
    *pState++ = Yn1;
    *pState++ = Yn2;

    /* The first stage goes from the input buffer to the output buffer. */
    /* Subsequent numStages occur in-place in the output buffer */
    pIn = pDst;

    /* Reset output pointer */
    pOut = pDst;

    /* decrement loop counter */
    stage--;

  } while (stage > 0U);
}

void arm_biquad_cascade_df1_init_int16(
    arm_biquad_casd_df1_inst_f32 *S,
    uint8_t numStages,
    float32_t *pCoeffs,
    float32_t *pState)
{
  /* Assign filter stages */
  S->numStages = numStages;

  /* Assign coefficient pointer */
  S->pCoeffs = pCoeffs;

  /* Clear state buffer and size is always 4 * numStages */
  memset(pState, 0, (4U * (uint32_t)numStages) * sizeof(float32_t));

  /* Assign state pointer */
  S->pState = pState;
}
void arm_biquad_cascade_df1_int16(
    const arm_biquad_casd_df1_inst_f32 *S,
    int16_t *pSrc,
    int16_t *pDst,
    unsigned int blockSize)
{

  const int16_t *pIn = pSrc;
  int16_t *pOut = pDst;

  float *pState = S->pState;
  float *pCoeffs = S->pCoeffs;
  float acc;
  float b0, b1, b2, a1, a2;
  float Xn1, Xn2, Yn1, Yn2;
  float Xn;
  uint32_t sample;
  uint32_t stage = S->numStages;

  do
  {
    // 系数
    b0 = *pCoeffs++;
    b1 = *pCoeffs++;
    b2 = *pCoeffs++;
    a1 = *pCoeffs++;
    a2 = *pCoeffs++;

    // 状态
    Xn1 = pState[0];
    Xn2 = pState[1];
    Yn1 = pState[2];
    Yn2 = pState[3];

    sample = blockSize;

    while (sample > 0U)
    {
      // 输入：int16 → float
      Xn = (float)(*pIn++);

      // Biquad 公式：y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] + a1*y[n-1] + a2*y[n-2]
      acc = b0 * Xn + b1 * Xn1 + b2 * Xn2 + a1 * Yn1 + a2 * Yn2;

      // 输出：float → int16，带饱和
      if (acc > 32767.0f)
      {
        acc = 32767.0f;
      }
      else if (acc < -32768.0f)
      {
        acc = -32768.0f;
      }
      *pOut++ = (int16_t)(acc);

      // 更新状态
      Xn2 = Xn1;
      Xn1 = Xn;
      Yn2 = Yn1;
      Yn1 = acc;

      sample--;
    }

    // 写回状态
    *pState++ = Xn1;
    *pState++ = Xn2;
    *pState++ = Yn1;
    *pState++ = Yn2;

    // 下一 stage 输入 = 上一输出
    pIn = pDst;
    pOut = pDst;

    stage--;

  } while (stage > 0U);
}
