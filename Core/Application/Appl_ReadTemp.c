/*
 * Appl_ReadTemp.c
 *
 *  Created on: 19-Jul-2026
 *      Author: aldri
 */

#include "main.h"
#include "Datatype.h"
#include "ErrorCode.h"
#include "Timer.h"
#include "Drv_NTC10K.h"
#include "Appl_ReadTemp.h"

TimerTimeOut g_Tim;
uint8_t g_8SensorAvailable = FALSE;
ErrorCode Appl_WaitAndGetStatus_TempSensor(void);

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Appl_InitTemperatureSensor(void)
{
#if (TMP_SENS_DS18B20 == TEMPERATURE_SENS_TYPE)
	g_8SensorAvailable = DS18B20_Start();
#else
	HAL_ADCEx_Calibration_Start(GetInstance_ADC1());
#endif
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
float Appl_ReadTemperature_InCelcius(void)
{
	float fTemperature = 0.0f;
#if (TMP_SENS_DS18B20 == TEMPERATURE_SENS_TYPE)
	fTemperature = DS18B20_ReadTemperature();
#else
	fTemperature = Drv_Ntc10K_GetTemperatureDegreeCelcius();
#endif
	return (fTemperature);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
uint8_t Appl_GetTempSensAvailStatus(void)
{
	return (g_8SensorAvailable);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
uint8_t Appl_WaitAndGetState_TempSensor(void)
{
	uint8_t err = FALSE;
#if (TMP_SENS_DS18B20 == TEMPERATURE_SENS_TYPE)
	err = DS18B20_GetSensorState();
#else
	err = TRUE;
#endif
	return (err);
}
