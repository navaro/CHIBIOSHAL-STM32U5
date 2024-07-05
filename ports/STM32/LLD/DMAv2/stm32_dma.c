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
 * @file    DMAv2/stm32_dma.c
 * @brief   Enhanced DMA helper driver code.
 *
 * @addtogroup STM32_DMA
 * @details DMA sharing helper driver. In the STM32 the DMA streams are a
 *          shared resource, this driver allows to allocate and free DMA
 *          streams at runtime in order to allow all the other device
 *          drivers to coordinate the access to the resource.
 * @note    The DMA ISR handlers are all declared into this module because
 *          sharing, the various device drivers can associate a callback to
 *          ISRs when allocating streams.
 * @{
 */

#include "hal.h"

/* The following macro is only defined if some driver requiring DMA services
   has been enabled.*/
#if defined(STM32_DMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Mask of the DMA1 streams in @p dma.allocated_mask.
 */
#if !defined(STM32U5) // STM32U5 PORT
#define STM32_DMA1_STREAMS_MASK     0x000000FFU
#else
#define STM32_DMA1_STREAMS_MASK     0x0000FFFFU
#endif

/**
 * @brief   Mask of the DMA2 streams in @p dma.allocated_mask.
 */
#define STM32_DMA2_STREAMS_MASK     0x0000FF00U

#if STM32_DMA_SUPPORTS_DMAMUX == TRUE

#define DMA1_CH0_VARIANT            DMAMUX1_Channel0
#define DMA1_CH1_VARIANT            DMAMUX1_Channel1
#define DMA1_CH2_VARIANT            DMAMUX1_Channel2
#define DMA1_CH3_VARIANT            DMAMUX1_Channel3
#define DMA1_CH4_VARIANT            DMAMUX1_Channel4
#define DMA1_CH5_VARIANT            DMAMUX1_Channel5
#define DMA1_CH6_VARIANT            DMAMUX1_Channel6
#define DMA1_CH7_VARIANT            DMAMUX1_Channel7
#define DMA2_CH0_VARIANT            DMAMUX1_Channel8
#define DMA2_CH1_VARIANT            DMAMUX1_Channel9
#define DMA2_CH2_VARIANT            DMAMUX1_Channel10
#define DMA2_CH3_VARIANT            DMAMUX1_Channel11
#define DMA2_CH4_VARIANT            DMAMUX1_Channel12
#define DMA2_CH5_VARIANT            DMAMUX1_Channel13
#define DMA2_CH6_VARIANT            DMAMUX1_Channel14
#define DMA2_CH7_VARIANT            DMAMUX1_Channel15

#else /* !(STM32_DMA_SUPPORTS_DMAMUX == TRUE) */

#define DMA1_CH0_VARIANT            0
#define DMA1_CH1_VARIANT            0
#define DMA1_CH2_VARIANT            0
#define DMA1_CH3_VARIANT            0
#define DMA1_CH4_VARIANT            0
#define DMA1_CH5_VARIANT            0
#define DMA1_CH6_VARIANT            0
#define DMA1_CH7_VARIANT            0
#define DMA2_CH0_VARIANT            0
#define DMA2_CH1_VARIANT            0
#define DMA2_CH2_VARIANT            0
#define DMA2_CH3_VARIANT            0
#define DMA2_CH4_VARIANT            0
#define DMA2_CH5_VARIANT            0
#define DMA2_CH6_VARIANT            0
#define DMA2_CH7_VARIANT            0

#endif /* !(STM32_DMA_SUPPORTS_DMAMUX == TRUE) */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   DMA streams descriptors.
 * @details This table keeps the association between an unique stream
 *          identifier and the involved physical registers.
 * @note    Don't use this array directly, use the appropriate wrapper macros
 *          instead: @p STM32_DMA1_STREAM0, @p STM32_DMA1_STREAM1 etc.
 */
const stm32_dma_stream_t _stm32_dma_streams[STM32_DMA_STREAMS] = {
#if !defined(STM32U5) // STM32U5 PORT
  {DMA1_Stream0, &DMA1->LIFCR, DMA1_CH0_VARIANT,  0,  0, STM32_DMA1_CH0_NUMBER},
  {DMA1_Stream1, &DMA1->LIFCR, DMA1_CH1_VARIANT,  6,  1, STM32_DMA1_CH1_NUMBER},
  {DMA1_Stream2, &DMA1->LIFCR, DMA1_CH2_VARIANT, 16,  2, STM32_DMA1_CH2_NUMBER},
  {DMA1_Stream3, &DMA1->LIFCR, DMA1_CH3_VARIANT, 22,  3, STM32_DMA1_CH3_NUMBER},
  {DMA1_Stream4, &DMA1->HIFCR, DMA1_CH4_VARIANT,  0,  4, STM32_DMA1_CH4_NUMBER},
  {DMA1_Stream5, &DMA1->HIFCR, DMA1_CH5_VARIANT,  6,  5, STM32_DMA1_CH5_NUMBER},
  {DMA1_Stream6, &DMA1->HIFCR, DMA1_CH6_VARIANT, 16,  6, STM32_DMA1_CH6_NUMBER},
  {DMA1_Stream7, &DMA1->HIFCR, DMA1_CH7_VARIANT, 22,  7, STM32_DMA1_CH7_NUMBER},
  {DMA2_Stream0, &DMA2->LIFCR, DMA2_CH0_VARIANT,  0,  8, STM32_DMA2_CH0_NUMBER},
  {DMA2_Stream1, &DMA2->LIFCR, DMA2_CH1_VARIANT,  6,  9, STM32_DMA2_CH1_NUMBER},
  {DMA2_Stream2, &DMA2->LIFCR, DMA2_CH2_VARIANT, 16, 10, STM32_DMA2_CH2_NUMBER},
  {DMA2_Stream3, &DMA2->LIFCR, DMA2_CH3_VARIANT, 22, 11, STM32_DMA2_CH3_NUMBER},
  {DMA2_Stream4, &DMA2->HIFCR, DMA2_CH4_VARIANT,  0, 12, STM32_DMA2_CH4_NUMBER},
  {DMA2_Stream5, &DMA2->HIFCR, DMA2_CH5_VARIANT,  6, 13, STM32_DMA2_CH5_NUMBER},
  {DMA2_Stream6, &DMA2->HIFCR, DMA2_CH6_VARIANT, 16, 14, STM32_DMA2_CH6_NUMBER},
  {DMA2_Stream7, &DMA2->HIFCR, DMA2_CH7_VARIANT, 22, 15, STM32_DMA2_CH7_NUMBER},
#else
		  {GPDMA1_Channel0, 0, STM32_DMA1_CH0_NUMBER},
		  {GPDMA1_Channel1, 1, STM32_DMA1_CH1_NUMBER},
		  {GPDMA1_Channel2, 2, STM32_DMA1_CH2_NUMBER},
		  {GPDMA1_Channel3, 3, STM32_DMA1_CH3_NUMBER},
		  {GPDMA1_Channel4, 4, STM32_DMA1_CH4_NUMBER},
		  {GPDMA1_Channel5, 5, STM32_DMA1_CH5_NUMBER},
		  {GPDMA1_Channel6, 6, STM32_DMA1_CH6_NUMBER},
		  {GPDMA1_Channel7, 7, STM32_DMA1_CH7_NUMBER},
#if 1
          {GPDMA1_Channel8, 8, STM32_DMA1_CH8_NUMBER},
          {GPDMA1_Channel9, 9, STM32_DMA1_CH9_NUMBER},
          {GPDMA1_Channel10, 10, STM32_DMA1_CH10_NUMBER},
          {GPDMA1_Channel11, 11, STM32_DMA1_CH11_NUMBER},
          {GPDMA1_Channel12, 12, STM32_DMA1_CH12_NUMBER},
          {GPDMA1_Channel13, 13, STM32_DMA1_CH13_NUMBER},
          {GPDMA1_Channel14, 14, STM32_DMA1_CH14_NUMBER},
          {GPDMA1_Channel15, 15, STM32_DMA1_CH15_NUMBER},
#endif
#endif
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Global DMA-related data structures.
 */
static struct {
  /**
   * @brief   Mask of the allocated streams.
   */
  uint32_t          allocated_mask;
  /**
   * @brief   DMA IRQ redirectors.
   */
  struct {
    /**
     * @brief   DMA callback function.
     */
    stm32_dmaisr_t    func;
    /**
     * @brief   DMA callback parameter.
     */
    void              *param;
  } streams[STM32_DMA_STREAMS];
} dma;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   DMA1 stream 0 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH0_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA1->LISR >> 0U) & STM32_DMA_ISR_MASK;
  DMA1->LIFCR = flags << 0U;
#else
  flags = _stm32_dma_streams[0].stream->CSR ;
  _stm32_dma_streams[0].stream->CFCR = flags & STM32_DMA_ISR_MASK;
#endif
  if (dma.streams[0].func)
    dma.streams[0].func(dma.streams[0].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 1 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH1_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA1->LISR >> 6U) & STM32_DMA_ISR_MASK;
  DMA1->LIFCR = flags << 6U;
#else
  flags = _stm32_dma_streams[1].stream->CSR ;
  _stm32_dma_streams[1].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[1].func)
    dma.streams[1].func(dma.streams[1].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 2 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH2_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA1->LISR >> 16U) & STM32_DMA_ISR_MASK;
  DMA1->LIFCR = flags << 16U;
