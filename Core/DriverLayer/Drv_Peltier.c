/*
 * Drv_Peltier.c
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */
#include "main.h"
#include "ErrorCode.h"
#include "Datatype.h"
#include "Timer.h"
#include "HI_GptTimer.h"
#include "Drv_Peltier.h"

static TimerTimeOut g_BridgeSwitchTimer;
static PELTIER_DIR_STATE g_PrevDir;
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_Peltier_Init(void)
{
	/*Channel 0*/
	Drv_SetTimerPeriod(GetInstance_Timer1_P0() , CONVERT_HZ_TO_US(100U/*100Hz*/));
	Drv_StartTimerPwm(GetInstance_Timer1_P0() , TIM_CHANNEL_4);

	/*Channel 1*/
	Drv_SetTimerPeriod(GetInstance_Timer3_P1() , CONVERT_HZ_TO_US(100U/*100Hz*/));
	Drv_StartTimerPwm(GetInstance_Timer3_P1() , TIM_CHANNEL_3);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_SetPeltierPower(PELTIER_DIR_STATE m_Dir , uint8_t u8SpeedPercent)
{
	if(g_PrevDir == m_Dir)
	{
		if(PELTIER_DIR_COOL == g_PrevDir)
		{
			HAL_GPIO_WritePin(PELTIER_DIR1_GPIO_Port, PELTIER_DIR1_Pin, GPIO_PIN_SET);
			Drv_SetTimerPwmDutycycle(GetInstance_Timer1_P0() , TIM_CHANNEL_4 , u8SpeedPercent/*%*/);
		}
		else if(PELTIER_DIR_HEAT == m_Dir)
		{
			HAL_GPIO_WritePin(PELTIER_DIR0_GPIO_Port, PELTIER_DIR0_Pin, GPIO_PIN_SET);
			Drv_SetTimerPwmDutycycle(GetInstance_Timer3_P1() , TIM_CHANNEL_3 , u8SpeedPercent/*%*/);
		}
		else/*Peltier Dir OFF*/
		{
			HAL_GPIO_WritePin(PELTIER_DIR0_GPIO_Port, PELTIER_DIR0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PELTIER_DIR1_GPIO_Port, PELTIER_DIR1_Pin, GPIO_PIN_RESET);
		}
		TimeOut_Stop(&(g_BridgeSwitchTimer));
	}
	else
	{
		if(TRUE == TimeOut_IsTimerRunning(&(g_BridgeSwitchTimer)))
		{
			if(TRUE == TimeOut_IsTimeout(&(g_BridgeSwitchTimer)))
			{
				/*Turn OFF all bridges*/
				Drv_SetTimerPwmDutycycle(GetInstance_Timer1_P0() , TIM_CHANNEL_4 , 0U/*%*/);
				Drv_SetTimerPwmDutycycle(GetInstance_Timer3_P1() , TIM_CHANNEL_3 , 0U/*%*/);
				HAL_GPIO_WritePin(PELTIER_DIR0_GPIO_Port, PELTIER_DIR0_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(PELTIER_DIR1_GPIO_Port, PELTIER_DIR1_Pin, GPIO_PIN_RESET);
				g_PrevDir = m_Dir;
			}
			else
			{
				/*NOP*/
			}
		}
		else/*If timer is not running*/
		{
			TimeOut_Init(&(g_BridgeSwitchTimer));
			TimeOut_Start(&(g_BridgeSwitchTimer) , PELTIER_DIR_SWITCH_HYSTERISIS_TIMEOUT_MS/*MS*/);
		}
	}
}
