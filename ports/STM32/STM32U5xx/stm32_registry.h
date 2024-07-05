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
 * @file    STM32U5xx/stm32_registry.h
 * @brief   STM32U5xx capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef STM32_REGISTRY_H
#define STM32_REGISTRY_H

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/* Cores.*/
#define STM32_HAS_M33                        TRUE

/**
 * @name    STM32U5xx capabilities
 * @{
 */

/*===========================================================================*/
/* Common.                                                                   */
/*===========================================================================*/

/* USART attributes.*/
#define STM32_HAS_USART1                    TRUE

#define STM32_PCLK1							160000000
#define STM32_USART1CLK						160000000
#define STM32_USART2CLK						160000000
#define STM32_USART3CLK						160000000
#define STM32_UART4CLK						160000000
#define STM32_UART5CLK						160000000
#define STM32_LPUART1CLK					80000000



#define STM32_HAS_GPIOA                     TRUE
#define STM32_HAS_GPIOB                     TRUE
#define STM32_HAS_GPIOC                     TRUE
#define STM32_HAS_GPIOD                     TRUE
#define STM32_HAS_GPIOE                     TRUE
#define STM32_HAS_GPIOF                     TRUE
#define STM32_HAS_GPIOG                     TRUE
#define STM32_HAS_GPIOH                     TRUE
#define STM32_HAS_GPIOI                     TRUE


/* USART attributes.*/
#define STM32_HAS_USART1                    TRUE
#define STM32_HAS_USART2                    TRUE
#define STM32_HAS_USART3                    TRUE
#define STM32_HAS_UART4                     TRUE
#define STM32_HAS_UART5                     TRUE
#define STM32_HAS_USART6                    FALSE
#define STM32_HAS_UART7                     FALSE
#define STM32_HAS_UART8                     FALSE
#define STM32_HAS_LPUART1                   TRUE

#define STM32_UART_USART2_IRQ_PRIORITY      5
#define STM32_UART_USART3_IRQ_PRIORITY      5
#define STM32_UART_USART4_IRQ_PRIORITY      9
#define STM32_UART_UART5_IRQ_PRIORITY       5

#define STM32_SERIAL_UART4_PRIORITY         8



#define STM32_USART2_RX_DMA_MSK             (STM32_DMA_STREAM_ID_MSK(1, 1))
#define STM32_USART2_RX_DMA_CHN             0x00000001
#define STM32_USART2_TX_DMA_MSK             (STM32_DMA_STREAM_ID_MSK(1, 2))
#define STM32_USART2_TX_DMA_CHN             0x02000010

#define STM32_DMAMUX1_SPI1_RX				6
#define STM32_DMAMUX1_SPI1_TX				7
#define STM32_DMAMUX1_SPI2_RX				8
#define STM32_DMAMUX1_SPI2_TX				9

#define STM32_DMAMUX1_USART2_RX				26
#define STM32_DMAMUX1_USART2_TX				27
#define STM32_DMAMUX1_USART3_RX             28
#define STM32_DMAMUX1_USART3_TX             29
#define STM32_DMAMUX1_USART4_RX             30
#define STM32_DMAMUX1_USART4_TX             31
#define STM32_DMAMUX1_UART5_RX				32
#define STM32_DMAMUX1_UART5_TX				33

/* SPI attributes.*/
#define STM32_HAS_SPI1                      TRUE

#define STM32_HAS_SPI2                      TRUE

#define STM32_HAS_SPI3                      TRUE

#define STM32_HAS_SPI4                      TRUE

#define STM32_HAS_SPI5                      TRUE

/* DMA attributes.*/
#define STM32_DMA1_NUM_CHANNELS             8
#define STM32_DMA2_NUM_CHANNELS             0
#define STM32_ADVANCED_DMA                  TRUE
#define STM32_DMA_SUPPORTS_DMAMUX           FALSE
#define STM32_DMA_SUPPORTS_CSELR			FALSE
#define STM32_HAS_DMA1						TRUE
#define STM32_HAS_DMA2						FALSE


/* RTC attributes.*/
#define STM32_HAS_RTC                       TRUE

/* Watchdog attributes.*/
#define STM32_HAS_IWDG						TRUE

/* I2C attributes.*/
#define STM32_HAS_I2C1						TRUE
#define STM32_HAS_I2C2						TRUE
#define STM32_HAS_I2C3						TRUE
#define STM32_HAS_I2C4						FALSE
#define STM32_I2C4_USE_BDMA					FALSE


/* timers */
#define STM32_HAS_TIM3						TRUE


/* USB attributes.*/
#define STM32_OTG_STEPPING                  2
#define STM32_HAS_OTG1                      TRUE
#define STM32_OTG1_ENDPOINTS                8

#define STM32_HAS_OTG2                      FALSE
#define STM32_OTG2_ENDPOINTS                8

#define STM32_HAS_USB                       TRUE

#define STM32_HAS_RNG1						TRUE

#define STM32_TIM_MAX_CHANNELS              4
#define STM32_TIM4_CHANNELS                 4
#define STM32_TIM16_CHANNELS                4
#define STM32_TIM17_CHANNELS                4
#define STM32_HAS_TIM4                      TRUE
#define STM32_HAS_TIM16                     TRUE
#define STM32_HAS_TIM17                     TRUE

#define STM32_TIM4CLK                       STM32_PCLK1
#define STM32_TIM16CLK                      STM32_PCLK1
#define STM32_TIM17CLK                      STM32_PCLK1


#define STM32_EXTI_NUM_LINES                16
#define STM32_EXTI_IMR1_MASK                0xFFFF

#endif /* STM32_REGISTRY_H */

/** @} */
