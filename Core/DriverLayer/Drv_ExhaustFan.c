/*
 * Drv_ExhaustFan.c
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */
#include "main.h"
#include "ErrorCode.h"
#include "HI_GptTimer.h"
#include "Drv_ExhaustFan.h"
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_InitilizeExhaustFan(void)
{
	TIM_HandleTypeDef *pTim = GetInstance_Timer16();
	Drv_SetTimerPeriod(pTim , CONVERT_HZ_TO_US(100U/*100Hz*/));
	Drv_SetSpeedExhaustFan(10U/*10%*/);
	Drv_StartTimerPwm(pTim , TIM_CHANNEL_1);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_SetSpeedExhaustFan(uint8_t u8SpeedPercent)
{
	TIM_HandleTypeDef *pTim = GetInstance_Timer16();
	Drv_SetTimerPwmDutycycle(pTim , TIM_CHANNEL_1 , u8SpeedPercent/*%*/);
}
