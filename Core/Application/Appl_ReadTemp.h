/*
 * Appl_ReadTemp.h
 *
 *  Created on: 19-Jul-2026
 *      Author: aldri
 */

#ifndef APPLICATION_APPL_READTEMP_H_
#define APPLICATION_APPL_READTEMP_H_

#define TMP_SENS_NTC_10K (0U)
#define TMP_SENS_DS18B20 (1U)

#define TEMPERATURE_SENS_TYPE (TMP_SENS_NTC_10K)

void Appl_InitTemperatureSensor(void);
float Appl_ReadTemperature_InCelcius(void);
uint8_t Appl_WaitAndGetState_TempSensor(void);
#endif /* APPLICATION_APPL_READTEMP_H_ */
