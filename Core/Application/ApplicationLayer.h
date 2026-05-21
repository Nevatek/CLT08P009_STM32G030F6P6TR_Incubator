/*
 * ApplicationLayer.h
 *
 *  Created on: 29-Mar-2026
 *      Author: aldrin.rebellow
 */

#ifndef APPLICATION_APPLICATIONLAYER_H_
#define APPLICATION_APPLICATIONLAYER_H_

#define DISPLAY_REFRESH_TIMEOUT_MS 		(5000U)
#define INCUBATOR_TARGET_TEMPERATURE 	(25U)/*25 Degree Celcuis*/

#define TEMPERATURE_WINDUP_OFFSET	 	(0U)/*0.3 Degree Celcuis*/
#define PID_SAMPLE_TIME_MS 				(50U)

typedef struct
{
	uint16_t u8ExFanSpeedPercent;
	uint16_t u8PeltierControlPercent;
	float fTargetTemperature;
	float fNtcTemp;
	float fPidOutput_Heater;
	float fPidOutput_Cooler;
}APPL;

APPL* GetInstance_ApplCfg(void);
void ApplicationLayer_Init(void);
void ApplicationLayer_Exe(void);
void ApplicationLayer_TemperatureControl_Init(void);
void ApplicationLayer_TemperatureControl_Exe(void);
#endif /* APPLICATION_APPLICATIONLAYER_H_ */