#else
  flags = _stm32_dma_streams[2].stream->CSR ;
  _stm32_dma_streams[2].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[2].func)
    dma.streams[2].func(dma.streams[2].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 3 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH3_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA1->LISR >> 22U) & STM32_DMA_ISR_MASK;
  DMA1->LIFCR = flags << 22U;
#else
  flags = _stm32_dma_streams[3].stream->CSR ;
  _stm32_dma_streams[3].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[3].func)
    dma.streams[3].func(dma.streams[3].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 4 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH4_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA1->HISR >> 0U) & STM32_DMA_ISR_MASK;
  DMA1->HIFCR = flags << 0U;
#else
  flags = _stm32_dma_streams[4].stream->CSR ;
  _stm32_dma_streams[4].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[4].func)
    dma.streams[4].func(dma.streams[4].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 5 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH5_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA1->HISR >> 6U) & STM32_DMA_ISR_MASK;
  DMA1->HIFCR = flags << 6U;
#else
  flags = _stm32_dma_streams[5].stream->CSR ;
  _stm32_dma_streams[5].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[5].func)
    dma.streams[5].func(dma.streams[5].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 6 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH6_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA1->HISR >> 16U) & STM32_DMA_ISR_MASK;
  DMA1->HIFCR = flags << 16U;
