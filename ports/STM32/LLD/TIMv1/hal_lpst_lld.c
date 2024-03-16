/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    TIMv1/hal_st_lld.c
 * @brief   ST Driver subsystem low level driver code.
 *
 * @addtogroup ST
 * @{
 */

#include "hal.h"

#if OSAL_LPST_MODE && (OSAL_ST_MODE != OSAL_ST_MODE_NONE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING

#define ST_ARR_INIT                         0x0000FFFFU

#if STM32_ST_USE_LPTIM1 == TRUE
#define ST_HANDLER                          STM32_LPTIM_HANDLER
#define ST_NUMBER                           STM32_LPTIM1_NUMBER
#define ST_CLOCK_SRC                        STM32_LSE_CK_MIN
#define ST_ENABLE_CLOCK()                   rccEnableLPTIM1(true)
#define ST_DISABLE_CLOCK()                  rccDisableLPTIM1()
#define ST_ENABLE_STOP()                    DBGMCU->APB1LFZ1 |= DBGMCU_APB1LFZ1_DBG_LPTIM1

#define STM32_LPTIM							STM32_LPTIM1

#endif
#if STM32_ST_USE_LPTIM2 == TRUE
#define ST_HANDLER                          STM32_LPTIM2_HANDLER
#define ST_NUMBER                           STM32_LPTIM2_NUMBER
#define ST_CLOCK_SRC                        STM32_LSE_CK_MIN
#define ST_ENABLE_CLOCK()                   rccEnableLPTIM2(true)
#define ST_DISABLE_CLOCK()                  rccDisableLPTIM2()
#define ST_ENABLE_STOP()                    DBGMCU->APB4FZ1 |= DBGMCU_APB4FZ1_DBG_LPTIM2

#define STM32_LPTIM							STM32_LPTIM2

#endif

#if ST_CLOCK_SRC % OSAL_ST_FREQUENCY != 0
#error "the selected ST frequency is not obtainable because integer rounding"
#endif

#if (ST_CLOCK_SRC / OSAL_ST_FREQUENCY) - 1 > 0xFFFF
#error "the selected ST frequency is not obtainable because TIM timer prescaler limits"
#endif

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */


#define WAIT_ARR	do { while (!(STM32_LPTIM->ISR & LPTIM_ISR_ARROK)) ; \
					STM32_LPTIM->ICR &= LPTIM_ICR_ARRMCF ; } while(0)
#define WAIT_CMP	do { while (!(STM32_LPTIM->ISR & LPTIM_ISR_CMPOK)) ; \
					STM32_LPTIM->ICR &= LPTIM_ICR_CMPMCF ; } while(0)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#define ST_ALARM_ACTIVE					(1<<16)
#define ST_ALARM_UPDATING				(1<<17)
#define ST_ALARM_COMPLETING				(1<<18)
#define ST_ABSTIME_MASK 				0xFFFF
static volatile uint32_t				_hal_lpst_lld_abstime = 0 ;
static volatile uint16_t				_hal_lpst_lld_arr_cnt = 0 ;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ST driver initialization.
 *
 * @notapi
 */
void st_lld_init(void) {

  /* Free running counter mode.*/

  /* Enabling timer clock.*/
  ST_ENABLE_CLOCK();

  /* Enabling the stop mode during debug for this timer.*/
  ST_ENABLE_STOP();

  STM32_LPTIM->CR &= ~LPTIM_CR_ENABLE ;
  /* Initializing the counter in free running mode.*/
  STM32_LPTIM->CFGR    = 5 << LPTIM_CFGR_PRESC_Pos ; // presacale 32
  STM32_LPTIM->IER |= /*LPTIM_ISR_CMPM | LPTIM_ISR_CMPOK |*/ LPTIM_ISR_ARRM ;


  STM32_LPTIM->CR |= LPTIM_CR_ENABLE  ;
  (void)STM32_LPTIM->CR ;
  (void)STM32_LPTIM->CR ;

  STM32_LPTIM->ARR    = ST_ARR_INIT  ;
  WAIT_ARR ;
  STM32_LPTIM->CMP    = ST_ARR_INIT  ;
  WAIT_CMP ;


  STM32_LPTIM->CR |= LPTIM_CR_CNTSTRT ;
  (void)STM32_LPTIM->CR ;


  EXTI->EMR2 |= (1<<15) ;

  extiEnableGroup2 (EXTI_MASK2(47), EXTI_MODE_BOTH_EDGES|EXTI_MODE_ACTION_INTERRUPT);

  nvicEnableVector(ST_NUMBER, STM32_ST_IRQ_PRIORITY);

}

