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
static TimerTimeOut g_DispRefreshTimer;
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
			  500, 0.02, 0.05,
			  _PID_P_ON_E, _PID_CD_REVERSE);

	PID_SetMode(&g_TPID_Cooler, _PID_MODE_AUTOMATIC);
	PID_SetSampleTime(&g_TPID_Cooler, PID_SAMPLE_TIME_MS/*Miiliseconds*/);
	PID_SetOutputLimits(&g_TPID_Cooler, 1/*%*/, 100U/*%*/);
	/*PID - COOLER - END*/

	Drv_SetSpeedExhaustFan(g_ApplCfg.u8ExFanSpeedPercent = 100U);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_TemperatureControl_Exe(void)
{
	PELTIER_DIR_STATE m_PeltierDir = Drv_GetPeltierDirection();
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
	PID_Compute(&g_TPID_Cooler);

	/*When NTC temp is greater than 25 deg celcuis - Perform COOLING*/
	if((TEMPERATURE_WINDUP_OFFSET + g_ApplCfg.fTargetTemperature) < g_ApplCfg.fNtcTemp)/*cooldown*/
	{
		g_ApplCfg.u8PeltierControlPercent =
				g_ApplCfg.u8ExFanSpeedPercent = (uint8_t)g_ApplCfg.fPidOutput_Cooler;
		Drv_SetPeltierPower(PELTIER_DIR_COOL , g_ApplCfg.u8PeltierControlPercent);
		PID_SetControllerDirection(&g_TPID_Cooler , _PID_CD_REVERSE);
	}
	/*When NTC temp is Lesser than 25 deg celcuis - Perform HEATING*/
	else if((g_ApplCfg.fTargetTemperature - TEMPERATURE_WINDUP_OFFSET) > g_ApplCfg.fNtcTemp)/*Heatup*/
	{
		g_ApplCfg.u8PeltierControlPercent =
				g_ApplCfg.u8ExFanSpeedPercent = (uint8_t)g_ApplCfg.fPidOutput_Cooler;
		Drv_SetPeltierPower(PELTIER_DIR_HEAT , g_ApplCfg.u8PeltierControlPercent);
		PID_SetControllerDirection(&g_TPID_Cooler , _PID_CD_DIRECT);
	}
	else
	{
		Drv_SetPeltierPower(PELTIER_DIR_OFF , g_ApplCfg.u8PeltierControlPercent);
	}
}
