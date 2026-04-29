/*
 * ApplicationLayer.h
 *
 *  Created on: 29-Mar-2026
 *      Author: aldrin.rebellow
 */

#ifndef APPLICATION_APPLICATIONLAYER_H_
#define APPLICATION_APPLICATIONLAYER_H_

#define DISPLAY_REFRESH_TIMEOUT_MS (100U)

typedef struct
{
	float fNtcTemp_A;
	uint8_t u8ExFanSpeedPercent;
	uint8_t u8PeltierControlPercent;
}APPL;

APPL* GetInstance_ApplCfg(void);
void ApplicationLayer_Init(void);
void ApplicationLayer_Exe(void);
void ApplicationLayer_TemperatureControl_Init(void);
void ApplicationLayer_TemperatureControl_Exe(void);
#endif /* APPLICATION_APPLICATIONLAYER_H_ */
