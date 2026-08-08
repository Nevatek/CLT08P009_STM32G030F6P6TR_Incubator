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
#include "HI_Adc.h"
#if (PELTIER_PID_CONTROL)
#include "pid.h"
#endif
#include "Drv_ET6226.h"
#include "Drv_ExhaustFan.h"
#include "Drv_Peltier.h"
#include "Appl_ReadTemp.h"
#include "ApplicationLayer.h"

#if (PELTIER_PID_CONTROL)
static PID_TypeDef g_TPID_Cooler;
#endif
static TimerTimeOut g_PeltierDirSwitchTimer;
static APPL g_ApplCfg;
static TimerTimeOut g_DispRefreshTimer;
static TimerTimeOut g_PidTriggerTimer;
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
	Drv_InitilizeExhaustFan();
	Drv_Peltier_Init();
	Appl_InitTemperatureSensor();
	HAL_Delay(500U);/*10 MS startup delay*/
	HI_AdcTaskAveraging_Init();
	ApplicationLayer_TemperatureControl_Init();
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_Exe(void)
{
	HI_AdcTaskAveraging();
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

	g_ApplCfg.fTargetTemperature = INCUBATOR_TARGET_TEMPERATURE;
#if (PELTIER_PID_CONTROL)
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
#else
	Drv_SetPeltierPower(PELTIER_ST_OFF);
	g_ApplCfg.fNtcTemp_Block = Appl_ReadTemperature_InCelcius();
	if(g_ApplCfg.fTargetTemperature <= g_ApplCfg.fNtcTemp_Block)
	{
		Drv_SetPeltierDirection(PELTIER_DIR_COOL);/*COOL now*/
	}
	else
	{
		Drv_SetPeltierDirection(PELTIER_DIR_HEAT);/*HEAT now*/
	}
	TimeOut_Init(&(g_PidTriggerTimer));
	TimeOut_Start(&(g_PidTriggerTimer) , PID_TRIGGER_TIMEOUT_MS);
#endif
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ApplicationLayer_TemperatureControl_Exe(void)
{
	PELTIER_DIR_STATE m_PeltierDir = Drv_GetPeltierDirection();

	if(TRUE == TimeOut_IsTimerRunning(&(g_PidTriggerTimer)))
	{
		if(TRUE == TimeOut_IsTimeout(&(g_PidTriggerTimer)))
		{
			g_ApplCfg.fNtcTemp_Block = Appl_ReadTemperature_InCelcius();
#if (PELTIER_PID_CONTROL)
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
#else
			/*If NTC temperature is VALID*/
			if(TRUE == Appl_WaitAndGetState_TempSensor()
					&& MIN_VALID_NTC_TEMPERATURE < g_ApplCfg.fNtcTemp_Block &&
					MAX_VALID_NTC_TEMPERATURE > g_ApplCfg.fNtcTemp_Block )
			{
				if(PELTIER_DIR_COOL == m_PeltierDir)/*If current dir is COOLING*/
				{
					/*Wait for temperature of NTC fall below target - windup temperature*/
					if((g_ApplCfg.fTargetTemperature - (TEMPERATURE_COOL_WINDUP_DIR_SW_OFFSET)) >= g_ApplCfg.fNtcTemp_Block)
					{
						if(TRUE == TimeOut_IsTimerRunning(&(g_PeltierDirSwitchTimer)))/*If timer is running*/
						{
							if(TRUE == TimeOut_IsTimeout(&(g_PeltierDirSwitchTimer)))/*When timer is expired*/
							{
								Drv_SetPeltierDirection(PELTIER_DIR_HEAT);/*Switch to HEAT now*/
								TimeOut_Stop(&(g_PeltierDirSwitchTimer));
							}
						}
						else/*If timer is not running*/
						{
							TimeOut_Init(&(g_PeltierDirSwitchTimer));/*Initilize timer*/
							TimeOut_Start(&(g_PeltierDirSwitchTimer) , PELTIER_DIR_SWITCH_HYSTERISIS_TIMEOUT_MS/*MS*/);/*Start Timer*/
							/*Switch to DIR OFF now*/
							HAL_GPIO_WritePin(PELTIER_DIR_CH0_GPIO_Port, PELTIER_DIR_CH0_Pin, GPIO_PIN_RESET);/*TURN OFF CH 0 - HIGH SIDE MOSFET*/
							HAL_GPIO_WritePin(PELTIER_DIR_CH1_GPIO_Port, PELTIER_DIR_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - HIGH SIDE MOSFET*/
						}
					}
					/*If temperature of NTC is between target and cool windup*/
					else if((g_ApplCfg.fTargetTemperature + TEMPERATURE_COOL_WINDUP_OFFSET) >=
								g_ApplCfg.fNtcTemp_Block)
					{
						/*Turn OFF - SIGNAL FET*/
						Drv_SetPeltierPower(PELTIER_ST_OFF);
					}
					else/*If NTC temperature is above target - COOL*/
					{
						Drv_ExecutePeltierDirection(PELTIER_DIR_COOL);
						Drv_SetPeltierPower(PELTIER_ST_ON);
					}
				}
				else if(PELTIER_DIR_HEAT == m_PeltierDir)/*If current dir is HEATING*/
				{
					/*Wait for temperature of NTC fall above target + windup temperature*/
					if((g_ApplCfg.fTargetTemperature + TEMPERATURE_HEAT_WINDUP_DIR_SW_OFFSET) <=
																						g_ApplCfg.fNtcTemp_Block)
					{
						if(TRUE == TimeOut_IsTimerRunning(&(g_PeltierDirSwitchTimer)))/*If timer is running*/
						{
							if(TRUE == TimeOut_IsTimeout(&(g_PeltierDirSwitchTimer)))/*When timer is expired*/
							{
								Drv_SetPeltierDirection(PELTIER_DIR_COOL);/*Switch to COOL now*/
								TimeOut_Stop(&(g_PeltierDirSwitchTimer));
							}
						}
						else/*If timer is not running*/
						{
							TimeOut_Init(&(g_PeltierDirSwitchTimer));/*Initilize timer*/
							TimeOut_Start(&(g_PeltierDirSwitchTimer) , PELTIER_DIR_SWITCH_HYSTERISIS_TIMEOUT_MS/*MS*/);/*Start Timer*/
							/*Switch to DIR OFF now*/
							HAL_GPIO_WritePin(PELTIER_DIR_CH0_GPIO_Port, PELTIER_DIR_CH0_Pin, GPIO_PIN_RESET);/*TURN OFF CH 0 - HIGH SIDE MOSFET*/
							HAL_GPIO_WritePin(PELTIER_DIR_CH1_GPIO_Port, PELTIER_DIR_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - HIGH SIDE MOSFET*/
						}
					}
					/*If temperature of NTC is between target and heat windup*/
					else if((g_ApplCfg.fTargetTemperature - TEMPERATURE_HEAT_WINDUP_OFFSET) <=
										g_ApplCfg.fNtcTemp_Block)
					{
						/*Turn OFF - SIGNAL FET*/
						Drv_SetPeltierPower(PELTIER_ST_OFF);
					}
					else/*If NTC temperature is below target - HEAT*/
					{

						Drv_ExecutePeltierDirection(PELTIER_DIR_HEAT);
						Drv_SetPeltierPower(PELTIER_ST_ON);
					}
				}
				else
				{
					/*NOP*/
				}
			}
			/*If NTC temperature is NOT VALID*/
			else
			{
				/*Turn OFF - SIGNAL FET*/
				HAL_GPIO_WritePin(PELTIER_SIG_CH0_GPIO_Port, PELTIER_SIG_CH0_Pin, GPIO_PIN_RESET);/*TURN OFF CH 0 - LOW SIDE MOSFET*/
				HAL_GPIO_WritePin(PELTIER_SIG_CH1_GPIO_Port, PELTIER_SIG_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - LOW SIDE MOSFET*/
				/*Switch to DIR OFF now*/
				HAL_GPIO_WritePin(PELTIER_DIR_CH0_GPIO_Port, PELTIER_DIR_CH0_Pin, GPIO_PIN_RESET);/*TURN OFF CH 0 - HIGH SIDE MOSFET*/
				HAL_GPIO_WritePin(PELTIER_DIR_CH1_GPIO_Port, PELTIER_DIR_CH1_Pin, GPIO_PIN_RESET);/*TURN OFF CH 1 - HIGH SIDE MOSFET*/

				ET6226M_DisplayErrorCode(ERROR_CODE_INVALID_NTC_TEMP);
			}
		}
	}
	else
	{
		TimeOut_Restart(&(g_PidTriggerTimer));
	}

/*Display control - START*/
	if(TRUE == TimeOut_IsTimerRunning(&(g_DispRefreshTimer)))
	{
		if(TRUE == TimeOut_IsTimeout(&(g_DispRefreshTimer)))
		{
			/*If temperature of NTC is in most accuracy region, Then display actual NTC temperature*/
			if(g_ApplCfg.fNtcTemp_Block <= (g_ApplCfg.fTargetTemperature + DISPLAY_ACCURACY_OFFSET_MIN)
					&& g_ApplCfg.fNtcTemp_Block >= (g_ApplCfg.fTargetTemperature - DISPLAY_ACCURACY_OFFSET_MIN))
			{
				ET6226M_DisplayNumber_F(g_ApplCfg.fNtcTemp_Block);
				g_ApplCfg.u8TempAchievedFlag = TRUE;/*Temperature achieved its target accuracy region*/
			}
			/*If temperature of NTC is out of accuracy region, Then display actual NTC temperature*/
			else
			{
				/*Exceded out of max accuracy region*/
				if(g_ApplCfg.fNtcTemp_Block > (g_ApplCfg.fTargetTemperature + DISPLAY_ACCURACY_OFFSET_MAX)
										|| g_ApplCfg.fNtcTemp_Block < (g_ApplCfg.fTargetTemperature - DISPLAY_ACCURACY_OFFSET_MAX))
				{
					ET6226M_DisplayNumber_F(g_ApplCfg.fNtcTemp_Block);
					g_ApplCfg.u8TempAchievedFlag = FALSE;/*Temperature over shoots its target accuracy region*/
				}
				else
				{
					/*Just out of accuracy region, but temp achieved flag not enabled*/
					if(FALSE == g_ApplCfg.u8TempAchievedFlag)
					{
						ET6226M_DisplayNumber_F(g_ApplCfg.fNtcTemp_Block);
					}
					else
					{
						/*NOP*/
					}
				}
			}
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
	g_ApplCfg.m_PelPin_Dir0 = HAL_GPIO_ReadPin(PELTIER_DIR_CH0_GPIO_Port, PELTIER_DIR_CH0_Pin);
	g_ApplCfg.m_PelPin_Dir1 = HAL_GPIO_ReadPin(PELTIER_DIR_CH1_GPIO_Port, PELTIER_DIR_CH1_Pin);
	g_ApplCfg.m_PelPin_Sig0 = HAL_GPIO_ReadPin(PELTIER_SIG_CH0_GPIO_Port, PELTIER_SIG_CH0_Pin);
	g_ApplCfg.m_PelPin_Sig1 = HAL_GPIO_ReadPin(PELTIER_SIG_CH1_GPIO_Port, PELTIER_SIG_CH1_Pin);
#endif
}
