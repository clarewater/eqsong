

#include "./game.h"
#include "./BSP/bsp.h"
#include "filterapp.h"
#include "meter.h"
#include "apptask.h"
void APP::run()
{

  // int value1 = Meter ::meter_get_value(MeterBtn100);
  // int value2 = Meter ::meter_get_value(MeterBtn400);
  // int value3 = Meter ::meter_get_value(MeterBtn3000);
  // int value4 = Meter ::meter_get_value(MeterBtn10000);

  /*
  * 按键扫描线程  注意消除抖动 发现数据 前后不一致 将数据发送到数据总线上
    DSP接收按键线程  接收数据总线上的数据  处理数据
  */
  Meter::meter_init();
  create_meter_thread();
  //create_listen_thread();

  delay(1000);
}
