/*
 * Drv_DS182B20.c
 *
 *  Created on: 19-Jul-2026
 *      Author: aldri
 */

#include "main.h"
#include "Datatype.h"
#include "HI_GptTimer.h"
#include "Drv_DS182B20.h"

#define DS18B20_PORT                GPIOA
#define DS18B20_PIN                 GPIO_PIN_3

#define DS18B20_CMD_SKIP_ROM        0xCCU
#define DS18B20_CMD_CONVERT_T       0x44U
#define DS18B20_CMD_READ_SCRATCH    0xBEU

#define DS18B20_CONVERSION_TIMEOUT_US    (750000UL)
#define DS18B20_POLL_INTERVAL_US         (1000U)

#define DS18B20_MAX_POLL_COUNT \
    ((uint16_t)(DS18B20_CONVERSION_TIMEOUT_US / DS18B20_POLL_INTERVAL_US))

static uint8_t g_u8SensorState = FALSE;

static void SetGpioPin_Output(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
static void SetGpioPin_Input(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
static void DS18B20_Write(uint8_t data);
static uint8_t DS18B20_Read(void);
static uint8_t DS18B20_CalculateCRC(const uint8_t *pu8Data, uint8_t u8Length);
/******************************************************************************
 * Function Name  : SetGpioPin_Output
 * ----------------------------------------------------------------------------
 * Purpose        : Configures the specified GPIO pin as a push-pull output.
 *
 * Description    :
 *      Configures the selected GPIO pin as a low-speed push-pull output for
 *      driving the 1-Wire bus LOW during reset, write, and read time slots.
 *
 * Parameters     :
 *      GPIOx      - Pointer to the GPIO peripheral.
 *      GPIO_Pin   - GPIO pin to configure.
 *
 * Return         :
 *      None.
 *
 * Note           :
 *      This function is intended for internal use by the DS18B20 driver.
 ******************************************************************************/
static void SetGpioPin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef gpioInit = {0};

    gpioInit.Pin   = GPIO_Pin;
    gpioInit.Mode  = GPIO_MODE_OUTPUT_PP;
    gpioInit.Pull  = GPIO_NOPULL;
    gpioInit.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOx, &gpioInit);
}


/******************************************************************************
 * Function Name  : SetGpioPin_Input
 * ----------------------------------------------------------------------------
 * Purpose        : Configures the specified GPIO pin as an input with pull-up.
 *
 * Description    :
 *      Releases the 1-Wire bus by configuring the GPIO as an input with an
 *      internal pull-up resistor, allowing the DS18B20 to drive the bus
 *      during presence detection and data transmission.
 *
 * Parameters     :
 *      GPIOx      - Pointer to the GPIO peripheral.
 *      GPIO_Pin   - GPIO pin to configure.
 *
 * Return         :
 *      None.
 *
 * Note           :
 *      This function is intended for internal use by the DS18B20 driver.
 *      An external pull-up resistor (typically 4.7 kΩ) is still required for
 *      reliable 1-Wire communication. The internal pull-up should not replace
 *      the external pull-up resistor specified by the DS18B20 datasheet.
 ******************************************************************************/
static void SetGpioPin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef gpioInit = {0};

    gpioInit.Pin  = GPIO_Pin;
    gpioInit.Mode = GPIO_MODE_INPUT;
    gpioInit.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOx, &gpioInit);
}
/******************************************************************************
 * Function Name  : DS18B20_Start
 * ----------------------------------------------------------------------------
 * Purpose        : Generates a 1-Wire reset pulse and detects the presence of
 *                  a DS18B20 sensor on the bus.
 *
 * Description    :
 *      Drives the 1-Wire bus LOW for the required reset period, releases the
 *      bus, and samples for the DS18B20 presence pulse as specified in the
 *      Maxim DS18B20 datasheet. The detected sensor status is stored
 *      internally and returned to the caller.
 *
 * Parameters     :
 *      None.
 *
 * Return         :
 *      uint8_t
 *          TRUE  - DS18B20 presence pulse detected.
 *          FALSE - No DS18B20 device detected on the 1-Wire bus.
 *
 * Note           :
 *      - This function is blocking and requires a microsecond delay function.
 *      - Presence detection timing complies with the DS18B20 datasheet.
 *      - This function must be called before issuing any ROM or Function
 *        commands to the DS18B20.
 *      - The sensor presence status can be retrieved using
 *        DS18B20_GetSensorState().
 ******************************************************************************/
