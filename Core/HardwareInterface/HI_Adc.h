/*
 * HI_Adc.h
 *
 *  Created on: 28-Jul-2026
 *      Author: aldri
 */

#ifndef HARDWAREINTERFACE_HI_ADC_H_
#define HARDWAREINTERFACE_HI_ADC_H_

uint16_t HI_GetAdcValue_PollingBlocking(void);
void HI_AdcTaskAveraging_Init(void);
void HI_AdcTaskAveraging(void);
#endif /* HARDWAREINTERFACE_HI_ADC_H_ */
