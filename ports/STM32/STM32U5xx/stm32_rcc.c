


#include "hal.h"

extern const uint8_t  APBPrescTable[8] ; // =  {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};


unsigned int
rcc_sys_ck_cpu(void)
{
	return SystemCoreClock ;

}


unsigned int
rcc_sys_ck_bus(void)
{
	/* Compute PCLK1 clock frequency --------------------------------------------*/
	/* Get PPRE1 prescaler */
	uint32_t tmp = APBPrescTable[((RCC->CFGR2 & RCC_CFGR2_PPRE1) >> RCC_CFGR2_PPRE1_Pos)];
	/* HCLK clock frequency */
	return SystemCoreClock >> tmp ;

}
