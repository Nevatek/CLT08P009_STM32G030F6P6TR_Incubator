/*
 * Drv_Thermistor.h
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */

#ifndef DRIVERLAYER_DRV_THERMISTOR_H_
#define DRIVERLAYER_DRV_THERMISTOR_H_

#define NTC_TEMP_AVG_SAMPLE_TIMEOUT_MS			(5U)/*Ms*/
/*NTC - PARAMETER - START*/
#define SERIES_RESISTOR                         (10000.0f)      // R2 resistor value in ohms
#define THERMISTOR_NOMINAL_RESISTANCE           (10000.0f)      // resistance at 25°C
#define STANDARD_ROOM_TEMP_KELVIN               (298.15f)       // Kelvin (25°C)
#define BETA_VALUE                  			(3975.0f)       // 3950.0f Beta parameter for thermistor
#define MAX_VAL_ADC                 			(4095.0f)       // maximum value of 10 bit ADC
#define NTC_TEMP_MAX            				(185.0f)/*Temperature in farenheit for 85 degree celcius as per standard industrial range*/
#define NTC_TEMP_MIN            				(-40.0f)/*Temperature in farenheit for -40 degree celcius as per standard industrial range*/
/*NTC - PARAMETER - END*/

void Drv_Thermistor_Init(void);
void Drv_Thermistor_Exe(void);
float Drv_GetCurrentTemperature(void);
float Drv_ReadThermistor_TemperatureInCelcius(void);
#endif /* DRIVERLAYER_DRV_THERMISTOR_H_ */
