
#include "./bsp.h"
static void bsp_debug_init()
{
    Serial.begin(115200);
}
void BSP::bsp_init()
{
    bsp_debug_init();
    bsp_adc_init();
    bsp_audio_init();
}
