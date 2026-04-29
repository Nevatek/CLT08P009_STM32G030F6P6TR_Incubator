/******************************************************************************
 * @file    Drv_ET6226.h
 * @brief   driver implementation for Drv_ET6226 led controller
 *
 * @details Handles initialization, transmission of LED Display control .
 *
 * @author
 * @date    2026-04-28
 *
 * @note    MISRA C:2012 compliant
 ******************************************************************************/

#ifndef HARDWAREINTERFACE_DRV_ET6226_H_
#define HARDWAREINTERFACE_DRV_ET6226_H_

#include "ErrorCode.h"

typedef enum
{
	ET6226M_BRIGHT_LEVEL_0 = 0U,  	/* Default Highest brightness */
	ET6226M_BRIGHT_LEVEL_1 = 1U,	/* minimum brightness */
	ET6226M_BRIGHT_LEVEL_2 = 2U,
	ET6226M_BRIGHT_LEVEL_3 = 3U,
	ET6226M_BRIGHT_LEVEL_4 = 4U,
	ET6226M_BRIGHT_LEVEL_5 = 5U,
	ET6226M_BRIGHT_LEVEL_6 = 6U,
	ET6226M_BRIGHT_LEVEL_7 = 7U   	/* maximum brightness */
} ET6226M_BrightLevel_t;

typedef enum
{
	ET6226M_7_SEGMENT = 0U,
	ET6226M_8_SEGMENT = 1U
} ET6226M_Mode_t;

typedef enum
{
	ET6226M_SLEEP = 0U,
	ET6226M_WAKE  = 1U
} ET6226M_Operation_t;

typedef enum
{
	ET6226M_OFF = 0U,
	ET6226M_ON = 1U
} ET6226M_Display_t;

typedef enum
{
    ET6226M_I2C_IDLE = 0U,
    ET6226M_I2C_BUSY = 1U,
	ET6226M_I2C_ERROR
} ET6226M_I2C_State_t;


void ET6226M_Init(void);
void ET6226M_RefreshDisplay(void);

ErrorCode ET6226M_SetBrightness(ET6226M_BrightLevel_t level);
ErrorCode ET6226M_SetMode(ET6226M_Mode_t mode);
ErrorCode ET6226M_SetOperation(ET6226M_Operation_t operation);
ErrorCode ET6226M_SetDisplayState(ET6226M_Display_t state);

void ET6226M_DisplayNumber(uint16_t value);

void ET6226M_TransferCompleteCallback(I2C_HandleTypeDef *hi2c);

ET6226M_I2C_State_t ET6226M_I2C_GetState(void);


#endif /* HARDWAREINTERFACE_DRV_ET6226_H_ */
