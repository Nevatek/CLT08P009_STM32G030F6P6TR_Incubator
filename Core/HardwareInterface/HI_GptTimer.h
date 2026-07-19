/*
 * Drv_GptTimer.h
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */

#ifndef HARDWAREINTERFACE_HI_GPTTIMER_H_
#define HARDWAREINTERFACE_HI_GPTTIMER_H_

#define CONVERT_HZ_TO_S(x)		(1U/x)
#define CONVERT_HZ_TO_US(x)		(1000000U/x)

void Drv_SetTimerPeriod(TIM_HandleTypeDef *htim ,
									uint32_t u32Period_us);
void Drv_SetTimerPwmDutycycle(TIM_HandleTypeDef* htim ,
					uint32_t Channel , uint8_t DutyCyclePercentage);
void Drv_StartTimerPwm(TIM_HandleTypeDef* htim ,
					uint32_t Channel);
void Drv_StopTimerPwm(TIM_HandleTypeDef* htim ,
					uint32_t Channel);
void delayUs (TIM_HandleTypeDef *htim , uint16_t time);
#endif /* HARDWAREINTERFACE_HI_GPTTIMER_H_ */
