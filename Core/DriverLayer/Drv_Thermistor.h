/*
 * Drv_Thermistor.h
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */

#ifndef DRIVERLAYER_DRV_THERMISTOR_H_
#define DRIVERLAYER_DRV_THERMISTOR_H_

#define NTC_TEMP_AVG_SAMPLE_TIMEOUT_MS			(10U)/*Ms*/
/*NTC - PARAMETER - START*/
#define SERIES_RESISTOR                         (10000.0f)      // R2 resistor value in ohms
#define NTC_SERIES_RESISTOR                     (300.0f)        // Rntc  + NTC_SERIES_RESISTOR as per schmeatics
#define THERMISTOR_NOMINAL_RESISTANCE           (10000.0f)      // resistance at 25°C
#define STANDARD_ROOM_TEMP_KELVIN               (298.15f)       // Kelvin (25°C)
#define BETA_VALUE                  			(3425.0f)       // 3950.0f Beta parameter for thermistor
#define MAX_VAL_ADC                 			(4095.0f)       // maximum value of 10 bit ADC
#define ONE_FLOAT                   			(1.0f)          // floating representaion of constant 1
#define SCALAR_FACTOR               			(9.0f / 5.0f)   // scale factor between Celsius and Fahrenheit.
#define OFFSET_VAL                  			(459.67f)       // offset used when converting Kelvin to Fahrenheit.
#define NTC_TEMP_MAX            				(185.0f)/*Temperature in farenheit for 85 degree celcius as per standard industrial range*/
#define NTC_TEMP_MIN            				(-40.0f)/*Temperature in farenheit for -40 degree celcius as per standard industrial range*/
/*NTC - PARAMETER - END*/

void Drv_Thermistor_Init(void);
void Drv_Thermistor_Exe(void);
float Drv_GetCurrentTemperature(void);
#endif /* DRIVERLAYER_DRV_THERMISTOR_H_ */
