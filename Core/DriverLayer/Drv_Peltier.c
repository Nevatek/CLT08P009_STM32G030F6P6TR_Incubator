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
#if (PELTIER_PID_CONTROL)
static TimerTimeOut g_BridgeSwitchTimer;
#endif
static PELTIER_DIR_STATE g_CurrDir;
static PELTIER_STATE g_CurrPeltierState;
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_Peltier_Init(void)
{
#if (PELTIER_PID_CONTROL)
	/*Channel 0*/
	Drv_SetTimerPeriod(GetInstance_Timer1_P0() , CONVERT_HZ_TO_US(100U/*100Hz*/));
	Drv_StartTimerPwm(GetInstance_Timer1_P0() , TIM_CHANNEL_4);

	/*Channel 1*/
	Drv_SetTimerPeriod(GetInstance_Timer3_P1() , CONVERT_HZ_TO_US(100U/*100Hz*/));
	Drv_StartTimerPwm(GetInstance_Timer3_P1() , TIM_CHANNEL_3);
#else
	/*Direction pin will be treated as HIGH SIDE mosfets*/
	HAL_GPIO_WritePin(PELTIER_DIR_CH0_GPIO_Port, PELTIER_DIR_CH0_Pin, GPIO_PIN_RESET);/*TURN OFF CH 0 - HIGH SIDE MOSFET*/
	HAL_GPIO_WritePin(PELTIER_DIR_CH1_GPIO_Port, PELTIER_DIR_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - HIGH SIDE MOSFET*/

	/*Signal pin will be treated as LOW SIDE mosfets*/
	HAL_GPIO_WritePin(PELTIER_SIG_CH0_GPIO_Port, PELTIER_SIG_CH0_Pin, GPIO_PIN_RESET);/*TURN OFF CH 0 - LOW SIDE MOSFET*/
	HAL_GPIO_WritePin(PELTIER_SIG_CH1_GPIO_Port, PELTIER_SIG_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - LOW SIDE MOSFET*/
#endif
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
PELTIER_DIR_STATE Drv_GetPeltierDirection(void)
{
	return (g_CurrDir);
}
#if (FALSE == PELTIER_PID_CONTROL)
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_ExecutePeltierDirection(PELTIER_DIR_STATE m_Dir)
{
	/*Signal pin will be treated as LOW SIDE mosfets*/
	HAL_GPIO_WritePin(PELTIER_SIG_CH0_GPIO_Port, PELTIER_SIG_CH0_Pin, GPIO_PIN_RESET);/*TURN OFF CH 0 - LOW SIDE MOSFET*/
	HAL_GPIO_WritePin(PELTIER_SIG_CH1_GPIO_Port, PELTIER_SIG_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - LOW SIDE MOSFET*/
	if(PELTIER_DIR_COOL == g_CurrDir)/*When cooling*/
	{
		HAL_GPIO_WritePin(PELTIER_DIR_CH0_GPIO_Port, PELTIER_DIR_CH0_Pin, GPIO_PIN_SET);/*TURN ON CH 0 - HIGH SIDE MOSFET*/
		HAL_GPIO_WritePin(PELTIER_DIR_CH1_GPIO_Port, PELTIER_DIR_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - HIGH SIDE MOSFET*/
	}
	else if(PELTIER_DIR_HEAT == g_CurrDir)/*When heating*/
	{
		HAL_GPIO_WritePin(PELTIER_DIR_CH0_GPIO_Port, PELTIER_DIR_CH0_Pin, GPIO_PIN_RESET);/*TURN OFF CH 0 - HIGH SIDE MOSFET*/
		HAL_GPIO_WritePin(PELTIER_DIR_CH1_GPIO_Port, PELTIER_DIR_CH1_Pin, GPIO_PIN_SET);/*TURN ON CH 1 - HIGH SIDE MOSFET*/
	}
	else
	{
		/*NOP*/
	}
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_SetPeltierDirection(PELTIER_DIR_STATE m_NewDir)
{
	(g_CurrDir = m_NewDir);
	HAL_GPIO_WritePin(PELTIER_SIG_CH0_GPIO_Port, PELTIER_SIG_CH0_Pin, GPIO_PIN_RESET);/*TURN OFF CH 0 - LOW SIDE MOSFET*/
	HAL_GPIO_WritePin(PELTIER_SIG_CH1_GPIO_Port, PELTIER_SIG_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - LOW SIDE MOSFET*/
	HAL_GPIO_WritePin(PELTIER_DIR_CH0_GPIO_Port, PELTIER_DIR_CH0_Pin, GPIO_PIN_RESET);/*TURN ON CH 0 - HIGH SIDE MOSFET*/
	HAL_GPIO_WritePin(PELTIER_DIR_CH1_GPIO_Port, PELTIER_DIR_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - HIGH SIDE MOSFET*/
}
#endif
#if (PELTIER_PID_CONTROL)
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_SetPeltierPower(PELTIER_DIR_STATE m_NewDir , uint8_t u8SpeedPercent)
#else
void Drv_SetPeltierPower(PELTIER_STATE m_St)
#endif
{
#if (PELTIER_PID_CONTROL)
	if(g_CurrDir == m_NewDir)
	{
		if(PELTIER_DIR_COOL == m_NewDir)
		{
			Drv_SetTimerPwmDutycycle(GetInstance_Timer1_P0() , TIM_CHANNEL_4 , u8SpeedPercent/*%*/);
		}
		else if(PELTIER_DIR_HEAT == m_NewDir)
		{
			Drv_SetTimerPwmDutycycle(GetInstance_Timer3_P1() , TIM_CHANNEL_3 , u8SpeedPercent/*%*/);
		}
		else/*Peltier Dir OFF*/
		{
			/*NOP*/
		}
	}
	else
	{
		if(TRUE == TimeOut_IsTimerRunning(&(g_BridgeSwitchTimer)))
		{
			if(TRUE == TimeOut_IsTimeout(&(g_BridgeSwitchTimer)))/*When timer is expired*/
			{
				if(PELTIER_DIR_COOL == m_NewDir)
				{
					HAL_GPIO_WritePin(PELTIER_DIR0_GPIO_Port, PELTIER_DIR0_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(PELTIER_DIR1_GPIO_Port, PELTIER_DIR1_Pin, GPIO_PIN_SET);
				}
				else if(PELTIER_DIR_HEAT == m_NewDir)
				{
					HAL_GPIO_WritePin(PELTIER_DIR0_GPIO_Port, PELTIER_DIR0_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(PELTIER_DIR1_GPIO_Port, PELTIER_DIR1_Pin, GPIO_PIN_RESET);
				}
				else/*Peltier Dir OFF*/
				{
					Drv_SetTimerPwmDutycycle(GetInstance_Timer1_P0() , TIM_CHANNEL_4 , 0U/*%*/);
					Drv_SetTimerPwmDutycycle(GetInstance_Timer3_P1() , TIM_CHANNEL_3 , 0U/*%*/);
				}
				TimeOut_Stop(&(g_BridgeSwitchTimer));
				g_CurrDir = m_NewDir;
			}
			else
			{
				/*NOP*/
			}
		}
		else/*If timer is not running - START TIMER AND TURN OFF BRIDGE*/
		{
			TimeOut_Init(&(g_BridgeSwitchTimer));
			TimeOut_Start(&(g_BridgeSwitchTimer) , PELTIER_DIR_SWITCH_HYSTERISIS_TIMEOUT_MS/*MS*/);
			/*Turn OFF all bridges*/
			Drv_SetTimerPwmDutycycle(GetInstance_Timer1_P0() , TIM_CHANNEL_4 , 0U/*%*/);
			Drv_SetTimerPwmDutycycle(GetInstance_Timer3_P1() , TIM_CHANNEL_3 , 0U/*%*/);
			HAL_GPIO_WritePin(PELTIER_DIR0_GPIO_Port, PELTIER_DIR0_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(PELTIER_DIR1_GPIO_Port, PELTIER_DIR1_Pin, GPIO_PIN_RESET);
		}
	}
#else
	g_CurrPeltierState = m_St;
	if(PELTIER_DIR_COOL == g_CurrDir)/*When cooling*/
	{
		HAL_GPIO_WritePin(PELTIER_SIG_CH1_GPIO_Port, PELTIER_SIG_CH1_Pin, g_CurrPeltierState);/*CONTROL CH 1 - HIGH SIDE MOSFET*/
	}
	else if(PELTIER_DIR_HEAT == g_CurrDir)/*When heating*/
	{
		HAL_GPIO_WritePin(PELTIER_SIG_CH0_GPIO_Port, PELTIER_SIG_CH0_Pin, g_CurrPeltierState);/*CONTROL CH 0 - HIGH SIDE MOSFET*/
	}
	else/*When system is OFF*/
	{

	}
#endif
}

