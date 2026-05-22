/*
 * Drv_Thermistor.c
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */
#include "main.h"
#include "math.h"
#include "Datatype.h"
#include "ErrorCode.h"
#include "Timer.h"
#include "Drv_Thermistor.h"

#define MAX_ADC_DATA_CNT 	(100U)

static TimerTimeOut g_TempAvgTimer;
static float fCurrTempC = 0.0f;
volatile static uint16_t u16ADCVal;
static uint8_t g_u8DataCnt;
static float fDataBuffer[MAX_ADC_DATA_CNT];
static uint8_t u8SampelCnt = 0U;
static uint8_t u8AdcRxFlag = FALSE;
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(FALSE == u8AdcRxFlag)
	{
		u16ADCVal = HAL_ADC_GetValue(GetInstance_ADC1());
		u8AdcRxFlag = TRUE;
	}
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_Thermistor_Init(void)
{
	TimeOut_Init(&(g_TempAvgTimer));
	TimeOut_Start(&(g_TempAvgTimer) , NTC_TEMP_AVG_SAMPLE_TIMEOUT_MS);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_Thermistor_Exe(void)
{
	if(TRUE == u8AdcRxFlag)
	{
		/*Copy temperature to buffer*/
		fDataBuffer[g_u8DataCnt++] = Drv_ReadThermistor_TemperatureInCelcius();
		u8SampelCnt++;
		if((MAX_ADC_DATA_CNT) <= g_u8DataCnt)
		{
			g_u8DataCnt = 0U;
		}
		if((MAX_ADC_DATA_CNT) <= u8SampelCnt)
		{
			u8SampelCnt = MAX_ADC_DATA_CNT;
		}

		/*Perform average*/
		fCurrTempC = 0.0f;
		for(uint8_t u8nI = 0U ; u8nI < u8SampelCnt ; ++u8nI)
		{
			fCurrTempC += fDataBuffer[u8nI];
		}
		fCurrTempC /= u8SampelCnt;
		u8AdcRxFlag = FALSE;
	}

	if(TRUE == TimeOut_IsTimerRunning(&(g_TempAvgTimer)))
	{
		if(TRUE == TimeOut_IsTimeout(&(g_TempAvgTimer)))
		{
			HAL_ADC_Start_IT(GetInstance_ADC1());
		}
		else
		{
			/*NOP*/
		}
	}
	else/*If timer is not running*/
	{
		TimeOut_Init(&(g_TempAvgTimer));
		TimeOut_Start(&(g_TempAvgTimer) , NTC_TEMP_AVG_SAMPLE_TIMEOUT_MS);
	}
}

/****************************** FUNCTION_HEADER ******************************
.Purpose : Function for converting Temperature from Kelvin to Farenheit
.Returns : Temperature in Farenheit
.Note    :
******************************************************************************/
float Drv_GetCurrentTemperature(void)
{
	return (fCurrTempC);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
float Drv_ReadThermistor_TemperatureInCelcius(void)
{
    // Voltage divider calculation
    float resistance = (SERIES_RESISTOR * ((MAX_VAL_ADC) / u16ADCVal)) - (SERIES_RESISTOR);

    float resistance_ratio = resistance / THERMISTOR_NOMINAL_RESISTANCE;
    float temperatureK = (BETA_VALUE * STANDARD_ROOM_TEMP_KELVIN) /
                         (BETA_VALUE + (STANDARD_ROOM_TEMP_KELVIN * logf(resistance_ratio)));

	return ((temperatureK - 273.15f));
}
