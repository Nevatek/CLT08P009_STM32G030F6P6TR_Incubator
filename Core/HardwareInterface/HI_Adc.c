/*
 * HI_Adc.c
 *
 *  Created on: 28-Jul-2026
 *      Author: aldri
 */
#include "main.h"
#include "Datatype.h"
#include "HI_Adc.h"
#include "ScanTime.h"

ScanTime_Tag g_ScanAdc;
/*********************.HAL_GPIO_EXTI_Callback().*****************************
 .Purpose        : Callback for GPIO interrupt Rising and falling
 .Returns        :  RETURN_ERROR
					RETURN_SUCCESS
 .Note           :
 ****************************************************************************/
uint16_t HI_GetAdcValue_PollingBlocking(void)
{
	uint32_t u32AdcVal = 0U;
	ADC_HandleTypeDef *pAdc = GetInstance_ADC1();

	ScanTime_RecordStartRecordingScan(&(g_ScanAdc) , TRUE);
	/*Start the ADC conversion*/
	HAL_ADC_Start(pAdc);
	/*Wait for the conversion to complete*/
	HAL_ADC_PollForConversion(pAdc, HAL_MAX_DELAY);
	u32AdcVal = HAL_ADC_GetValue(pAdc);
	ScanTime_CalCulateScanTime(&(g_ScanAdc));
	return (uint16_t)(u32AdcVal);
}
