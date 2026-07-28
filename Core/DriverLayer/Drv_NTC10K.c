/*
 * Drv_NTC10K.c
 *
 *  Created on: 28-Jul-2026
 *      Author: Aldrin
 *
 *  NTC Configuration:
 *      R1 = 10kΩ (Connected to 3.3V)
 *      R2 = 10kΩ NTC (Connected to GND)
 *      ADC connected to divider midpoint
 *
 *      Vout = VCC * (Rntc / (Rfixed + Rntc))
 */
#include "main.h"
#include <math.h>
#include <stdint.h>
#include "Drv_NTC10K.h"
#include "HI_Adc.h"

/*==============================================================================
 * Configuration
 *============================================================================*/

#define NTC_FIXED_RESISTOR_OHM       (10000.0f)     /* R1 */
#define NTC_NOMINAL_RESISTANCE_OHM   (10000.0f)     /* Resistance @25°C */
#define NTC_NOMINAL_TEMP_C           (25.0f)
#define NTC_BETA_COEFFICIENT         (3435.0f)

#define ADC_MAX_COUNT                (4095.0f)      /* 12-bit ADC */
#define KELVIN_OFFSET                (273.15f)

#define INV_T0                       (1.0f / (NTC_NOMINAL_TEMP_C + KELVIN_OFFSET))
#define INV_BETA                     (1.0f / NTC_BETA_COEFFICIENT)

/* ADC limits to detect open/short */
#define ADC_MIN_VALID                (1U)
#define ADC_MAX_VALID                (4094U)

/* Optional temperature clamp */
#define MIN_TEMPERATURE_C            (-40.0f)
#define MAX_TEMPERATURE_C            (125.0f)

uint16_t g_Adc0Value;
float g_NtcResistance;
/*==============================================================================
 * Public Functions
 *============================================================================*/

/**
 * @brief  Reads the NTC temperature.
 *
 * @return Temperature in degree Celsius.
 */
float Drv_Ntc10K_GetTemperatureDegreeCelcius(void)
{
    float adc;

    float temperatureKelvin;
    float temperatureCelsius;

    /* Read ADC */
    g_Adc0Value = HI_GetAdcValue_PollingBlocking();

    /* Prevent divide-by-zero and infinite resistance */
    if (g_Adc0Value <= ADC_MIN_VALID)
    {
    		g_Adc0Value = ADC_MIN_VALID;
    }
    else if (g_Adc0Value >= ADC_MAX_VALID)
    {
    		g_Adc0Value = ADC_MAX_VALID;
    }

    adc = (float)g_Adc0Value;

    /*
     * Divider equation:
     *
     * Vadc = Vcc * Rntc / (Rfixed + Rntc)
     *
     * Therefore:
     *
     * Rntc = Rfixed * ADC / (ADCmax - ADC)
     */
    g_NtcResistance =
        (NTC_FIXED_RESISTOR_OHM * adc) /
        (ADC_MAX_COUNT - adc);

    /* Beta equation */
    temperatureKelvin =
        1.0f /
        (
            INV_T0 +
            (INV_BETA * logf(g_NtcResistance / NTC_NOMINAL_RESISTANCE_OHM))
        );

    /* Kelvin to Celsius */
    temperatureCelsius = temperatureKelvin - KELVIN_OFFSET;

    /* Clamp to practical range */
    if (temperatureCelsius < MIN_TEMPERATURE_C)
    {
        temperatureCelsius = MIN_TEMPERATURE_C;
    }
    else if (temperatureCelsius > MAX_TEMPERATURE_C)
    {
        temperatureCelsius = MAX_TEMPERATURE_C;
    }

    return temperatureCelsius;
}
