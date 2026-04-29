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

#define MAX_ADC_DATA_CNT 	(10U)

static TimerTimeOut g_TempAvgTimer;
static float fCurrTempC = 0.0f;
static uint16_t u16ADCVal;
static uint8_t g_u8DataCnt;
static float fDataBuffer[MAX_ADC_DATA_CNT];
static float ConvertkelvinToCelcuis(float temperatureK);
static float Drv_ReadThermistor_TemperatureInCelcius(void);
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_Thermistor_Init(void)
{
	ADC_HandleTypeDef *pAdc = GetInstance_ADC1();
	HAL_ADC_Start(pAdc);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_Thermistor_Exe(void)
{
	volatile float fTemp = 0.0f;
	volatile uint32_t u32Temp = 0U;

	if(TRUE == TimeOut_IsTimerRunning(&(g_TempAvgTimer)))
	{
		if(TRUE == TimeOut_IsTimeout(&(g_TempAvgTimer)))
		{
			fTemp = Drv_ReadThermistor_TemperatureInCelcius();

			/*Copy temperature to buffer*/
			fDataBuffer[g_u8DataCnt++] = fTemp;
			if((MAX_ADC_DATA_CNT) <= g_u8DataCnt)
			{
				g_u8DataCnt = 0U;
			}

			/*Perform average*/
			fCurrTempC = 0.0f;
			for(uint8_t u8nI = 0U ; u8nI < MAX_ADC_DATA_CNT ; ++u8nI)
			{
				fCurrTempC += fDataBuffer[u8nI];
			}
			fCurrTempC /= MAX_ADC_DATA_CNT;
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
/****************************** FUNCTION_HEADER ******************************
.Purpose : Function for converting Temperature from Kelvin to Farenheit
.Returns : Temperature in Farenheit
.Note    :
******************************************************************************/
float ConvertkelvinToCelcuis(float temperatureK)
{
    float fTempCelcuis = 0.0f;
    fTempCelcuis = temperatureK - 273.15f ;//conversion  of temperature to celcuis
    return (fTempCelcuis);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
float Drv_ReadThermistor_TemperatureInCelcius(void)
{
	float fTempC = 0.0f;
	ADC_HandleTypeDef *pAdc = GetInstance_ADC1();
	HAL_ADC_PollForConversion(pAdc, 1U/*ADC Timeout in 1 millisecond*/);
	u16ADCVal = HAL_ADC_GetValue(pAdc);/*Read ADC Value*/
    // Voltage divider calculation
    volatile float resistance = (SERIES_RESISTOR * ((MAX_VAL_ADC) / u16ADCVal)) -
                                        						(SERIES_RESISTOR);

    volatile float resistance_ratio = resistance / THERMISTOR_NOMINAL_RESISTANCE;
    volatile float resistance_log = logf(resistance_ratio);

    float temperatureK = (BETA_VALUE * STANDARD_ROOM_TEMP_KELVIN) /
                         (BETA_VALUE + (STANDARD_ROOM_TEMP_KELVIN * resistance_log));


    fTempC = ConvertkelvinToCelcuis(temperatureK);
	return (fTempC);
}
