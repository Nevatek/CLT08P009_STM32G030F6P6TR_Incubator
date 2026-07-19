/*
 * Appl_ReadTemp.h
 *
 *  Created on: 19-Jul-2026
 *      Author: aldri
 */

#ifndef APPLICATION_APPL_READTEMP_H_
#define APPLICATION_APPL_READTEMP_H_


void Appl_InitTemperatureSensor(void);
uint8_t Appl_ReadTemperature_InCelcius(float *fTmpC);
uint8_t Appl_GetTempSensAvailStatus(void);
#endif /* APPLICATION_APPL_READTEMP_H_ */
