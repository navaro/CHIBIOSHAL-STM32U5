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
 * @file    TIMv1/stm32_tim2.inc
 * @brief   Shared TIM2 handler.
 *
 * @addtogroup STM32_TIM2_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(STM32_HAS_LPTIM1)
#error "STM32_HAS_LPTIM1 not defined in registry"
#endif

/* Driver checks for robustness, undefined USE macros are defaulted to
   FALSE. This makes this module independent from drivers implementation.*/
#if !defined(STM32_ST_USE_LPTIM1)
#define STM32_ST_USE_LPTIM1                   FALSE
#endif
#if !defined(STM32_ST_USE_LPTIM2)
#define STM32_ST_USE_LPTIM2                   FALSE
#endif


#if OSAL_LPST_MODE

/* Priority settings checks.*/
#if !defined(STM32_IRQ_LPTIM1_PRIORITY)
#error "STM32_IRQ_LPTIM1_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(STM32_IRQ_LPTIM1_PRIORITY)
#error "Invalid IRQ priority assigned to STM32_IRQ_LPTIM1_PRIORITY"
#endif

#endif /* STM32_HAS_LPTIM1 */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void lptim_irq_init(void) {
#if defined(OSAL_LPST_MODE)
#if STM32_ST_USE_LPTIM1 == TRUE
  nvicEnableVector(STM32_LPTIM1_NUMBER, STM32_IRQ_LPTIM1_PRIORITY);
#endif
#if STM32_ST_USE_LPTIM2 == TRUE
  nvicEnableVector(STM32_LPTIM2_NUMBER, STM32_IRQ_LPTIM2_PRIORITY);
#endif
#endif
}

static inline void lptim_irq_deinit(void) {
#if defined(OSAL_LPST_MODE)
#if STM32_ST_USE_LPTIM1 == TRUE
  nvicDisableVector(STM32_LPTIM1_NUMBER);
#endif
#if STM32_ST_USE_LPTIM2 == TRUE
  nvicDisableVector(STM32_LPTIM2_NUMBER);
#endif
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(OSAL_LPST_MODE) || defined(__DOXYGEN__)
/**
 * @brief   LPTIOM1 interrupt handler.
 *
 * @isr
 */
#if STM32_ST_USE_LPTIM1 == TRUE
OSAL_IRQ_HANDLER(STM32_LPTIM1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if 1
  st_lld_serve_interrupt();
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/**
 * @brief   LPTIOM1 interrupt handler.
 *
 * @isr
 */
#if STM32_ST_USE_LPTIM2 == TRUE
OSAL_IRQ_HANDLER(STM32_LPTIM2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if 1
  st_lld_serve_interrupt();
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */
