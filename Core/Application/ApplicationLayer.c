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
#include "Datatype.h"
#include "Timer.h"
#include "Drv_ET6226.h"
#include "Drv_ExhaustFan.h"
#include "Drv_Thermistor.h"
#include "ApplicationLayer.h"

static APPL g_ApplCfg;
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
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_TemperatureControl_Init(void)
{
	TimeOut_Init(&(g_DispRefreshTimer));
	TimeOut_Start(&(g_DispRefreshTimer) , DISPLAY_REFRESH_TIMEOUT_MS);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_TemperatureControl_Exe(void)
{
	/*Display control - START*/
	g_ApplCfg.fNtcTemp_A = Drv_GetCurrentTemperature();
	if(TRUE == TimeOut_IsTimerRunning(&(g_DispRefreshTimer)))
	{
		if(TRUE == TimeOut_IsTimeout(&(g_DispRefreshTimer)))
		{
			ET6226M_DisplayNumber(g_ApplCfg.fNtcTemp_A * 10);
		}
		else
		{
			/*NOP*/
		}
	}
	else
	{
		TimeOut_Init(&(g_DispRefreshTimer));
		TimeOut_Start(&(g_DispRefreshTimer) , DISPLAY_REFRESH_TIMEOUT_MS);
	}
	/*Display control - END*/
}