#else
  flags = _stm32_dma_streams[6].stream->CSR ;
  _stm32_dma_streams[6].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[6].func)
    dma.streams[6].func(dma.streams[6].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 stream 7 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA1_CH7_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA1->HISR >> 22U) & STM32_DMA_ISR_MASK;
  DMA1->HIFCR = flags << 22U;
#else
  flags = _stm32_dma_streams[7].stream->CSR ;
  _stm32_dma_streams[7].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[7].func)
    dma.streams[7].func(dma.streams[7].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 0 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA2_CH0_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA2->LISR >> 0U) & STM32_DMA_ISR_MASK;
  DMA2->LIFCR = flags << 0U;
#else
  flags = _stm32_dma_streams[8].stream->CSR ;
  _stm32_dma_streams[8].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[8].func)
    dma.streams[8].func(dma.streams[8].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 1 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA2_CH1_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA2->LISR >> 6U) & STM32_DMA_ISR_MASK;
  DMA2->LIFCR = flags << 6U;
#else
  flags = _stm32_dma_streams[9].stream->CSR ;
  _stm32_dma_streams[9].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[9].func)
    dma.streams[9].func(dma.streams[9].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 2 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA2_CH2_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA2->LISR >> 16U) & STM32_DMA_ISR_MASK;
  DMA2->LIFCR = flags << 16U;
#else
  flags = _stm32_dma_streams[10].stream->CSR ;
  _stm32_dma_streams[10].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[10].func)
    dma.streams[10].func(dma.streams[10].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 3 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA2_CH3_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA2->LISR >> 22U) & STM32_DMA_ISR_MASK;
  DMA2->LIFCR = flags << 22U;
#else
  flags = _stm32_dma_streams[11].stream->CSR ;
  _stm32_dma_streams[11].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[11].func)
    dma.streams[11].func(dma.streams[11].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 4 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA2_CH4_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA2->HISR >> 0U) & STM32_DMA_ISR_MASK;
  DMA2->HIFCR = flags << 0U;
#else
  flags = _stm32_dma_streams[12].stream->CSR ;
  _stm32_dma_streams[12].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[12].func)
    dma.streams[12].func(dma.streams[12].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 5 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA2_CH5_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA2->HISR >> 6U) & STM32_DMA_ISR_MASK;
  DMA2->HIFCR = flags << 6U;
#else
  flags = _stm32_dma_streams[13].stream->CSR ;
  _stm32_dma_streams[13].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[13].func)
    dma.streams[13].func(dma.streams[13].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 6 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA2_CH6_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA2->HISR >> 16U) & STM32_DMA_ISR_MASK;
  DMA2->HIFCR = flags << 16U;
