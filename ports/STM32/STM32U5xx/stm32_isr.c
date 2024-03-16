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
 * @file    STM32H7xx/stm32_isr.c
 * @brief   STM32H7xx ISR handler code.
 *
 * @addtogroup STM32H7xx_ISR
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define exti_serve_irq(pr, channel) {                                       \
                                                                            \
  if ((pr) & (1U << (channel))) {                                           \
    _pal_isr_code(channel);                                                 \
  }                                                                         \
}

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#include "stm32_exti.h"
#include "stm32_exti0.inc"
#include "stm32_exti1.inc"
#include "stm32_exti2.inc"
#include "stm32_exti3.inc"
#include "stm32_exti4.inc"
#include "stm32_exti5.inc"
#include "stm32_exti6.inc"
#include "stm32_exti7.inc"
#include "stm32_exti8.inc"
#include "stm32_exti9.inc"
#include "stm32_exti10.inc"
#include "stm32_exti11.inc"
#include "stm32_exti12.inc"
#include "stm32_exti13.inc"
#include "stm32_exti14.inc"
#include "stm32_exti15.inc"


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables IRQ sources.
 *
 * @notapi
 */
void irqInit(void) {

  exti0_irq_init();
  exti1_irq_init();
  exti2_irq_init();
  exti3_irq_init();
  exti4_irq_init();
  exti5_irq_init();
  exti6_irq_init();
  exti7_irq_init();
  exti8_irq_init();
  exti9_irq_init();
  exti10_irq_init();
  exti11_irq_init();
  exti12_irq_init();
  exti13_irq_init();
  exti14_irq_init();
  exti15_irq_init();

 }

/**
 * @brief   Disables IRQ sources.
 *
 * @notapi
 */
void irqDeinit(void) {

  exti0_irq_deinit();
  exti1_irq_deinit();
  exti2_irq_deinit();
  exti3_irq_deinit();
  exti4_irq_deinit();
  exti5_irq_deinit();
  exti6_irq_deinit();
  exti7_irq_deinit();
  exti8_irq_deinit();
  exti9_irq_deinit();
  exti10_irq_deinit();
  exti11_irq_deinit();
  exti12_irq_deinit();
  exti13_irq_deinit();
  exti14_irq_deinit();
  exti15_irq_deinit();
 }

/** @} */