uint8_t DS18B20_Start(void)
{
    uint8_t response = FALSE;
    TIM_HandleTypeDef * const pTimer = GetInstance_Timer17_OW();

    /* Master reset pulse */
    SetGpioPin_Output(DS18B20_PORT, DS18B20_PIN);
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);

    delayUs(pTimer, 600U);

    /* Release bus */
    SetGpioPin_Input(DS18B20_PORT, DS18B20_PIN);

    /* Wait before sampling presence pulse */
    delayUs(pTimer, 40U);

    if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN) == GPIO_PIN_RESET)
    {
        response = TRUE;
    }
    else
    {
        response = FALSE;
    }

    g_u8SensorState = response;

    /* Finish reset time slot */
    delayUs(pTimer, 420U);

    return response;
}
/******************************************************************************
 * Function Name  : DS18B20_Write
 * ----------------------------------------------------------------------------
 * Purpose        : Writes one byte to the DS18B20 using the 1-Wire protocol.
 *
 * Description    :
 *      Transmits 8 bits (LSB first) to the DS18B20. Each bit is transmitted
 *      using the timing specified in the Maxim DS18B20 datasheet.
 *
 * Parameters     :
 *      data    - Byte to transmit.
 *
 * Return         :
 *      None.
 *
 * Note           :
 *      This function is blocking and requires a microsecond delay function.
 ******************************************************************************/
static void DS18B20_Write(uint8_t data)
{
    uint8_t u8BitIndex;
    TIM_HandleTypeDef * const pTimer = GetInstance_Timer17_OW();

    for (u8BitIndex = 0U; u8BitIndex < 8U; u8BitIndex++)
    {
        SetGpioPin_Output(DS18B20_PORT, DS18B20_PIN);
        HAL_GPIO_WritePin(DS18B20_PORT,
                          DS18B20_PIN,
                          GPIO_PIN_RESET);

        if ((data & ((uint8_t)(1U << u8BitIndex))) != 0U)
        {
            /* Write Logic '1' */
            delayUs(pTimer, 6U);

            SetGpioPin_Input(DS18B20_PORT, DS18B20_PIN);

            delayUs(pTimer, 54U);
        }
        else
        {
            /* Write Logic '0' */
            delayUs(pTimer, 60U);

            SetGpioPin_Input(DS18B20_PORT, DS18B20_PIN);

            delayUs(pTimer, 5U);
        }
    }
}
/******************************************************************************
 * Function Name  : DS18B20_Read
 * ----------------------------------------------------------------------------
 * Purpose        : Reads one byte from the DS18B20 using the 1-Wire protocol.
 *
 * Description    :
 *      Reads eight bits (LSB first) from the DS18B20. Each bit is sampled
 *      according to the timing specified in the Maxim DS18B20 datasheet.
 *
 * Parameters     :
 *      None.
 *
 * Return         :
 *      uint8_t
 *          Byte read from the DS18B20.
 *
 * Note           :
 *      This function is blocking and requires a microsecond delay function.
 ******************************************************************************/