#else
  flags = _stm32_dma_streams[14].stream->CSR ;
  _stm32_dma_streams[14].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[14].func)
    dma.streams[14].func(dma.streams[14].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA2 stream 7 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(STM32_DMA2_CH7_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

#if !defined(STM32U5) // STM32U5 PORT
  flags = (DMA2->HISR >> 22U) & STM32_DMA_ISR_MASK;
  DMA2->HIFCR = flags << 22U;
#else
  flags = _stm32_dma_streams[15].stream->CSR ;
  _stm32_dma_streams[15].stream->CFCR = flags & STM32_DMA_ISR_MASK ;
#endif
  if (dma.streams[15].func)
    dma.streams[15].func(dma.streams[15].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   STM32 DMA helper initialization.
 *
 * @init
 */
void dmaInit(void) {
  unsigned i;

  dma.allocated_mask = 0U;
  for (i = 0U; i < STM32_DMA_STREAMS; i++) {
#if !defined(STM32U5) // STM32U5 PORT
    _stm32_dma_streams[i].stream->CR = STM32_DMA_CR_RESET_VALUE;
#else
    _stm32_dma_streams[i].stream->CCR = STM32_DMA_CR_RESET_VALUE;
#endif
    dma.streams[i].func = NULL;
  }
#if !defined(STM32U5) // STM32U5 PORT
  DMA1->LIFCR = 0xFFFFFFFFU;
  DMA1->HIFCR = 0xFFFFFFFFU;
  DMA2->LIFCR = 0xFFFFFFFFU;
  DMA2->HIFCR = 0xFFFFFFFFU;
#endif
}

/**
 * @brief   Allocates a DMA stream.
 * @details The stream is allocated and, if required, the DMA clock enabled.
 *          The function also enables the IRQ vector associated to the stream
 *          and initializes its priority.
 *
 * @param[in] id        numeric identifiers of a specific stream or:
 *                      - @p STM32_DMA_STREAM_ID_ANY for any stream.
 *                      - @p STM32_DMA_STREAM_ID_ANY_DMA1 for any stream
 *                        on DMA1.
 *                      - @p STM32_DMA_STREAM_ID_ANY_DMA2 for any stream
 *                        on DMA2.
 *                      .
 * @param[in] priority  IRQ priority for the DMA stream
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p stm32_dma_stream_t
 *                      structure.
 * @retval NULL         if a/the stream is not available.
 *
 * @iclass
 */
const stm32_dma_stream_t *dmaStreamAllocI(uint32_t id,
                                          uint32_t priority,
                                          stm32_dmaisr_t func,
                                          void *param) {
  uint32_t i, startid, endid;

  osalDbgCheckClassI();

  if (id < STM32_DMA_STREAMS) {
    startid = id;
    endid   = id;
  }
#if defined(STM32U5) || STM32_DMA_SUPPORTS_DMAMUX == TRUE // STM32U5 PORT
  else if (id == STM32_DMA_STREAM_ID_ANY) {
    startid = 0U;
    endid   = STM32_DMA_STREAMS - 1U;
  }
#if !defined(STM32U5) // STM32U5 PORT
  else if (id == STM32_DMA_STREAM_ID_ANY_DMA1) {
    startid = 0U;
    endid   = (STM32_DMA_STREAMS / 2U) - 1U;
  }
  else if (id == STM32_DMA_STREAM_ID_ANY_DMA2) {
    startid = (STM32_DMA_STREAMS / 2U) - 1U;
    endid   = STM32_DMA_STREAMS - 1U;
  }
#endif
#endif
  else {
    osalDbgCheck(false);
    return NULL;
  }

  for (i = startid; i <= endid; i++) {
    uint32_t mask = (1U << i);
    if ((dma.allocated_mask & mask) == 0U) {
      const stm32_dma_stream_t *dmastp = STM32_DMA_STREAM(i);

      /* Installs the DMA handler.*/
      dma.streams[i].func  = func;
      dma.streams[i].param = param;
      dma.allocated_mask  |= mask;

      /* Enabling DMA clocks required by the current streams set.*/
      if ((STM32_DMA1_STREAMS_MASK & mask) != 0U) {
        rccEnableDMA1(true);
      }
#if !defined(STM32U5) // STM32U5 PORT
      if ((STM32_DMA2_STREAMS_MASK & mask) != 0U) {
        rccEnableDMA2(true);
      }
#endif

#if (STM32_DMA_SUPPORTS_DMAMUX == TRUE) && defined(rccEnableDMAMUX)
      /* Enabling DMAMUX if present.*/
      if (dma.allocated_mask != 0U) {
        rccEnableDMAMUX(true);
      }
#endif

      /* Putting the stream in a safe state.*/
      dmaStreamDisable(dmastp);
#if defined(STM32U5) // STM32U5 PORT
      dmastp->stream->CCR = STM32_DMA_CR_RESET_VALUE;
      dmastp->stream->CFCR = STM32_DMA_FCR_RESET_VALUE;
#else
      dmastp->stream->CR = STM32_DMA_CR_RESET_VALUE;
      dmastp->stream->FCR = STM32_DMA_FCR_RESET_VALUE;
#endif
      /* Enables the associated IRQ vector if a callback is defined.*/
      if (func != NULL) {
        nvicEnableVector(dmastp->vector, priority);
      }

      return dmastp;
    }
  }

  return NULL;
}

/**
 * @brief   Allocates a DMA stream.
 * @details The stream is allocated and, if required, the DMA clock enabled.
 *          The function also enables the IRQ vector associated to the stream
 *          and initializes its priority.
 *
 * @param[in] id        numeric identifiers of a specific stream or:
 *                      - @p STM32_DMA_STREAM_ID_ANY for any stream.
 *                      - @p STM32_DMA_STREAM_ID_ANY_DMA1 for any stream
 *                        on DMA1.
 *                      - @p STM32_DMA_STREAM_ID_ANY_DMA2 for any stream
 *                        on DMA2.
 *                      .
 * @param[in] priority  IRQ priority for the DMA stream
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p stm32_dma_stream_t
 *                      structure.
 * @retval NULL         if a/the stream is not available.
 *
 * @api
 */
const stm32_dma_stream_t *dmaStreamAlloc(uint32_t id,
                                         uint32_t priority,
                                         stm32_dmaisr_t func,
                                         void *param) {
  const stm32_dma_stream_t *dmastp;

  osalSysLock();
  dmastp = dmaStreamAllocI(id, priority, func, param);
  osalSysUnlock();

  return dmastp;
}

/**
 * @brief   Releases a DMA stream.
 * @details The stream is freed and, if required, the DMA clock disabled.
 *          Trying to release a unallocated stream is an illegal operation
 *          and is trapped if assertions are enabled.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 *
 * @iclass
 */
void dmaStreamFreeI(const stm32_dma_stream_t *dmastp) {

  osalDbgCheck(dmastp != NULL);

  /* Check if the streams is not taken.*/
  osalDbgAssert((dma.allocated_mask & (1U << dmastp->selfindex)) != 0U,
                "not allocated");

  /* Disables the associated IRQ vector.*/
  nvicDisableVector(dmastp->vector);

  /* Marks the stream as not allocated.*/
  dma.allocated_mask &= ~(1U << dmastp->selfindex);

  /* Shutting down clocks that are no more required, if any.*/
  if ((dma.allocated_mask & STM32_DMA1_STREAMS_MASK) == 0U) {
    rccDisableDMA1();
  }
#if !defined(STM32U5) // STM32U5 PORT
  if ((dma.allocated_mask & STM32_DMA2_STREAMS_MASK) == 0U) {
    rccDisableDMA2();
  }
#endif
#if (STM32_DMA_SUPPORTS_DMAMUX == TRUE) && defined(rccDisableDMAMUX)
  /* Shutting down DMAMUX if present.*/
  if (dma.allocated_mask == 0U) {
    rccDisableDMAMUX();
  }
#endif
}

/**
 * @brief   Releases a DMA stream.
 * @details The stream is freed and, if required, the DMA clock disabled.
 *          Trying to release a unallocated stream is an illegal operation
 *          and is trapped if assertions are enabled.
 *
 * @param[in] dmastp    pointer to a stm32_dma_stream_t structure
 *
 * @api
 */
void dmaStreamFree(const stm32_dma_stream_t *dmastp) {

  osalSysLock();
  dmaStreamFreeI(dmastp);
  osalSysUnlock();
}

#if !defined(STM32U5) && (STM32_DMA_SUPPORTS_DMAMUX == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Associates a peripheral request to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a @p stm32_dma_stream_t structure
 * @param[in] per       peripheral identifier
 *
 * @special
 */
void dmaSetRequestSource(const stm32_dma_stream_t *dmastp, uint32_t per) {

  osalDbgCheck(per < 256U);

  dmastp->mux->CCR = per;
}
#endif

#endif /* STM32_DMA_REQUIRED */

/** @} */
