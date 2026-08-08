/*
 * ApplicationLayer.h
 *
 *  Created on: 29-Mar-2026
 *      Author: aldrin.rebellow
 */

#ifndef APPLICATION_APPLICATIONLAYER_H_
#define APPLICATION_APPLICATIONLAYER_H_

#define DISPLAY_REFRESH_TIMEOUT_MS 					(5000U)
#define PID_TRIGGER_TIMEOUT_MS 						(10U)
#define INCUBATOR_TARGET_TEMPERATURE 				(25U)/*25 Degree Celcuis*/

#define TEMPERATURE_HEAT_WINDUP_OFFSET				(0.85)/*minus Degree Celcuis*/
#define TEMPERATURE_COOL_WINDUP_OFFSET				(0.35)/*plus Degree Celcuis*/

#define TEMPERATURE_COOL_WINDUP_DIR_SW_OFFSET		(1.0)/*max offset from target temp at which direction changes to heat*/
#define TEMPERATURE_HEAT_WINDUP_DIR_SW_OFFSET		(1.0)/*max offset from target temp at which direction changes to cool*/

#define PID_SAMPLE_TIME_MS 							(50U)

#define MIN_VALID_NTC_TEMPERATURE 					(-10)
#define MAX_VALID_NTC_TEMPERATURE 					(+85)

#define DISPLAY_ACCURACY_OFFSET_MIN					(0.19)
#define DISPLAY_ACCURACY_OFFSET_MAX					(1.5)

#define PELTIER_PID_CONTROL 						(FALSE)

#define ERROR_CODE_INVALID_NTC_TEMP					(10U)

typedef struct
{
	float fTargetTemperature;
	float fNtcTemp_Ambient;
	float fNtcTemp_Block;
	uint8_t u8TempAchievedFlag;
	GPIO_PinState m_PelPin_Dir0;
	GPIO_PinState m_PelPin_Dir1;
	GPIO_PinState m_PelPin_Sig0;
	GPIO_PinState m_PelPin_Sig1;
}APPL;

APPL* GetInstance_ApplCfg(void);
void ApplicationLayer_Init(void);
void ApplicationLayer_Exe(void);
void ApplicationLayer_TemperatureControl_Init(void);
void ApplicationLayer_TemperatureControl_Exe(void);
#endif /* APPLICATION_APPLICATIONLAYER_H_ */
