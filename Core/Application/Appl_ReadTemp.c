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
#include "Appl_ReadTemp.h"

TimerTimeOut g_Tim;
ErrorCode Appl_WaitAndGetStatus_DS18B20(void);

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Appl_InitTemperatureSensor(void)
{

}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
uint8_t Appl_ReadTemperature_InCelcius(float *fTmpC)
{
	uint8_t u8Status = 0U;
	return (u8Status);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
uint8_t Appl_GetTempSensAvailStatus(void)
{
	return (FALSE);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
ErrorCode Appl_WaitAndGetStatus_DS18B20(void)
{
	ErrorCode err = ErrorCode_Success;

	return (err);
}
