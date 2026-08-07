/*
 * HI_Adc.c
 *
 *  Created on: 28-Jul-2026
 *      Author: aldri
 */
#include "main.h"
#include "Datatype.h"
#include "HI_Adc.h"
#include "Timer.h"
#include "ScanTime.h"


#define MAX_ADC_BUFF_SIZE (100U)
ScanTime_Tag g_ScanAdc;
ScanTime_Tag g_ScanAdcAverage;
TimerTimeOut g_AdcScanTimer;
uint16_t g_16AdcBuffDataIndex;
uint16_t g_16AdcDataCnt;
uint16_t g_u16AdcBuff[MAX_ADC_BUFF_SIZE];
static uint16_t HI_ReadAdcChannel(void);
/*********************.HAL_GPIO_EXTI_Callback().*****************************
 .Purpose        : Callback for GPIO interrupt Rising and falling
 .Returns        :  RETURN_ERROR
					RETURN_SUCCESS
 .Note           :
 ****************************************************************************/
uint16_t HI_GetAdcValue_PollingBlocking(void)
{
	ScanTime_RecordStartRecordingScan(&(g_ScanAdcAverage) , TRUE);
	uint32_t u32AvgAdc = 0U;
	for(uint16_t u16idx = 0U ; u16idx < g_16AdcDataCnt ; ++u16idx)
	{
		/*Perform avergaing*/
		u32AvgAdc += g_u16AdcBuff[u16idx];
	}
	u32AvgAdc /= g_16AdcDataCnt;
	ScanTime_CalCulateScanTime(&(g_ScanAdcAverage));
	return (uint16_t)(u32AvgAdc);
}
void HI_AdcTaskAveraging_Init(void)
{
	g_16AdcBuffDataIndex = 0U;
	g_u16AdcBuff[g_16AdcBuffDataIndex++] = HI_ReadAdcChannel();
	g_16AdcDataCnt = g_16AdcBuffDataIndex;
}
void HI_AdcTaskAveraging(void)
{
	uint32_t u32AdcVal = 0U;
	if(TRUE == TimeOut_IsTimerRunning(&(g_AdcScanTimer)))/*If timer is running*/
	{
		if(TRUE == TimeOut_IsTimeout(&(g_AdcScanTimer)))/*When timer is expired*/
		{
			ScanTime_RecordStartRecordingScan(&(g_ScanAdc) , TRUE);

			g_u16AdcBuff[g_16AdcBuffDataIndex++] = u32AdcVal = HI_ReadAdcChannel();
			if(MAX_ADC_BUFF_SIZE <= g_16AdcBuffDataIndex)
			{
				g_16AdcDataCnt = MAX_ADC_BUFF_SIZE;
				g_16AdcBuffDataIndex = 0U;
			}
			else
			{
				if(MAX_ADC_BUFF_SIZE > g_16AdcDataCnt)
				{
					g_16AdcDataCnt = g_16AdcBuffDataIndex;
				}
				else
				{
					g_16AdcDataCnt = MAX_ADC_BUFF_SIZE;
				}
			}
			ScanTime_CalCulateScanTime(&(g_ScanAdc));
			TimeOut_Stop(&(g_AdcScanTimer));
		}
	}
	else
	{
		TimeOut_Start(&(g_AdcScanTimer) , 1U/*1 ms*/);
	}
}

uint16_t HI_ReadAdcChannel(void)
{
	ADC_HandleTypeDef *pAdc = GetInstance_ADC1();
	uint16_t u32AdcVal = 0U;
	/*Start the ADC conversion*/
	HAL_ADC_Start(pAdc);
	/*Wait for the conversion to complete*/
	HAL_ADC_PollForConversion(pAdc, HAL_MAX_DELAY);
	u32AdcVal = HAL_ADC_GetValue(pAdc);
	return u32AdcVal;
}
