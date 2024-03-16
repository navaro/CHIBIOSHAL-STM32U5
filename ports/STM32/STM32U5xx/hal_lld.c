
/**
 * @file    LPC13xx/hal_lld.c
 * @brief   LPC13xx HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */


#include "hal.h"


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

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

#if 1
  RCC_TypeDef *rcc = RCC; /* For inspection.*/
  (void)rcc;
  PWR_TypeDef *pwr = PWR ;
  (void)pwr ;
#endif

#if 0
    /* regulator select */
    PWR->CR3 |= PWR_CR3_REGSEL ;
    /* Wait until system switch on new regulator */
    while (!(PWR->SVMSR & PWR_SVMSR_REGS) ) ;
#endif

#if 1
  /* USB power supply enable */
	PWR->SVMCR |= PWR_SVMCR_IO2SV
				| PWR_SVMCR_USV | PWR_SVMCR_UVMEN ; // USB enable

	/* Check the power supply configuration */
	while (!(PWR->SVMSR & PWR_SVMSR_VDDUSBRDY) ) ;
#endif

  /* DMA subsystems initialization.*/
#if defined(STM32_BDMA_REQUIRED)
  bdmaInit();
#endif
#if defined(STM32_DMA_REQUIRED)
  dmaInit();
#endif
#if defined(STM32_MDMA_REQUIRED)
  mdmaInit();
#endif

  /* IRQ subsystem initialization.*/
 irqInit();

}

/**
 * @brief   STM32H7xx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function should be invoked just after the system reset.
 *
 * @special
 */
void stm32_clock_init(void) {

    uint32_t timeout ;

#if 1
  RCC_TypeDef *rcc = RCC; /* For inspection.*/
  (void)rcc;
  PWR_TypeDef *pwr = PWR ;
  (void)pwr ;
#endif

  /* Enable power and clock to Backup Domain AHB3 */
  RCC->AHB3ENR |= RCC_AHB3ENR_PWREN;
  /* Delay after an RCC peripheral clock enabling */
  (void)RCC->AHB3ENR;

  /* MSI clock range selection in MSISRANGE and MSIKRANGE.*/
  RCC->ICSCR1 |=  RCC_ICSCR1_MSIRGSEL ;
  /* Select MSISRANGE 4MHz */
  RCC->ICSCR1 = (RCC->ICSCR1 & ~RCC_ICSCR1_MSISRANGE) | (RCC_MSIRANGE_4) ;
  /* select default trimming value */
  RCC->ICSCR2 = (RCC->ICSCR2 & ~RCC_ICSCR2_MSITRIM1) |
		  ((uint32_t)(RCC_MSICALIBRATION_DEFAULT)<<RCC_ICSCR2_MSITRIM1_Pos) ;

  /* flash latency to 4WS */
  FLASH->ACR = (FLASH->ACR & FLASH_ACR_LATENCY) | (FLASH_LATENCY_4) ;
  while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_LATENCY_4) ;

  /* PLL config */
  /* Disable PLL1 */
  RCC->CR &= ~RCC_CR_PLL1ON ;
  while (RCC->CR & RCC_CR_PLL1RDY) ;

  /* set PCLK3 prescaer div by 2 */
  RCC->CFGR3 |= RCC_CFGR3_PPRE3_2 ;

  /* Enable power and clock to RTC */
  RCC->APB3ENR |= RCC_APB3ENR_RTCAPBEN;
  /* Delay after an RCC peripheral clock enabling */
  (void)RCC->APB3ENR;

  /* Enable access to BDCR register */
  PWR->DBPR |= PWR_DBPR_DBP ;
  while (!(PWR->DBPR & PWR_DBPR_DBP)) ;

  /* Enable 32Khz external clock */
  RCC->BDCR |= RCC_BDCR_LSEON ;
  for (timeout=10000; timeout && !(RCC->BDCR & RCC_BDCR_LSERDY); timeout--) ;
  RCC->BDCR |= RCC_BDCR_RTCSEL_0 ;

  /* Disable BOOSTEN to programm PLL*/
  PWR->VOSR &= ~PWR_VOSR_BOOSTEN ;

  /* programm PLL */
  __HAL_RCC_PLL_CONFIG(RCC_PLLSOURCE_MSI,
		  RCC_PLLMBOOST_DIV1,
                        1,
                        80,
                        2,
                        2,
                        2);

  /* program PLL1FRACR */
  RCC->PLL1CFGR &=  ~RCC_PLL1CFGR_PLL1FRACEN;
  RCC->PLL1FRACR = RCC->PLL1FRACR & ~RCC_PLL1FRACR_PLL1FRACN ;
  RCC->PLL1CFGR |= RCC_PLL1CFGR_PLL1FRACEN ;

  /* program PLL1RGE */
  RCC->PLL1CFGR &= ~RCC_PLL1CFGR_PLL1RGE | (RCC_PLLVCIRANGE_0);

  /* set VOS1 and BOOSTEN */
  PWR->VOSR |= (PWR_VOSR_VOS_0 | PWR_VOSR_VOS_1) | PWR_VOSR_BOOSTEN;

  /* enable PLL1R */
  RCC->PLL1CFGR |= RCC_PLL1_DIVR ;

  /* enable PLL */
  RCC->CR |= RCC_CR_PLL1ON ;

  /* wait for PLL */
  while (!(RCC->CR & RCC_CR_PLL1RDY)) ;
  while (!(PWR->VOSR & PWR_VOSR_BOOSTRDY));

  /* selected PLL pll1_r_ck  as system clock */
  RCC->CFGR1 = (RCC->CFGR1 & ~RCC_CFGR1_SW) | RCC_SYSCLKSOURCE_PLLCLK;
  uint32_t val = RCC->CFGR1 & RCC_CFGR1_SWS ;
  while (val != (RCC_SYSCLKSOURCE_PLLCLK<<RCC_CFGR1_SWS_Pos)) {
	  val = RCC->CFGR1 & RCC_CFGR1_SWS ;
  }

  /* Note: All prescalers 0 so PCLK1,2 and are all 160MHz */

#if 0
  /* Enable the Secure Internal High Speed oscillator (SHSI) */
  RCC->CR |= RCC_CR_SHSION ;
  while (!(RCC->CR & RCC_CR_SHSIRDY)) ;
#endif

#if 1
  /* Enable the Internal High Speed oscillator (HSI48) */
  RCC->CR |= RCC_CR_HSI48ON ;
  while (!(RCC->CR & RCC_CR_HSI48RDY)) ;
#endif

  /* set USART1 clock source */
  RCC->CCIPR1 =  (RCC->CCIPR1 & ~RCC_CCIPR1_USART1SEL) |
		  	  	  (uint32_t)(RCC_USART1CLKSOURCE_PCLK2) ;
  /* USART1 Peripheral clock enable */
  // Note: uncomment to enable debug UART at startup already
  //RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  /* Delay after an RCC peripheral clock enabling */
  //(void)RCC->APB2ENR;

  /* set I2C2 clock source HSI16 */
  RCC->CCIPR1 =  (RCC->CCIPR1 & ~RCC_CCIPR1_I2C2SEL) |
                  (uint32_t)(RCC_CCIPR1_I2C2SEL_1) ;

  // enable BKPSRAM
  RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN ;

  /* GPIO CLOCK ENABLE */
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOAEN ;
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOBEN ;
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOCEN ;
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIODEN ;
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOEEN ;
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOFEN ;
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOGEN ;
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOHEN ;
  RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOIEN ;

}

/** @} */
