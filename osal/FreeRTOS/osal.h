/*
    Copyright (C) 2015-2024, Navaro, All Rights Reserved
    SPDX-License-Identifier: MIT

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/


/**
 * @file    osal.h
 * @brief   OSAL module header.
 *
 * @addtogroup OSAL
 * @{
 */

#ifndef OSAL_H
#define OSAL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "halconf.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "../../../debug.h"


/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Common constants
 * @{
 */
#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE                               0
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE                                (1)
#endif

#define OSAL_SUCCESS                        FALSE
#define OSAL_FAILED                         TRUE
/** @} */

#if 1
/**
 * @name    Messages
 * @{
 */
#define MSG_OK                              0 // RDY_OK
#define MSG_RESET                           -2 // RDY_RESET
#define MSG_TIMEOUT                         -1 // RDY_TIMEOUT
#define MSG_NAK                         	-3 // NAK
/** @} */
#endif

#if 1
/**
 * @name    Special time constants
 * @{
 */
#define TIME_IMMEDIATE                      ((sysinterval_t)0)
#define TIME_INFINITE                       ((sysinterval_t)-1)
/** @} */
#endif

/**
 * @name    Systick modes.
 * @{
 */
#define OSAL_ST_MODE_NONE                   0
#define OSAL_ST_MODE_PERIODIC               1
#define OSAL_ST_MODE_FREERUNNING            2
/** @} */

/**
 * @name    Systick parameters.
 * @{
 */
/**
 * @brief   Size in bits of the @p systick_t type.
 */
#define OSAL_ST_RESOLUTION                  32

/**
 * @brief   Required systick frequency or resolution.
 */
#define OSAL_ST_FREQUENCY                   configTICK_RATE_HZ

/**
 * @brief   Systick mode required by the underlying OS.
 */
#define OSAL_ST_MODE                        OSAL_ST_MODE_NONE
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/



#if (OSAL_ST_RESOLUTION != 16) && (OSAL_ST_RESOLUTION != 32)
#error "invalid OSAL_ST_RESOLUTION, must be 16 or 32"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

extern volatile uint32_t _osal_base_prio ;
extern BaseType_t _osal_xHigherPriorityTaskWoken  ;

#if 1
/**
 * @brief   Type of a system status word.
 */
typedef uint32_t syssts_t;
#endif

#if 1
/**
 * @brief   Type of a message.
 */
typedef int32_t msg_t;
#endif

#if 1
/**
 * @brief   Type of system time counter.
 */
typedef uint32_t systime_t;
#endif

#if 1
/**
 * @brief   Type of system time interval.
 */
typedef uint32_t sysinterval_t;
#endif

#if 1
/**
 * @brief   Type of realtime counter.
 */
typedef uint32_t rtcnt_t;
#endif

#if 1
/**
 * @brief   Type of a thread reference.
 */
typedef void * thread_reference_t;
#endif

#if 1
/**
 * @brief   Type of an event flags mask.
 */
typedef uint32_t eventflags_t;
#endif

#if 1 // !CH_CFG_USE_EVENTS
/**
 * @brief   Type of an event flags object.
 * @note    The content of this structure is not part of the API and should
 *          not be relied upon. Implementers may define this structure in
 *          an entirely different way.
 * @note    Retrieval and clearing of the flags are not defined in this
 *          API and are implementation-dependent.
 */
//typedef struct {
//  volatile eventflags_t flags;      /**< @brief Flags stored into the
//                                                object.                     */
//} event_source_t;
typedef StaticEventGroup_t event_source_t ;
#endif

/**
 * @brief   Type of a mutex.
 * @note    If the OS does not support mutexes or there is no OS then the
 *          mechanism can be simulated.
 */
typedef StaticQueue_t mutex_t;


#if 1
/**
 * @brief   Type of a thread queue.
 * @details A thread queue is a queue of sleeping threads, queued threads
 *          can be dequeued one at time or all together.
 * @note    In this implementation it is implemented as a single reference
 *          because there are no real threads.
 */
typedef StaticQueue_t threads_queue_t;
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Debug related macros
 * @{
 */
/**
 * @brief   Condition assertion.
 * @details If the condition check fails then the OSAL panics with a
 *          message and halts.
 * @note    The condition is tested only if the @p OSAL_ENABLE_ASSERTIONS
 *          switch is enabled.
 * @note    The remark string is not currently used except for putting a
 *          comment in the code about the assertion.
 *
 * @param[in] c         the condition to be verified to be true
 * @param[in] remark    a remark string
 *
 * @api
 */
#define osalDbgAssert(c, remark) DBG_ASSERT_T((c), remark)

/**
 * @brief   Function parameters check.
 * @details If the condition check fails then the OSAL panics and halts.
 * @note    The condition is tested only if the @p OSAL_ENABLE_CHECKS switch
 *          is enabled.
 *
 * @param[in] c         the condition to be verified to be true
 *
 * @api
 */
#define osalDbgCheck(c) 	DBG_ASSERT_T(c, __FUNCTION__)

/**
 * @brief   I-Class state check.
 * @note    Not implemented in this simplified OSAL.
 */
#define osalDbgCheckClassI() // chDbgCheckClassI()

/**
 * @brief   S-Class state check.
 * @note    Not implemented in this simplified OSAL.
 */
#define osalDbgCheckClassS() // chDbgCheckClassS()
/** @} */

/**
 * @name    IRQ service routines wrappers
 * @{
 */
/**
 * @brief   Priority level verification macro.
 */
#define OSAL_IRQ_IS_VALID_PRIORITY(n) 1

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers.
 */
#define OSAL_IRQ_PROLOGUE() \
		_osal_xHigherPriorityTaskWoken = 0 ; \
		// CH_IRQ_PROLOGUE()

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers.
 */
#define OSAL_IRQ_EPILOGUE() \
		portYIELD_FROM_ISR( _osal_xHigherPriorityTaskWoken ); \
		// CH_IRQ_EPILOGUE()

/**
 * @brief   IRQ handler function declaration.
 * @details This macro hides the details of an ISR function declaration.
 *
 * @param[in] id        a vector name as defined in @p vectors.s
 */
#define OSAL_IRQ_HANDLER(id) void id(void)
/** @} */

/**
 * @name    Time conversion utilities
 * @{
 */
/**
 * @brief   Seconds to system ticks.
 * @details Converts from seconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] secs      number of seconds
 * @return              The number of ticks.
 *
 * @api
 */
#define OSAL_S2I(sec) ((systime_t)((uint32_t)(sec) * (uint32_t)OSAL_ST_FREQUENCY))

/**
 * @brief   Milliseconds to system ticks.
 * @details Converts from milliseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msecs     number of milliseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define OSAL_MS2I(msec) ((systime_t)(((((uint32_t)(msec)) *    \
        ((uint32_t)OSAL_ST_FREQUENCY)) + 999UL) / 1000UL))

/**
 * @brief   Microseconds to system ticks.
 * @details Converts from microseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usecs     number of microseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define OSAL_US2I(usec) ((systime_t)(((((uint32_t)(usec)) *   \
        ((uint32_t)OSAL_ST_FREQUENCY)) + 999999UL) / 1000000UL))

/** @} */

/**
 * @name    Time conversion utilities for the realtime counter
 * @{
 */
/**
 * @brief   Seconds to realtime counter.
 * @details Converts from seconds to realtime counter cycles.
 * @note    The macro assumes that @p freq >= @p 1.
 *
 * @param[in] freq      clock frequency, in Hz, of the realtime counter
 * @param[in] sec       number of seconds
 * @return              The number of cycles.
 *
 * @api
 */
#define OSAL_S2RTC(freq, sec) S2RTC(freq, sec)

/**
 * @brief   Milliseconds to realtime counter.
 * @details Converts from milliseconds to realtime counter cycles.
 * @note    The result is rounded upward to the next millisecond boundary.
 * @note    The macro assumes that @p freq >= @p 1000.
 *
 * @param[in] freq      clock frequency, in Hz, of the realtime counter
 * @param[in] msec      number of milliseconds
 * @return              The number of cycles.
 *
 * @api
 */
#define OSAL_MS2RTC(freq, msec) MS2RTC(freq, msec)

/**
 * @brief   Microseconds to realtime counter.
 * @details Converts from microseconds to realtime counter cycles.
 * @note    The result is rounded upward to the next microsecond boundary.
 * @note    The macro assumes that @p freq >= @p 1000000.
 *
 * @param[in] freq      clock frequency, in Hz, of the realtime counter
 * @param[in] usec      number of microseconds
 * @return              The number of cycles.
 *
 * @api
 */
#define OSAL_US2RTC(freq, usec) US2RTC(freq, usec)
/** @} */

/**
 * @name    Sleep macros using absolute time
 * @{
 */
/**
 * @brief   Delays the invoking thread for the specified number of seconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] secs      time in seconds, must be different from zero
 *
 * @api
 */
#define osalThreadSleepSeconds(sec) vTaskDelay(OSAL_S2I(sec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          milliseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] msecs     time in milliseconds, must be different from zero
 *
 * @api
 */
#define osalThreadSleepMilliseconds(msec) vTaskDelay(OSAL_MS2I(msec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          microseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system tick clock.
 * @note    The maximum specifiable value is implementation dependent.
 *
 * @param[in] usecs     time in microseconds, must be different from zero
 *
 * @api
 */
#define osalThreadSleepMicroseconds(usec) vTaskDelay(OSAL_US2I(usec))
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

 void chSysPolledDelayX(rtcnt_t cycles) ;

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   OSAL module initialization.
 *
 * @api
 */
static inline void osalInit(void) {
	  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

}

/**
 * @brief   System halt with error message.
 *
 * @param[in] reason    the halt message pointer
 *
 * @api
 */
static inline void osalSysHalt(const char *reason) {


	dbg_assert(reason);
}

/**
 * @brief   Disables interrupts globally.
 *
 * @special
 */
static inline void osalSysDisable(void) {

	portDISABLE_INTERRUPTS();
}

/**
 * @brief   Enables interrupts globally.
 *
 * @special
 */
static inline void osalSysEnable(void) {

	portENABLE_INTERRUPTS();
}

/**
 * @brief   Enters a critical zone from thread context.
 * @note    This function cannot be used for reentrant critical zones.
 *
 * @special
 */
static inline void osalSysLock(void) {

	portENTER_CRITICAL();
	xTaskNotifyStateClear (xTaskGetCurrentTaskHandle ()) ;
	//ulTaskNotifyTakeS (pdTRUE, 0) ;
}

/**
 * @brief   Leaves a critical zone from thread context.
 * @note    This function cannot be used for reentrant critical zones.
 *
 * @special
 */
static inline void osalSysUnlock(void) {

	taskEXIT_CRITICAL();
}

/**
 * @brief   Enters a critical zone from ISR context.
 * @note    This function cannot be used for reentrant critical zones.
 *
 * @special
 */
static inline void osalSysLockFromISR(void) {

	_osal_base_prio = taskENTER_CRITICAL_FROM_ISR();
	// _osal_xHigherPriorityTaskWoken = 0 ; // ToDo: move to OSAL_IRQ_PROLOGUE()
}

/**
 * @brief   Leaves a critical zone from ISR context.
 * @note    This function cannot be used for reentrant critical zones.
 *
 * @special
 */
static inline void osalSysUnlockFromISR(void) {

	BaseType_t xHigherPriorityTaskWoke = _osal_xHigherPriorityTaskWoken ;
	taskEXIT_CRITICAL_FROM_ISR(_osal_base_prio);
	portYIELD_FROM_ISR( xHigherPriorityTaskWoke ); // ToDo: move to OSAL_IRQ_EPILOGUE()
}

/**
 * @brief   Returns the execution status and enters a critical zone.
 * @details This functions enters into a critical zone and can be called
 *          from any context. Because its flexibility it is less efficient
 *          than @p chSysLock() which is preferable when the calling context
 *          is known.
 * @post    The system is in a critical zone.
 *
 * @return              The previous system status, the encoding of this
 *                      status word is architecture-dependent and opaque.
 *
 * @xclass
 */
static inline syssts_t osalSysGetStatusAndLockX(void) {
	    if ( (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0) {
	    	__asm volatile ("cpsid i");
	        return 1;
	    }
	    else {
	    	taskENTER_CRITICAL ();
	        return 0;
	    }
}


/**
 * @brief   Restores the specified execution status and leaves a critical zone.
 * @note    A call to @p chSchRescheduleS() is automatically performed
 *          if exiting the critical zone and if not in ISR context.
 *
 * @param[in] sts       the system status to be restored.
 *
 * @xclass
 */
static inline void osalSysRestoreStatusX(syssts_t sts) {
  if (sts) {
	  __asm volatile ("cpsie i");
  } else {
	  taskEXIT_CRITICAL ();
  }
}


/**
 * @brief   Polled delay.
 * @note    The real delay is always few cycles in excess of the specified
 *          value.
 *
 * @param[in] cycles    number of cycles
 *
 * @xclass
 */
#if 1 // PORT_SUPPORTS_RT || defined(__DOXYGEN__)
static inline void osalSysPolledDelayX(rtcnt_t cycles) {

  chSysPolledDelayX(cycles);
}
#endif

/**
 * @brief   Systick callback for the underlying OS.
 * @note    This callback is only defined if the OSAL requires such a
 *          service from the HAL.
 */
#if (OSAL_ST_MODE != OSAL_ST_MODE_NONE) || defined(__DOXYGEN__)
static inline void osalOsTimerHandlerI(void) {

  //chSysTimerHandlerI();
}
#endif

/**
 * @brief   Checks if a reschedule is required and performs it.
 * @note    I-Class functions invoked from thread context must not reschedule
 *          by themselves, an explicit reschedule using this function is
 *          required in this scenario.
 * @note    Not implemented in this simplified OSAL.
 *
 * @sclass
 */
static inline void osalOsRescheduleS(void) {

////!!  chSchRescheduleS();
}

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p osalInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 * @note    This function can be called from any context but its atomicity
 *          is not guaranteed on architectures whose word size is less than
 *          @p systime_t size.
 *
 * @return              The system time in ticks.
 *
 * @xclass
 */
static inline systime_t osalOsGetSystemTimeX(void) {

  return xTaskGetTickCount();
}

/**
 * @brief   Adds an interval to a system time returning a system time.
 *
 * @param[in] systime   base system time
 * @param[in] interval  interval to be added
 * @return              The new system time.
 *
 * @xclass
 */
static inline systime_t osalTimeAddX(systime_t systime,
                                     sysinterval_t interval) {

  return systime + interval;
}

/**
 * @brief   Subtracts two system times returning an interval.
 *
 * @param[in] start     first system time
 * @param[in] end       second system time
 * @return              The interval representing the time difference.
 *
 * @xclass
 */
static inline sysinterval_t osalTimeDiffX(systime_t start, systime_t end) {

  return  end - start;
}

/**
 * @brief   Checks if the specified time is within the specified time window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 * @note    This function can be called from any context.
 *
 * @param[in] time      the time to be verified
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @xclass
 */
static inline bool osalTimeIsInRangeX(systime_t time,
                                      systime_t start,
                                      systime_t end) {

  return (bool)((systime_t)(time - start) < (systime_t)(end - start)) ;
}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] delay     the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 *
 * @sclass
 */
static inline void osalThreadSleepS(systime_t time) {
	osalSysUnlock() ;
	vTaskDelay(time);
	osalSysLock() ;
}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] delay     the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 *
 * @api
 */
static inline void osalThreadSleep(sysinterval_t delay) {

	vTaskDelay(delay);
}

/**
 * @brief   Sends the current thread sleeping and sets a reference variable.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @return              The wake up message.
 *
 * @sclass
 */
static inline msg_t osalThreadSuspendS(thread_reference_t *trp) {

   //vTaskSuspend(*trp);
	*trp = xTaskGetCurrentTaskHandle () ;
	msg_t msg = -1 ;
	//osalSysUnlock () ;
	xTaskNotifyWaitS( (uint32_t) -1, (uint32_t) -1, (uint32_t *)&msg, TIME_INFINITE ) ;

	//msg_t msg =	ulTaskNotifyTakeS( pdTRUE, TIME_INFINITE )  ;
	//osalSysLock () ;
	return msg ;
}

/**
 * @brief   Sends the current thread sleeping and sets a reference variable.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] timeout   the timeout in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE the thread is not enqueued and
 *                        the function returns @p MSG_TIMEOUT as if a timeout
 *                        occurred.
 *                      .
 * @return              The wake up message.
 * @retval MSG_TIMEOUT  if the operation timed out.
 *
 * @sclass
 */
static inline msg_t osalThreadSuspendTimeoutS(thread_reference_t *trp,
                                              sysinterval_t timeout) {
//#warning "osalThreadSuspendTimeoutS not implemented!"
	//osalSysUnlock () ;
	*trp = xTaskGetCurrentTaskHandle () ;
	msg_t msg = -1 ; //=  ulTaskNotifyTakeS( pdTRUE, timeout ) ;
	if (xTaskNotifyWaitS( (uint32_t) -1, (uint32_t) -1, (uint32_t *)&msg, timeout ) == pdFALSE) {
		msg = MSG_TIMEOUT ;
	}

	//osalSysLock () ;
	return msg ;
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must not reschedule because it can be called from
 *          ISR context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @iclass
 */
static inline void osalThreadResumeI(thread_reference_t *trp, msg_t msg) {

	xTaskNotifyFromISR ( *trp, msg, eSetValueWithOverwrite, &_osal_xHigherPriorityTaskWoken );
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @iclass
 */
static inline void osalThreadResumeS(thread_reference_t *trp, msg_t msg) {

	xTaskNotify( *trp, msg, eSetValueWithOverwrite );
}

/**
 * @brief   Initializes a threads queue object.
 *
 * @param[out] tqp      pointer to the threads queue object
 *
 * @init
 */
static inline void osalThreadQueueObjectInit(threads_queue_t *tqp) {

	xSemaphoreCreateCountingStatic ((UBaseType_t)-1, 0, (StaticSemaphore_t *)(tqp) ) ;
}

/**
 * @brief   Enqueues the caller thread.
 * @details The caller thread is enqueued and put to sleep until it is
 *          dequeued or the specified timeouts expires.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] timeout   the timeout in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE the thread is not enqueued and
 *                        the function returns @p MSG_TIMEOUT as if a timeout
 *                        occurred.
 *                      .
 * @return              The message from @p osalQueueWakeupOneI() or
 *                      @p osalQueueWakeupAllI() functions.
 * @retval MSG_TIMEOUT  if the thread has not been dequeued within the
 *                      specified timeout or if the function has been
 *                      invoked with @p TIME_IMMEDIATE as timeout
 *                      specification.
 *
 * @sclass
 */
static inline msg_t osalThreadEnqueueTimeoutS(threads_queue_t *tqp,
                                              sysinterval_t timeout) {

	osalSysUnlock () ;
	uint32_t r = xSemaphoreTake( (SemaphoreHandle_t)tqp, timeout ) == pdTRUE ? MSG_OK : MSG_TIMEOUT ;
	osalSysLock () ;
	return r ;
}

/**
 * @brief   Dequeues and wakes up one thread from the queue, if any.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
static inline void osalThreadDequeueNextI(threads_queue_t *tqp, msg_t msg) {
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(tqp, &_osal_xHigherPriorityTaskWoken );
}

/**
 * @brief   Dequeues and wakes up all threads from the queue.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
static inline void osalThreadDequeueAllI(threads_queue_t *tqp, msg_t msg) {
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	while (!uxSemaphoreGetCount((SemaphoreHandle_t)tqp)) {
		xSemaphoreGiveFromISR(tqp, &_osal_xHigherPriorityTaskWoken );
	}
}

/**
 * @brief   Initializes an event flags object.
 *
 * @param[out] esp      pointer to the event flags object
 *
 * @init
 */

static inline void osalEventObjectInit(event_source_t *esp) {

	xEventGroupCreateStatic (esp) ;
}


/**
 * @brief   Add flags to an event source object.
 *
 * @param[in] esp       pointer to the event flags object
 * @param[in] flags     flags to be ORed to the flags mask
 *
 * @iclass
 */
static inline void osalEventBroadcastFlagsI(event_source_t *esp,
                                            eventflags_t flags) {
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;

     /* Set bit 0 and bit 4 in xEventGroup. */
     xEventGroupSetBitsFromISR((EventGroupHandle_t)esp,
			  flags, /* The bits being set. */
             &_osal_xHigherPriorityTaskWoken );
}


/**
 * @brief   Add flags to an event source object.
 *
 * @param[in] esp       pointer to the event flags object
 * @param[in] flags     flags to be ORed to the flags mask
 *
 * @iclass
 */
static inline void osalEventBroadcastFlags(event_source_t *esp,
                                            eventflags_t flags) {

	xEventGroupSetBits( (EventGroupHandle_t)esp,
			   flags );

}


/**
 * @brief   Initializes s @p mutex_t object.
 *
 * @param[out] mp       pointer to the @p mutex_t object
 *
 * @init
 */
static inline void osalMutexObjectInit(mutex_t *mp) {


	xSemaphoreCreateRecursiveMutexStatic((StaticSemaphore_t *)(mp) ) ;

}

/**
 * @brief   Locks the specified mutex.
 * @post    The mutex is locked and inserted in the per-thread stack of owned
 *          mutexes.
 *
 * @param[in,out] mp    pointer to the @p mutex_t object
 *
 * @api
 */
static inline void osalMutexLock(mutex_t *mp) {
	xSemaphoreTakeRecursive( (SemaphoreHandle_t) mp,
			portMAX_DELAY  );
}

/**
 * @brief   Unlocks the specified mutex.
 * @note    The HAL guarantees to release mutex in reverse lock order. The
 *          mutex being unlocked is guaranteed to be the last locked mutex
 *          by the invoking thread.
 *          The implementation can rely on this behavior and eventually
 *          ignore the @p mp parameter which is supplied in order to support
 *          those OSes not supporting a stack of the owned mutexes.
 *
 * @param[in,out] mp    pointer to the @p mutex_t object
 *
 * @api
 */
static inline void osalMutexUnlock(mutex_t *mp) {
	xSemaphoreGiveRecursive( (SemaphoreHandle_t) mp );
}

#endif /* _OSAL_H_ */

/** @} */