static uint8_t DS18B20_Read(void)
{
    uint8_t u8Value = 0U;
    uint8_t u8BitIndex;
    TIM_HandleTypeDef * const pTimer = GetInstance_Timer17_OW();

    for (u8BitIndex = 0U; u8BitIndex < 8U; u8BitIndex++)
    {
        /* Initiate read time slot */
        SetGpioPin_Output(DS18B20_PORT, DS18B20_PIN);

        HAL_GPIO_WritePin(DS18B20_PORT,
                          DS18B20_PIN,
                          GPIO_PIN_RESET);

        delayUs(pTimer, 2U);

        /* Release the bus */
        SetGpioPin_Input(DS18B20_PORT, DS18B20_PIN);

        /* Sample at approximately 15 us from slot start */
        delayUs(pTimer, 13U);

        if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN) == GPIO_PIN_SET)
        {
            u8Value |= (uint8_t)(1U << u8BitIndex);
        }
        else
        {
            /* Bit is logic '0' */
        }

        /* Complete the read slot */
        delayUs(pTimer, 45U);
    }

    return u8Value;
}
/******************************************************************************
 * Function Name  : DS18B20_GetSensorState
 * ----------------------------------------------------------------------------
 * Purpose        : Returns the current presence status of the DS18B20 sensor.
 *
 * Description    :
 *      Returns the sensor presence status determined during the last reset
 *      sequence (DS18B20_Start()).
 *
 * Parameters     :
 *      None.
 *
 * Return         :
 *      uint8_t
 *          TRUE  - DS18B20 detected on the 1-Wire bus.
 *          FALSE - DS18B20 not detected.
 *
 * Note           :
 *      The returned value reflects the most recent bus reset operation.
 ******************************************************************************/
uint8_t DS18B20_GetSensorState(void)
{
    return g_u8SensorState;
}
/******************************************************************************
 * Function Name  : DS18B20_ReadBit
 * ----------------------------------------------------------------------------
 * Purpose        : Reads a single bit from the 1-Wire bus.
 *
 * Description    :
 *      Generates a 1-Wire read time slot and samples the bus to read one bit
 *      from the DS18B20. This function is primarily used to poll the
 *      temperature conversion status after issuing the Convert T command.
 *
 * Parameters     :
 *      None.
 *
 * Return         :
 *      uint8_t
 *          TRUE  - Logic '1' detected.
 *          FALSE - Logic '0' detected.
 *
 * Note           :
 *      - This function is blocking.
 *      - Uses the hardware microsecond timer.
 *      - Timing complies with the Maxim DS18B20 datasheet.
 ******************************************************************************/
static uint8_t DS18B20_ReadBit(void)
{
    uint8_t u8Bit;
    TIM_HandleTypeDef * const pTimer = GetInstance_Timer17_OW();

    /* Initiate read time slot */
    SetGpioPin_Output(DS18B20_PORT, DS18B20_PIN);

    HAL_GPIO_WritePin(DS18B20_PORT,
                      DS18B20_PIN,
                      GPIO_PIN_RESET);

    delayUs(pTimer, 2U);

    /* Release bus */
    SetGpioPin_Input(DS18B20_PORT, DS18B20_PIN);

    /* Sample approximately 15 us after slot start */
    delayUs(pTimer, 13U);

    if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN) == GPIO_PIN_SET)
    {
        u8Bit = TRUE;
    }
    else
    {
        u8Bit = FALSE;
    }

    /* Complete the read slot */
    delayUs(pTimer, 45U);

    return u8Bit;
}
/******************************************************************************
 * Function Name  : DS18B20_WaitForConversionComplete
 * ----------------------------------------------------------------------------
 * Purpose        : Waits until the DS18B20 temperature conversion completes.
 *
 * Description    :
 *      Polls the 1-Wire bus until the DS18B20 indicates that the temperature
 *      conversion has completed by returning a logic '1'. The function exits
 *      immediately when the conversion is complete or returns FALSE if the
 *      conversion timeout expires.
 *
 * Parameters     :
 *      None.
 *
 * Return         :
 *      uint8_t
 *          TRUE  - Temperature conversion completed successfully.
 *          FALSE - Conversion timeout occurred.
 *
 * Note           :
 *      - This function is blocking.
 *      - Uses the hardware microsecond timer for deterministic timing.
 *      - Maximum conversion time is 750 ms at 12-bit resolution.
 ******************************************************************************/
