#ifndef __fw_soft_typedef_h__
#define __fw_soft_typedef_h__

/* FreeRTOS头文件 */
#include "Arduino.h"


#define bFALSE 0
#define bTRUE 1


#define PNULL (unsigned char *)0
#define OneKB 1024UL
#define OneMB OneKB * 1024UL
#define OneGB OneMB * 1024UL
#define Block4KB (4 * OneKB)

/*-----------------------------------------------------------*/
// 宏定义
typedef void (*vvFunc)(void);				 //
typedef void (*vcFunc)(void);				 //
typedef void (*CharFunc)(unsigned char Dat); //

// 关键字
#define u64 uint64_t
#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

#define u8_pt u8 *

#define vu64 volatile uint64_t
#define vu32 volatile uint32_t
#define vu16 volatile uint16_t
#define vu8 volatile uint8_t

#define OK 0

#define OFF 0
#define ON 1

#define MaxTick 0XFFFFFFFFUL
/*-----------------------------------------------------------*/
/* 线程安全锁 互斥量*/
#define Using_thread_cslock

#define thread_cslock_t SemaphoreHandle_t	 // 安全锁类型
#define thread_cslock_Pt SemaphoreHandle_t * // 安全锁类型指针
#ifdef Using_thread_cslock
#define thread_cslock_WaitMax portMAX_DELAY
#define thread_cslock_init(cslock_name) xSemaphoreCreateMutex() // rt_mutex_create(cslock_name,RT_IPC_FLAG_PRIO)
#define thread_cslock_destroy(cslock) vSemaphoreDelete(cslock)	// rt_mutex_delete(cslock)
#define thread_cslock_test(cslock)
#define thread_cslock_lock(cslock, time) xSemaphoreTake(cslock, time) // rt_mutex_take(cslock,time);
#define thread_cslock_free(cslock) xSemaphoreGive(cslock)			  // rt_mutex_release(cslock)

// 在中断中使用互斥量没有意义

#endif
/*-----------------------------------------------------------*/
/* 线程安全队列 */
#define Using_thread_mq

#define thread_mq_t QueueHandle_t
#define thread_mq_Pt QueueHandle_t *
#ifdef Using_thread_mq
#define thread_mq_init(mq_name, OneSize, MqNum) xQueueCreate((UBaseType_t)MqNum, (UBaseType_t)OneSize)
#define thread_mq_destroy(mq) // rt_mq_delete(mq)
#define thread_mq_test(mq)
#define thread_mq_send(mq, sBuf, Size, time) xQueueSend(mq, sBuf, time);
#define thread_mq_recv(mq, rBuf, Size, time) xQueueReceive(mq, rBuf, time);

#define thread_mq_send_irq(mq, sBuf, Size, time)                \
	{                                                           \
		BaseType_t xHigherPriorityTaskWoken;                    \
		xHigherPriorityTaskWoken = pdFALSE;                     \
		xQueueSendFromISR(mq, sBuf, &xHigherPriorityTaskWoken); \
		/*	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);				*/  \
	}
#define thread_mq_recv_irq(mq, rBuf, Size, time)                   \
	{                                                              \
		BaseType_t xHigherPriorityTaskWoken;                       \
		xHigherPriorityTaskWoken = pdFALSE;                        \
		xQueueReceiveFromISR(mq, rBuf, &xHigherPriorityTaskWoken); \
	}

#endif

// 任务管理
#define thread_create(func, name, stack_size, argc, priority, tcb) \
	xTaskCreate((TaskFunction_t)func,                              \
				(const char *)name,                                \
				(uint32_t)stack_size / 4,                          \
				(void *)argc,                                      \
				(UBaseType_t)priority,                             \
				(TaskHandle_t *)tcb);

// 内存管理
#define mem_get_free() xPortGetFreeHeapSize()		   // 例如 int vfree = mem_get_free();
#define mem_alloc(size_bytes) pvPortMalloc(size_bytes) // eg. u8* buf = (u8*)mem_alloc(100);
#define mem_free(pv) vPortFree(pv)					   // eg. mem_free(buf);

#endif
