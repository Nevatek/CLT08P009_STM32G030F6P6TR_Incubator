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
//	Drv_SetTimerPeriod(GetInstance_Timer16() , CONVERT_HZ_TO_US(100U/*100Hz*/));
//	Drv_StartTimerPwm(GetInstance_Timer16() , TIM_CHANNEL_1);
	HAL_GPIO_WritePin(FAN_ENABLE_GPIO_Port, FAN_ENABLE_Pin, GPIO_PIN_SET);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_SetSpeedExhaustFan(uint8_t u8SpeedPercent)
{
//	Drv_SetTimerPwmDutycycle(GetInstance_Timer16() , TIM_CHANNEL_1 , u8SpeedPercent/*%*/);
}
