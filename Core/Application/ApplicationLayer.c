/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Application.c
.Summary : Application layer main header file
.Note :

Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
/*Aldrin.Rebellow 29/03/2026 Initial Release*/
#include "main.h"
#include "math.h"
#include "Datatype.h"
#include "Timer.h"
#include "pid.h"
#include "Drv_ET6226.h"
#include "Drv_ExhaustFan.h"
#include "Drv_Thermistor.h"
#include "Drv_Peltier.h"
#include "ApplicationLayer.h"

static APPL g_ApplCfg;
static PID_TypeDef g_TPID_Cooler;
static PID_TypeDef g_TPID_Heater;
static TimerTimeOut g_DispRefreshTimer;
static TimerTimeOut g_TimerPID;
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
APPL* GetInstance_ApplCfg(void)
{
	return (&(g_ApplCfg));
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_Init(void)
{
	ET6226M_Init();
	Drv_Thermistor_Init();
	Drv_InitilizeExhaustFan();
	Drv_Peltier_Init();
	ApplicationLayer_TemperatureControl_Init();
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_Exe(void)
{
	Drv_Thermistor_Exe();
	ET6226M_RefreshDisplay();
	ApplicationLayer_TemperatureControl_Exe();
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer22
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_TemperatureControl_Init(void)
{
	TimeOut_Init(&(g_DispRefreshTimer));
	TimeOut_Start(&(g_DispRefreshTimer) , DISPLAY_REFRESH_TIMEOUT_MS);

	g_ApplCfg.fTargetTemperature 	= INCUBATOR_TARGET_TEMPERATURE;

	/*PID - COOLER - START*/
	PID(&g_TPID_Cooler, &g_ApplCfg.fNtcTemp,
			&g_ApplCfg.fPidOutput_Cooler,
			&g_ApplCfg.fTargetTemperature,
			  0.5, 0.5, 0.5,
			  _PID_P_ON_M, _PID_CD_REVERSE);

	PID_SetMode(&g_TPID_Cooler, _PID_MODE_AUTOMATIC);
	PID_SetSampleTime(&g_TPID_Cooler, PID_SAMPLE_TIME_MS/*Miiliseconds*/);
	PID_SetOutputLimits(&g_TPID_Cooler, 1/*%*/, 100/*%*/);
	/*PID - COOLER - END*/

	/*PID - HEATER - START*/
	PID(&g_TPID_Heater, &g_ApplCfg.fNtcTemp,
			&g_ApplCfg.fPidOutput_Heater,
			&g_ApplCfg.fTargetTemperature,
			  0.5, 0.5, 0.5,
			  _PID_P_ON_M, _PID_CD_DIRECT);

	PID_SetMode(&g_TPID_Heater, _PID_MODE_AUTOMATIC);
	PID_SetSampleTime(&g_TPID_Heater, PID_SAMPLE_TIME_MS/*Miiliseconds*/);
	PID_SetOutputLimits(&g_TPID_Heater, 1/*%*/, 100/*%*/);
	/*PID - HEATER - END*/

	TimeOut_Init(&(g_TimerPID));
	TimeOut_Start(&(g_TimerPID) , PID_SAMPLE_TIME_MS);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_TemperatureControl_Exe(void)
{
	/*Display control - START*/
	g_ApplCfg.fNtcTemp = Drv_GetCurrentTemperature();
	if(TRUE == TimeOut_IsTimerRunning(&(g_DispRefreshTimer)))
	{
		if(TRUE == TimeOut_IsTimeout(&(g_DispRefreshTimer)))
		{
			ET6226M_DisplayNumber(g_ApplCfg.fNtcTemp * 10);
		}
		else
		{
			/*NOP*/
		}
	}
	else
	{
		TimeOut_Restart(&(g_DispRefreshTimer));
	}
	/*Display control - END*/

	/*Calculate delta change in temperature*/
	if(TRUE == TimeOut_IsTimerRunning(&(g_TimerPID)))
	{
		if(TRUE == TimeOut_IsTimeout(&(g_TimerPID)))
		{
			PID_Compute(&g_TPID_Cooler);
			PID_Compute(&g_TPID_Heater);

			if((g_ApplCfg.fNtcTemp >
						(TEMPERATURE_WINDUP_OFFSET + g_ApplCfg.fTargetTemperature)))/*cooldown*/
			{
				g_ApplCfg.u8PeltierControlPercent =
						g_ApplCfg.u8ExFanSpeedPercent = (uint8_t)g_ApplCfg.fPidOutput_Cooler;
				Drv_SetPeltierPower(PELTIER_DIR_COOL , g_ApplCfg.u8PeltierControlPercent);
			}
			else if((g_ApplCfg.fNtcTemp <
					(g_ApplCfg.fTargetTemperature - TEMPERATURE_WINDUP_OFFSET)))/*Heatup*/
			{
				g_ApplCfg.u8PeltierControlPercent =
						g_ApplCfg.u8ExFanSpeedPercent = (uint8_t)g_ApplCfg.fPidOutput_Heater;
				Drv_SetPeltierPower(PELTIER_DIR_HEAT , g_ApplCfg.u8PeltierControlPercent);
			}
			else
			{
				Drv_SetPeltierPower(PELTIER_DIR_OFF , g_ApplCfg.u8PeltierControlPercent);
			}
			if(MIN_EXHAUST_FAN_SPEED_PERCENT > g_ApplCfg.u8ExFanSpeedPercent)
			{
				g_ApplCfg.u8ExFanSpeedPercent = MIN_EXHAUST_FAN_SPEED_PERCENT;
			}
			Drv_SetSpeedExhaustFan(g_ApplCfg.u8ExFanSpeedPercent);
		}
	}
	else
	{
		TimeOut_Restart(&(g_TimerPID));
	}
}