/**
 * @brief   IRQ handling code.
 */
void st_lld_serve_interrupt(void)
{
	uint32_t isr = STM32_LPTIM->ISR ;
	STM32_LPTIM->ICR = isr ;
	uint32_t handle = 0 ;

	if (_hal_lpst_lld_abstime&ST_ALARM_ACTIVE) {

		if (isr & LPTIM_ISR_CMPOK) {
			if (_hal_lpst_lld_abstime & ST_ALARM_UPDATING) {
				STM32_LPTIM->CMP = _hal_lpst_lld_abstime & ST_ABSTIME_MASK ;
				_hal_lpst_lld_abstime &= ~ST_ALARM_UPDATING ;
				_hal_lpst_lld_abstime |= ST_ALARM_COMPLETING ;

			} else if (_hal_lpst_lld_abstime & ST_ALARM_COMPLETING) {
				_hal_lpst_lld_abstime &= ~ST_ALARM_COMPLETING ;

 			}
			__DSB();
			__ISB();

		}

		if (isr & LPTIM_ISR_ARRM) {
			_hal_lpst_lld_arr_cnt++ ;
			if (ST_ARR_INIT == (_hal_lpst_lld_abstime & ST_ABSTIME_MASK)) {
				handle = 1 ;

			}

		}

		if (isr & LPTIM_ISR_CMPM) {
			handle = 1 ;

		}

		if (handle) {
			osalSysLockFromISR();
			osalOsTimerHandlerI();
			osalSysUnlockFromISR();

		}

	}

}

systime_t st_lld_get_counter(void)
{
	uint32_t cnt  ;
	do  { cnt = STM32_LPTIM->CNT ; }
	while (cnt != STM32_LPTIM->CNT) ;
	return  cnt ;
}

uint32_t st_lld_get_counter32(void)
{
	uint32_t cnt ;
	cnt =  (_hal_lpst_lld_arr_cnt << 16) | st_lld_get_counter() ;
	return cnt ;
}

void st_lld_start_alarm(systime_t abstime)
{
	_hal_lpst_lld_abstime = (abstime & ST_ABSTIME_MASK) | ST_ALARM_ACTIVE | ST_ALARM_COMPLETING ;
	STM32_LPTIM->ICR = LPTIM_ISR_CMPM | LPTIM_ISR_CMPOK ;
	STM32_LPTIM->IER |= (LPTIM_ISR_CMPOK|LPTIM_ISR_CMPM) ;
	STM32_LPTIM->CMP    = abstime ;
	__DSB();
	__ISB();
}

void st_lld_stop_alarm(void)
{
	if (_hal_lpst_lld_abstime & (ST_ALARM_COMPLETING|ST_ALARM_UPDATING)) {
		while (!(STM32_LPTIM->ISR & LPTIM_ISR_CMPOK)) ;
	}
	STM32_LPTIM->IER &= ~(LPTIM_ISR_CMPOK|LPTIM_ISR_CMPM) ;
	STM32_LPTIM->ICR = LPTIM_ISR_CMPOK|LPTIM_ISR_CMPM ;

	_hal_lpst_lld_abstime = 0 ;
	__DSB();
	__ISB();
}

void st_lld_set_alarm(systime_t abstime)
{
	// osalDbgAssert(abstime != ST_ARR_INIT, "st_lld_set_alarm");
	if (_hal_lpst_lld_abstime & (ST_ALARM_COMPLETING|ST_ALARM_UPDATING)) {
		_hal_lpst_lld_abstime = (abstime & ST_ABSTIME_MASK) | ST_ALARM_ACTIVE | ST_ALARM_UPDATING ;
	} else {
		_hal_lpst_lld_abstime = (abstime & ST_ABSTIME_MASK) | ST_ALARM_ACTIVE | ST_ALARM_COMPLETING ;
		STM32_LPTIM->CMP    = abstime ;
	}
	__DSB();
	__ISB();
}

systime_t st_lld_get_alarm(void)
{
	return STM32_LPTIM->CMP ;
}

bool st_lld_is_alarm_active(void)
{
	return (_hal_lpst_lld_abstime & ST_ALARM_ACTIVE) ?  1 : 0 ;
}


void st_lld_halt(void)
{
	st_lld_stop_alarm() ;
	ST_DISABLE_CLOCK() ;
}



#endif /* OSAL_ST_MODE != OSAL_ST_MODE_NONE */

/** @} */
