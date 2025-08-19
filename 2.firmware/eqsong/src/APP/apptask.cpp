

#include "BSP/bsp.h"
#include "apptask.h"
#include "APP/meter.h"
#include "APP/filterapp.h"
TaskHandle_t meter_tcb = NULL;
TaskHandle_t listen_tcb = NULL;

static void meter_task_run(void *pvParameters)
{

    while (1)
    {
        UBaseType_t stack_free = uxTaskGetStackHighWaterMark(meter_tcb);
        // DEBUG_PRINT("[Meter] Stack free: %u bytes", stack_free);

        //! 发布 需要修改这个是null

        uint16_t value100 = Meter::meter_get_value(MeterBtn100);
        uint16_t value400 = Meter::meter_get_value(MeterBtn400);
        uint16_t value3000 = Meter::meter_get_value(MeterBtn3000);
        uint16_t value10000 = Meter::meter_get_value(MeterBtn10000);

        // TODO 需要根据档位调整发送数据 等板子测试

        /*
         * 0 - 4096 分为7个档位
         * 0 - 640 档位1
         * 641 - 1280 档位2
         * 1281 - 1920 档位3
         * 1921 - 2560 档位4
         * 2561 - 3200 档位5
         * 3201 - 3840 档位6
         * 3841 - 4096 档位7
         */
        DEBUG_PRINT("meter_task_run: value100 =  %d, value400 =%d, value3000 = %d, value10000 = %d", value100, value400, value3000, value10000);
        MeterList meter_list = {
            .value100 = value100,
            .value400 = value400,
            .value3000 = value3000,
            .value10000 = value10000,
        };
        Meter::push_event_meter_value(&meter_list); // 这里发布数据
        delay((1000));
    }
}

void listen_task_run(void *pvParameters)
{
    while (1)
    {
        // 监听数据变化
        //! 发布 监听数据
        UBaseType_t stack_free = uxTaskGetStackHighWaterMark(listen_tcb);
        // DEBUG_PRINT("[listen] Stack free: %u bytes", stack_free);
        MeterList meter_list;
        Meter::listen_meter_value(&meter_list); // 这里发布数据
        DEBUG_PRINT("listen_task_run: value100 =  %d, value400 =%d, value3000 = %d, value10000 = %d", meter_list.value100, meter_list.value400, meter_list.value3000, meter_list.value10000);

        delay(200);
    }
}

void create_meter_thread()
{
    thread_create(meter_task_run, "meter", 1024 * 10, NULL, 2, &meter_tcb);
    if (meter_tcb != NULL)
    {
        DEBUG_PRINT("create_meter_thread成功");
        delay(200);

        /* code */
    }
}

void create_listen_thread()
{
    thread_create(listen_task_run, "listen_tcb", 1024 * 10, NULL, 1, &listen_tcb);
    if (listen_tcb != NULL)
    {
        DEBUG_PRINT("create_listen_thread成功");
    }
    else
    {
        DEBUG_PRINT("create_listen_thread失败");
    }
}

void listen_meter_coroutine()
{
    while (1)
    {

        UBaseType_t stack_free = uxTaskGetStackHighWaterMark(listen_tcb);
        // DEBUG_PRINT("[listen_meter_coroutine] Stack free: %u bytes", stack_free);
        MeterList meter_list;
        BaseType_t ret = Meter::listen_meter_value(&meter_list); // 这里pull数据
        if (ret == pdPASS)
        {
            GainLevel gain100 = get_gain_level(meter_list.value100);
            GainLevel gain400 = get_gain_level(meter_list.value400);
            GainLevel gain3000 = get_gain_level(meter_list.value3000);
            GainLevel gain10000 = get_gain_level(meter_list.value10000);

            DEBUG_PRINT("listen_task_run: value100 = %d (%d dB), value400 = %d (%d dB), value3000 = %d (%d dB), value10000 = %d (%d dB)",
                        meter_list.value100, gain100,
                        meter_list.value400, gain400,
                        meter_list.value3000, gain3000,
                        meter_list.value10000, gain10000);
            sos_filter_begin(gain100, gain400, gain3000, gain3000);
        }
        delay(50);
    }
}
