
#if !defined(FILTER_APP_H)
#define FILTER_APP_H

#include "BSP/bsp.h"
#define IIR 4 // 4段级联
typedef enum
{
    GAIN_MINUS_12DB = -12,
    GAIN_MINUS_6DB = -6,
    GAIN_MINUS_3DB = -3,
    GAIN_0DB = 0,
    GAIN_PLUS_1DB = 1,
    GAIN_PLUS_2DB = 2,
    GAIN_PLUS_3DB = 3,
    GAIN_PLUS_6DB = 6
} GainLevel;
// 声明为 extern（不分配内存）

extern float left_state[4 * IIR];   /*历史状态缓冲区         */
extern float right_state[4 * IIR];  /*历史状态缓冲区         */
extern float left_coeffs[5 * IIR];  /*系数数组指针， 数组的长度为5*numStages. 根据档位赋值给矩阵 */
extern float right_coeffs[5 * IIR]; /*系数数组指针， 数组的长度为5*numStages. 根据档位赋值给矩阵*/
extern float f100_sos[35];          // 5 * 7
extern float f400_sos[35];
extern float f3000_sos[35];
extern float f10000_sos[35];
void sos_filter_begin(GainLevel gain1, GainLevel gain2, GainLevel gain3, GainLevel gain4);

GainLevel get_gain_level(int value);

#endif // FILTER_APP_H
