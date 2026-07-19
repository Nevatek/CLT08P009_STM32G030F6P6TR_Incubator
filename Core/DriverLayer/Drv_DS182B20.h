/*
 * Drv_DS182B20.h
 *
 *  Created on: 19-Jul-2026
 *      Author: aldri
 */
#ifndef DRV_DS182B20_H_
#define DRV_DS182B20_H_

#include <stdint.h>

#define DS18B20_INVALID_TEMPERATURE    (999.0f)

uint8_t DS18B20_Start(void);
float DS18B20_ReadTemperature(void);
uint8_t DS18B20_GetSensorState(void);

#endif
