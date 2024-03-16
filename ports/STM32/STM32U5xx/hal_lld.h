
/**
 * @file    STM32U5xx/hal_lld.h
 * @brief   HAL subsystem low level driver header template.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

//#include "hal.h"
#include "nvic.h"
#include "stm32_registry.h"
#include "stm32_rcc.h"
#include "stm32_dma.h"


/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS FALSE

/**
 * @brief   Platform name.
 */
#define PLATFORM_NAME           "STM32U5"
#define PLATFORM_STM            1


/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/


/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#define STM32_RTC_CK                	  32768
#define STM32_HSI48_OSC                 48000000


#define RCC_MSIRANGE_4                 	RCC_ICSCR1_MSISRANGE_2                                                           /*!< MSI = 4 MHz     */
#define RCC_MSICALIBRATION_DEFAULT     	0x10U                   /*!< Default MSI calibration trimming value */
#define FLASH_LATENCY_1           		  FLASH_ACR_LATENCY_1WS    /*!< FLASH One wait state */
#define FLASH_LATENCY_4           		  FLASH_ACR_LATENCY_4WS    /*!< FLASH One wait state */

#define __HAL_RCC_PLL_CONFIG(__PLL1SOURCE__, __PLL1MBOOST__,__PLL1M__, __PLL1N__, __PLL1P__, __PLL1Q__, __PLL1R__) \
  do{   MODIFY_REG(RCC->PLL1CFGR,(RCC_PLL1CFGR_PLL1SRC | RCC_PLL1CFGR_PLL1M|\
                  RCC_PLL1CFGR_PLL1MBOOST), ((__PLL1SOURCE__) << RCC_PLL1CFGR_PLL1SRC_Pos) |\
                  (((__PLL1M__) - 1U) << RCC_PLL1CFGR_PLL1M_Pos) | (__PLL1MBOOST__));\
        MODIFY_REG(RCC->PLL1DIVR ,(RCC_PLL1DIVR_PLL1N | RCC_PLL1DIVR_PLL1P | RCC_PLL1DIVR_PLL1Q |\
                  RCC_PLL1DIVR_PLL1R), ( (((__PLL1N__) - 1U ) & RCC_PLL1DIVR_PLL1N) |\
                  ((((__PLL1P__) -1U ) << RCC_PLL1DIVR_PLL1P_Pos) &  RCC_PLL1DIVR_PLL1P) | \
                  ((((__PLL1Q__) -1U) << RCC_PLL1DIVR_PLL1Q_Pos) &  RCC_PLL1DIVR_PLL1Q) |\
                  ((((__PLL1R__)- 1U) << RCC_PLL1DIVR_PLL1R_Pos) &  RCC_PLL1DIVR_PLL1R))); \
  } while(0)


/** @defgroup RCC_PLL_Clock_Source  RCC PLL Clock Source
  * @{
  */
#define RCC_PLLSOURCE_NONE              0x00000000U
#define RCC_PLLSOURCE_MSI               RCC_PLL1CFGR_PLL1SRC_0
#define RCC_PLLSOURCE_HSI               RCC_PLL1CFGR_PLL1SRC_1
#define RCC_PLLSOURCE_HSE               (RCC_PLL1CFGR_PLL1SRC_0 | RCC_PLL1CFGR_PLL1SRC_1)
/**
  * @}
  */
/** @defgroup RCC_PLLMBOOST_EPOD_Clock_Divider PLLMBOOST EPOD Clock Divider
  * @{
  */
#define RCC_PLLMBOOST_DIV1              0x00000000U
#define RCC_PLLMBOOST_DIV2              RCC_PLL1CFGR_PLL1MBOOST_0
#define RCC_PLLMBOOST_DIV4              RCC_PLL1CFGR_PLL1MBOOST_1
#define RCC_PLLMBOOST_DIV6              (RCC_PLL1CFGR_PLL1MBOOST_1 | RCC_PLL1CFGR_PLL1MBOOST_0)
#define RCC_PLLMBOOST_DIV8              RCC_PLL1CFGR_PLL1MBOOST_2
#define RCC_PLLMBOOST_DIV10             (RCC_PLL1CFGR_PLL1MBOOST_2 | RCC_PLL1CFGR_PLL1MBOOST_0)
#define RCC_PLLMBOOST_DIV12             (RCC_PLL1CFGR_PLL1MBOOST_2 | RCC_PLL1CFGR_PLL1MBOOST_1)
#define RCC_PLLMBOOST_DIV14             (RCC_PLL1CFGR_PLL1MBOOST_2 | RCC_PLL1CFGR_PLL1MBOOST_1| RCC_PLL1CFGR_PLL1MBOOST_0)
#define RCC_PLLMBOOST_DIV16             RCC_PLL1CFGR_PLL1MBOOST_3
/**
  * @}
  */

/** @defgroup RCC_PLL_VCI_Range  RCC PLL1 VCI Range
  * @{
  */
#define RCC_PLLVCIRANGE_0               0x00000000U
#define RCC_PLLVCIRANGE_1               (RCC_PLL1CFGR_PLL1RGE_1 | RCC_PLL1CFGR_PLL1RGE_0)
/**
  * @}
  */


/** @defgroup RCC_PLL_Clock_Output  RCC PLL Clock Output
  * @{
  */
#define RCC_PLL1_DIVP                   RCC_PLL1CFGR_PLL1PEN
#define RCC_PLL1_DIVQ                   RCC_PLL1CFGR_PLL1QEN
#define RCC_PLL1_DIVR                   RCC_PLL1CFGR_PLL1REN
/**
  * @}
  */

/** @defgroup RCC_System_Clock_Source System Clock Source
  * @{
  */
#define RCC_SYSCLKSOURCE_MSI             0x00000000U                        /*!< MSI selection as system clock */
#define RCC_SYSCLKSOURCE_HSI             RCC_CFGR1_SW_0                     /*!< HSI selection as system clock */
#define RCC_SYSCLKSOURCE_HSE             RCC_CFGR1_SW_1                     /*!< HSE selection as system clock */
#define RCC_SYSCLKSOURCE_PLLCLK          (RCC_CFGR1_SW_0 | RCC_CFGR1_SW_1)  /*!< PLL1 selection as system clock */
/**
  * @}
  */

/** @defgroup RCCEx_USART1_Clock_Source USART1 Clock Source
  * @{
  */
#define RCC_USART1CLKSOURCE_PCLK2         0x00000000U
#define RCC_USART1CLKSOURCE_SYSCLK        RCC_CCIPR1_USART1SEL_0
#define RCC_USART1CLKSOURCE_HSI           RCC_CCIPR1_USART1SEL_1
#define RCC_USART1CLKSOURCE_LSE           (RCC_CCIPR1_USART1SEL_0 | RCC_CCIPR1_USART1SEL_1)
/**
  * @}
  */


/*
 * HSI48 related checks.
 */
#if STM32_HSI48_ENABLED
#define STM32_HSI48_CK                    STM32_HSI48_OSC

#else /* !STM32_HSI48_ENABLED */
#define STM32_HSI48_CK          0U

#endif


#define STM32_RNGCLK			                STM32_HSI48_OSC


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
  void hal_lld_init(void);
  void stm32_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
