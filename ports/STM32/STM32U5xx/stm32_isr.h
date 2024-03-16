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
 * @file    STM32H7xx/stm32_isr.h
 * @brief   STM32H7xx ISR handler header.
 *
 * @addtogroup STM32H7xx_ISR
 * @{
 */

#ifndef STM32_ISR_H
#define STM32_ISR_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    ISRs suppressed in standard drivers
 * @{
 */
#define STM32_TIM1_SUPPRESS_ISR
#define STM32_TIM2_SUPPRESS_ISR
//#define STM32_TIM3_SUPPRESS_ISR
#define STM32_TIM4_SUPPRESS_ISR
#define STM32_TIM5_SUPPRESS_ISR
#define STM32_TIM6_SUPPRESS_ISR
#define STM32_TIM7_SUPPRESS_ISR
#define STM32_TIM8_SUPPRESS_ISR
#define STM32_TIM12_SUPPRESS_ISR
#define STM32_TIM13_SUPPRESS_ISR
#define STM32_TIM14_SUPPRESS_ISR
#define STM32_TIM15_SUPPRESS_ISR
#define STM32_TIM16_SUPPRESS_ISR
#define STM32_TIM17_SUPPRESS_ISR

// #define STM32_USART1_SUPPRESS_ISR
#define STM32_USART2_SUPPRESS_ISR  // used combined for serial and uart driver
//#define STM32_USART3_SUPPRESS_ISR
// #define STM32_UART4_SUPPRESS_ISR
//#define STM32_UART5_SUPPRESS_ISR
#define STM32_USART6_SUPPRESS_ISR
#define STM32_UART7_SUPPRESS_ISR
#define STM32_UART8_SUPPRESS_ISR
//#define STM32_LPUART1_SUPPRESS_ISR
/** @} */

/**
 * @name    ISR names and numbers
 * @{
 */
/*
 * ADC units.
 */
#define STM32_ADC12_HANDLER                 Vector88
#define STM32_ADC3_HANDLER                  Vector23C

#define STM32_ADC12_NUMBER                  18
#define STM32_ADC3_NUMBER                   127

/*
 * BDMA units.
 */
#define STM32_BDMA1_CH0_HANDLER             Vector244
#define STM32_BDMA1_CH1_HANDLER             Vector248
#define STM32_BDMA1_CH2_HANDLER             Vector24C
#define STM32_BDMA1_CH3_HANDLER             Vector250
#define STM32_BDMA1_CH4_HANDLER             Vector254
#define STM32_BDMA1_CH5_HANDLER             Vector258
#define STM32_BDMA1_CH6_HANDLER             Vector25C
#define STM32_BDMA1_CH7_HANDLER             Vector260

#define STM32_BDMA1_CH0_NUMBER              129
#define STM32_BDMA1_CH1_NUMBER              130
#define STM32_BDMA1_CH2_NUMBER              131
#define STM32_BDMA1_CH3_NUMBER              132
#define STM32_BDMA1_CH4_NUMBER              133
#define STM32_BDMA1_CH5_NUMBER              134
#define STM32_BDMA1_CH6_NUMBER              135
#define STM32_BDMA1_CH7_NUMBER              136

/*
 * DMA units.
 */
#define STM32_DMA1_CH0_HANDLER              GPDMA1_Channel0_IRQHandler
#define STM32_DMA1_CH1_HANDLER              GPDMA1_Channel1_IRQHandler
#define STM32_DMA1_CH2_HANDLER              GPDMA1_Channel2_IRQHandler
#define STM32_DMA1_CH3_HANDLER              GPDMA1_Channel3_IRQHandler
#define STM32_DMA1_CH4_HANDLER              GPDMA1_Channel4_IRQHandler
#define STM32_DMA1_CH5_HANDLER              GPDMA1_Channel5_IRQHandler
#define STM32_DMA1_CH6_HANDLER              GPDMA1_Channel6_IRQHandler
#define STM32_DMA1_CH7_HANDLER              GPDMA1_Channel7_IRQHandler
#define STM32_DMA2_CH0_HANDLER              GPDMA1_Channel8_IRQHandler
#define STM32_DMA2_CH1_HANDLER              GPDMA1_Channel9_IRQHandler
#define STM32_DMA2_CH2_HANDLER              GPDMA1_Channel10_IRQHandler
#define STM32_DMA2_CH3_HANDLER              GPDMA1_Channel11_IRQHandler
#define STM32_DMA2_CH4_HANDLER              GPDMA1_Channel12_IRQHandler
#define STM32_DMA2_CH5_HANDLER              GPDMA1_Channel13_IRQHandler
#define STM32_DMA2_CH6_HANDLER              GPDMA1_Channel14_IRQHandler
#define STM32_DMA2_CH7_HANDLER              GPDMA1_Channel15_IRQHandler

#define STM32_DMA1_CH0_NUMBER               29
#define STM32_DMA1_CH1_NUMBER               30
#define STM32_DMA1_CH2_NUMBER               31
#define STM32_DMA1_CH3_NUMBER               32
#define STM32_DMA1_CH4_NUMBER               33
#define STM32_DMA1_CH5_NUMBER               34
#define STM32_DMA1_CH6_NUMBER               35
#define STM32_DMA1_CH7_NUMBER               36
#define STM32_DMA2_CH0_NUMBER               0
#define STM32_DMA2_CH1_NUMBER               0
#define STM32_DMA2_CH2_NUMBER               0
#define STM32_DMA2_CH3_NUMBER               0
#define STM32_DMA2_CH4_NUMBER               0
#define STM32_DMA2_CH5_NUMBER               0
#define STM32_DMA2_CH6_NUMBER               0
#define STM32_DMA2_CH7_NUMBER               0

/*
 * MDMA units.
 */
#define STM32_MDMA_HANDLER                  Vector228

#define STM32_MDMA_NUMBER                   122

/*
 * ETH units.
 */
#define STM32_ETH_HANDLER                   Vector134

#define STM32_ETH_NUMBER                    61

/*
 * EXTI units.
 */
#define STM32_EXTI0_HANDLER                 EXTI0_IRQHandler
#define STM32_EXTI1_HANDLER                 EXTI1_IRQHandler
#define STM32_EXTI2_HANDLER                 EXTI2_IRQHandler
#define STM32_EXTI3_HANDLER                 EXTI3_IRQHandler
#define STM32_EXTI4_HANDLER                 EXTI4_IRQHandler
#define STM32_EXTI5_HANDLER                 EXTI5_IRQHandler
#define STM32_EXTI6_HANDLER                 EXTI6_IRQHandler
#define STM32_EXTI7_HANDLER                 EXTI7_IRQHandler
#define STM32_EXTI8_HANDLER                 EXTI8_IRQHandler
#define STM32_EXTI9_HANDLER                 EXTI9_IRQHandler
#define STM32_EXTI10_HANDLER                EXTI10_IRQHandler
#define STM32_EXTI11_HANDLER                EXTI11_IRQHandler
#define STM32_EXTI12_HANDLER                EXTI12_IRQHandler
#define STM32_EXTI13_HANDLER                EXTI13_IRQHandler
#define STM32_EXTI14_HANDLER                EXTI14_IRQHandler
#define STM32_EXTI15_HANDLER                EXTI15_IRQHandler


#define STM32_EXTI0_NUMBER                  11
#define STM32_EXTI1_NUMBER                  12
#define STM32_EXTI2_NUMBER                  13
#define STM32_EXTI3_NUMBER                  14
#define STM32_EXTI4_NUMBER                  15
#define STM32_EXTI5_NUMBER                  16
#define STM32_EXTI6_NUMBER                  17
#define STM32_EXTI7_NUMBER                  18
#define STM32_EXTI8_NUMBER                  19
#define STM32_EXTI9_NUMBER                  20
#define STM32_EXTI10_NUMBER                 21
#define STM32_EXTI11_NUMBER                 22
#define STM32_EXTI12_NUMBER                 23
#define STM32_EXTI13_NUMBER                 24
#define STM32_EXTI14_NUMBER                 25
#define STM32_EXTI15_NUMBER                 26



/*
 * FDCAN units.
 */
#define STM32_FDCAN1_IT0_HANDLER            Vector8C
#define STM32_FDCAN1_IT1_HANDLER            Vector94
#define STM32_FDCAN2_IT0_HANDLER            Vector90
#define STM32_FDCAN2_IT1_HANDLER            Vector98

#define STM32_FDCAN1_IT0_NUMBER             19
#define STM32_FDCAN1_IT1_NUMBER             21
#define STM32_FDCAN2_IT0_NUMBER             20
#define STM32_FDCAN2_IT1_NUMBER             22

/*
 * I2C units.
 */
#define STM32_I2C1_EVENT_HANDLER            I2C1_EV_IRQHandler
#define STM32_I2C1_ERROR_HANDLER            I2C1_ER_IRQHandler
#define STM32_I2C2_EVENT_HANDLER            I2C2_EV_IRQHandler
#define STM32_I2C2_ERROR_HANDLER            I2C2_ER_IRQHandler
#define STM32_I2C3_EVENT_HANDLER            Vector160
#define STM32_I2C3_ERROR_HANDLER            Vector164
#define STM32_I2C4_EVENT_HANDLER            Vector1BC
#define STM32_I2C4_ERROR_HANDLER            Vector1C0

#define STM32_I2C1_EVENT_NUMBER             55
#define STM32_I2C1_ERROR_NUMBER             56
#define STM32_I2C2_EVENT_NUMBER             57
#define STM32_I2C2_ERROR_NUMBER             58
#define STM32_I2C3_EVENT_NUMBER             0
#define STM32_I2C3_ERROR_NUMBER             0
#define STM32_I2C4_EVENT_NUMBER             0
#define STM32_I2C4_ERROR_NUMBER             0

/*
 * QUADSPI units.
 */
#define STM32_QUADSPI1_HANDLER              Vector1B0

#define STM32_QUADSPI1_NUMBER               92

/*
 * SDMMC units.
 */
#define STM32_SDMMC1_HANDLER                Vector104
#define STM32_SDMMC2_HANDLER                Vector230

#define STM32_SDMMC1_NUMBER                 49
#define STM32_SDMMC2_NUMBER                 131

/*
 * SPI units.
 */
#define STM32_SPI1_HANDLER                  VectorCC
#define STM32_SPI2_HANDLER                  VectorD0
#define STM32_SPI3_HANDLER                  Vector10C
#define STM32_SPI4_HANDLER                  Vector190
#define STM32_SPI5_HANDLER                  Vector194
#define STM32_SPI6_HANDLER                  Vector198

#define STM32_SPI1_NUMBER                   35
#define STM32_SPI2_NUMBER                   36
#define STM32_SPI3_NUMBER                   51
#define STM32_SPI4_NUMBER                   84
#define STM32_SPI5_NUMBER                   85
#define STM32_SPI6_NUMBER                   86

/*
 * TIM units.
 */
#define STM32_TIM1_BRK_HANDLER              VectorA0
#define STM32_TIM1_UP_HANDLER               VectorA4
#define STM32_TIM1_TRGCO_HANDLER            VectorA8
#define STM32_TIM1_CC_HANDLER               VectorAC
#define STM32_TIM2_HANDLER                  VectorB0
#define STM32_TIM3_HANDLER                  TIM3_IRQHandler
#define STM32_TIM4_HANDLER                  VectorB8
#define STM32_TIM5_HANDLER                  Vector108
#define STM32_TIM6_HANDLER                  Vector118
#define STM32_TIM7_HANDLER                  Vector11C
#define STM32_TIM8_BRK_TIM12_HANDLER        VectorEC
#define STM32_TIM8_UP_TIM13_HANDLER         VectorF0
#define STM32_TIM8_TRGCO_TIM14_HANDLER      VectorF4
#define STM32_TIM8_CC_HANDLER               VectorF8
#define STM32_TIM15_HANDLER                 Vector210
#define STM32_TIM16_HANDLER                 Vector214
#define STM32_TIM17_HANDLER                 Vector218

//NvR
#define STM32_LPTIM1_HANDLER                Vector1B4
#define STM32_LPTIM2_HANDLER                Vector268
#define STM32_LPTIM3_HANDLER                Vector26C


#define STM32_TIM1_BRK_NUMBER               24
#define STM32_TIM1_UP_NUMBER                25
#define STM32_TIM1_TRGCO_NUMBER             26
#define STM32_TIM1_CC_NUMBER                27
#define STM32_TIM2_NUMBER                   28
//#define STM32_TIM3_NUMBER                   29
#define STM32_TIM4_NUMBER                   30
#define STM32_TIM5_NUMBER                   50
#define STM32_TIM6_NUMBER                   54
#define STM32_TIM7_NUMBER                   55
#define STM32_TIM8_BRK_TIM12_NUMBER         43
#define STM32_TIM8_UP_TIM13_NUMBER          44
#define STM32_TIM8_TRGCO_TIM14_NUMBER       45
#define STM32_TIM8_CC_NUMBER                46
#define STM32_TIM15_NUMBER                  116
#define STM32_TIM16_NUMBER                  117
#define STM32_TIM17_NUMBER                  118

//NvR
#define STM32_TIM3_NUMBER                   46




#define STM32_LPTIM1_NUMBER                 93
#define STM32_LPTIM2_NUMBER                 138
#define STM32_LPTIM3_NUMBER                 139


/*
 * USART/UART units.
 */
#define STM32_USART1_HANDLER                USART1_IRQHandler
#define STM32_USART2_HANDLER                USART2_IRQHandler
#define STM32_USART3_HANDLER                USART3_IRQHandler
#define STM32_UART4_HANDLER                 UART4_IRQHandler
#define STM32_UART5_HANDLER                 UART5_IRQHandler
#define STM32_USART6_HANDLER                Vector15C
#define STM32_UART7_HANDLER                 Vector188
#define STM32_UART8_HANDLER                 Vector18C
#define STM32_LPUART1_HANDLER               LPUART1_IRQHandler



#define STM32_USART1_NUMBER                 61
#define STM32_USART2_NUMBER                 62
#define STM32_USART3_NUMBER                 63
#define STM32_UART4_NUMBER                  64
#define STM32_UART5_NUMBER                  65
//#define STM32_USART6_NUMBER                 71
//#define STM32_UART7_NUMBER                  82
//#define STM32_UART8_NUMBER                  83
#define STM32_LPUART1_NUMBER                66

/*
 * USB/OTG units.
 */
//#define STM32_OTG1_HANDLER                  OTG_FS_IRQHandler
//#define STM32_OTG1_EP1OUT_HANDLER           Vector1C8
//#define STM32_OTG1_EP1IN_HANDLER            Vector1CC
//#define STM32_OTG2_HANDLER                  Vector174
//#define STM32_OTG2_EP1OUT_HANDLER           Vector168
//#define STM32_OTG2_EP1IN_HANDLER            Vector16C

#define STM32_OTG1_NUMBER                   73
//#define STM32_OTG1_EP1OUT_NUMBER            98
//#define STM32_OTG1_EP1IN_NUMBER             99
//#define STM32_OTG2_NUMBER                   77
//#define STM32_OTG2_EP1OUT_NUMBER            74
//#define STM32_OTG2_EP1IN_NUMBER             75

/*
 * LTDC units.
 */
#define STM32_LTDC_EV_HANDLER               Vector1A0
#define STM32_LTDC_ER_HANDLER               Vector1A4

#define STM32_LTDC_EV_NUMBER                88
#define STM32_LTDC_ER_NUMBER                89

/*
 * DMA2D units.
 */
#define STM32_DMA2D_HANDLER                 Vector1A8

#define STM32_DMA2D_NUMBER                  90

/*
 * FSMC units.
 */
#define STM32_FSMC_HANDLER                  Vector100

#define STM32_FSMC_NUMBER                   48

/*
 * DCMI units.
 */
#define STM32_DCMI_HANDLER                  Vector178

#define STM32_DCMI_NUMBER                   78
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void irqInit(void);
  void irqDeinit(void);
#ifdef __cplusplus
}
#endif

#endif /* STM32_ISR_H */

/** @} */
