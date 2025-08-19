#include <Arduino.h>
#include "BSP/bsp.h"
#include "APP/game.h"
void setup()
{
  BSP::bsp_init();
  APP::run();
}

void loop()
{
  listen_meter_coroutine();

  // put your main code here, to run repeatedly:
}
