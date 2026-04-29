/*
 * Drv_GptTimer.h
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */

#ifndef HARDWAREINTERFACE_DRV_GPTTIMER_H_
#define HARDWAREINTERFACE_DRV_GPTTIMER_H_

#define CONVERT_HZ_TO_S(x)		(1U/x)
#define CONVERT_HZ_TO_US(x)		(1000000U * (1U/x))
#define CONVERT_SEC_TO_US(x) 	(x*1000000U)

void Drv_SetTimerPeriod(TIM_HandleTypeDef *htim ,
									uint32_t u32Period_us);

#endif /* HARDWAREINTERFACE_DRV_GPTTIMER_H_ */
