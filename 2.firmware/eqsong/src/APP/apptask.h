#if !defined(USER_TASK_H)
#define USER_TASK_H

// 几个任务存放
#include "BSP/bsp.h"

extern void create_listen_thread();
extern void create_meter_thread();

extern void listen_meter_coroutine();
#endif // USER_TASK_H
