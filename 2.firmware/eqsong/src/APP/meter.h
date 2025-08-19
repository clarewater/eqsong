#if !defined(METER_H)
#define METER_H
#include "BSP/bsp.h"
#define METER100_ADC1_CHANNEL ADC1_CHANNEL_5 // GPIO33
#define METER400_ADC1_CHANNEL ADC1_CHANNEL_4 // GPIO32
#define METER3000_ADC1_CHANNEL ADC1_CHANNEL_6 // GPIO35
#define METER10000_ADC1_CHANNEL ADC1_CHANNEL_7 // GPIO34
typedef enum
{
    MeterBtn100 = 0,
    MeterBtn400,
    MeterBtn3000,
    MeterBtn10000,
} MeterBtn;

typedef struct
{
    MeterBtn btn;
    uint16_t value;
} MeterValue;

typedef struct
{
    uint16_t value100;
    uint16_t value400;
    uint16_t value3000;
    uint16_t value10000;
} MeterList, *MeterListPtr;
namespace Meter
{

    void meter_init();
    void meter_set_value(MeterValue *meter_value);
    int meter_get_value(MeterBtn btn);

    BaseType_t push_event_meter_value(MeterList *meter_list);
    BaseType_t listen_meter_value(MeterList *meter_list);
}

#endif // METER_H
