/*
 * Drv_GptTimer.c
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */
#include "main.h"
#include "ErrorCode.h"
#include <stdbool.h>
#include "Drv_GptTimer.h"
/*********************.HAL_GPIO_EXTI_Callback().*****************************
 .Purpose        : Callback for GPIO interrupt Rising and falling
 .Returns        :  RETURN_ERROR
					RETURN_SUCCESS
 .Note           :
 ****************************************************************************/
/*
 * Set timer period during runtime
 */
void Drv_SetTimerPeriod(TIM_HandleTypeDef *htim ,
									uint32_t u32Period_us)
{
	uint32_t u32Prescalar = 0U;
	uint32_t u32ARR = 0U;
    uint32_t u32Timer_clk = 0U;

    // Get APB1 or APB2 timer clock
    {
    	u32Timer_clk = HAL_RCC_GetPCLK2Freq();

        // If APB2 prescaler > 1, timer clock is doubled
        if ((RCC->CFGR & RCC_CFGR_PPRE2) != RCC_CFGR_PPRE2_DIV1)
        {
        	u32Timer_clk *= 2U;  // timer clock doubled if APB prescaler >
        }

    }

    /*Target counter clock: 1 MHz (1 tick = 1 µs)*/
    // Compute PSC and ARR
    u32Prescalar = (u32Timer_clk / 1000000ULL) - 1U;/*1 Mhz*/
    u32ARR		 = (u32Period_us) - 1U;

	__HAL_TIM_DISABLE(htim);
	__HAL_TIM_SET_PRESCALER(htim, u32Prescalar);
	__HAL_TIM_SET_AUTORELOAD(htim, u32ARR);
	__HAL_TIM_SET_COUNTER(htim, 0U);
	__HAL_TIM_ENABLE(htim);
}