static uint8_t DS18B20_WaitForConversionComplete(void)
{
    uint16_t u16PollCount;
    TIM_HandleTypeDef * const pTimer = GetInstance_Timer17_OW();

    for (u16PollCount = 0U;
         u16PollCount < DS18B20_MAX_POLL_COUNT;
         u16PollCount++)
    {
        if (DS18B20_ReadBit() == TRUE)
        {
            return TRUE;
        }

        delayUs(pTimer, DS18B20_POLL_INTERVAL_US);
    }

    return FALSE;
}
/******************************************************************************
 * Function Name  : DS18B20_CalculateCRC
 * ----------------------------------------------------------------------------
 * Purpose        : Calculates the Dallas/Maxim CRC-8 checksum.
 *
 * Description    :
 *      Calculates the CRC-8 value for the supplied data using the Dallas/
 *      Maxim polynomial. This function is used to verify the integrity of
 *      the DS18B20 scratchpad data.
 *
 * Parameters     :
 *      pu8Data    - Pointer to the data buffer.
 *      u8Length   - Number of bytes over which the CRC is calculated.
 *
 * Return         :
 *      uint8_t
 *          Calculated CRC-8 value.
 *
 * Note           :
 *      - Polynomial : x^8 + x^5 + x^4 + 1
 *      - Polynomial (LSB First): 0x8C
 *      - Used for DS18B20 scratchpad verification.
 ******************************************************************************/
static uint8_t DS18B20_CalculateCRC(const uint8_t *pu8Data,
                                    uint8_t u8Length)
{
    uint8_t u8ByteIndex;
    uint8_t u8BitIndex;
    uint8_t u8CRC = 0U;
    uint8_t u8Data;

    for (u8ByteIndex = 0U; u8ByteIndex < u8Length; u8ByteIndex++)
    {
        u8Data = pu8Data[u8ByteIndex];

        for (u8BitIndex = 0U; u8BitIndex < 8U; u8BitIndex++)
        {
            if (((u8CRC ^ u8Data) & 0x01U) != 0U)
            {
                u8CRC = (uint8_t)((u8CRC >> 1U) ^ 0x8CU);
            }
            else
            {
                u8CRC >>= 1U;
            }

            u8Data >>= 1U;
        }
    }

    return u8CRC;
}
/******************************************************************************
 * Function Name  : DS18B20_ReadTemperature
 * ----------------------------------------------------------------------------
 * Purpose        : Reads the current temperature from the DS18B20 sensor.
 *
 * Description    :
 *      Starts a temperature conversion, waits until the conversion is
 *      complete, reads the complete scratchpad, verifies the CRC, and
 *      converts the raw temperature value into degrees Celsius.
 *
 * Parameters     :
 *      None.
 *
 * Return         :
 *      float
 *          Measured temperature in degrees Celsius.
 *
 *          Returns DS18B20_INVALID_TEMPERATURE if:
 *          - No DS18B20 device is detected.
 *          - Temperature conversion times out.
 *          - Scratchpad CRC validation fails.
 *
 * Note           :
 *      - This function is blocking.
 *      - Supports signed temperature values.
 *      - CRC verification requires DS18B20_CalculateCRC().
 ******************************************************************************/
float DS18B20_ReadTemperature(void)
{
    uint8_t u8Index;
    uint8_t au8ScratchPad[9U];
    int16_t s16RawTemperature;

    if (DS18B20_Start() == FALSE)
    {
        return DS18B20_INVALID_TEMPERATURE;
    }

    /* Start temperature conversion */
    DS18B20_Write(DS18B20_CMD_SKIP_ROM);
    DS18B20_Write(DS18B20_CMD_CONVERT_T);

    /* Wait for conversion completion */
    if (DS18B20_WaitForConversionComplete() == FALSE)
    {
        return DS18B20_INVALID_TEMPERATURE;
    }

    /* Read Scratchpad */
    if (DS18B20_Start() == FALSE)
    {
        return DS18B20_INVALID_TEMPERATURE;
    }

    DS18B20_Write(DS18B20_CMD_SKIP_ROM);
    DS18B20_Write(DS18B20_CMD_READ_SCRATCH);

    for (u8Index = 0U; u8Index < 9U; u8Index++)
    {
        au8ScratchPad[u8Index] = DS18B20_Read();
    }

    /* Verify CRC */
    if (DS18B20_CalculateCRC(au8ScratchPad, 8U) != au8ScratchPad[8U])
    {
        return DS18B20_INVALID_TEMPERATURE;
    }

    /* Convert raw temperature to degrees Celsius */
    s16RawTemperature =
        (int16_t)(((uint16_t)au8ScratchPad[1U] << 8U) |
                  (uint16_t)au8ScratchPad[0U]);

    return ((float)s16RawTemperature / 16.0f);
}
