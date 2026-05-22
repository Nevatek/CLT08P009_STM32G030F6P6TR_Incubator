/*
 * ApplicationLayer.h
 *
 *  Created on: 29-Mar-2026
 *      Author: aldrin.rebellow
 */

#ifndef APPLICATION_APPLICATIONLAYER_H_
#define APPLICATION_APPLICATIONLAYER_H_

#define DISPLAY_REFRESH_TIMEOUT_MS 					(5000U)
#define INCUBATOR_TARGET_TEMPERATURE 				(25U)/*25 Degree Celcuis*/

#define TEMPERATURE_COOL_WINDUP_OFFSET				(0.1)/*0.3 Degree Celcuis*/
#define TEMPERATURE_HEAT_WINDUP_OFFSET				(0.1)/*0.3 Degree Celcuis*/

#define TEMPERATURE_COOL_WINDUP_DIR_SW_OFFSET		(5)/*max offset from target temp at which direction changes to heat*/
#define TEMPERATURE_HEAT_WINDUP_DIR_SW_OFFSET		(5)/*max offset from target temp at which direction changes to cool*/

#define PID_SAMPLE_TIME_MS 							(50U)

#define MIN_VALID_NTC_TEMPERATURE 					(-25)
#define MAX_VALID_NTC_TEMPERATURE 					(+85)

#define DISPLAY_ACCURACY_OFFSET_MIN					(0.1)
#define DISPLAY_ACCURACY_OFFSET_MAX					(0.7)

#define PELTIER_PID_CONTROL 						(FALSE)

#define ERROR_CODE_INVALID_NTC_TEMP					(10U)

typedef struct
{
	uint16_t u8ExFanSpeedPercent;
	uint16_t u8PeltierControlPercent;
	float fTargetTemperature;
	float fNtcTemp;
	float fPidOutput_Heater;
	float fPidOutput_Cooler;
	uint8_t u8TempAchievedFlag;
}APPL;

APPL* GetInstance_ApplCfg(void);
void ApplicationLayer_Init(void);
void ApplicationLayer_Exe(void);
void ApplicationLayer_TemperatureControl_Init(void);
void ApplicationLayer_TemperatureControl_Exe(void);
#endif /* APPLICATION_APPLICATIONLAYER_H_ */
