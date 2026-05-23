/*
 * Drv_Peltier.h
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */

#ifndef DRIVERLAYER_DRV_PELTIER_H_
#define DRIVERLAYER_DRV_PELTIER_H_

#define PELTIER_DIR_SWITCH_HYSTERISIS_TIMEOUT_MS (100U)
#if (PELTIER_PID_CONTROL)
typedef enum
{
	PELTIER_DIR_OFF = 0U,
	PELTIER_DIR_HEAT = 1U,
	PELTIER_DIR_COOL = 2U,
}PELTIER_DIR_STATE;
#else
typedef enum
{
	PELTIER_DIR_COOL = 0U,
	PELTIER_DIR_HEAT = 1U,
}PELTIER_DIR_STATE;
typedef enum
{
	PELTIER_ST_OFF = 0U,
	PELTIER_ST_ON = 1U,
}PELTIER_STATE;
#endif
void Drv_Peltier_Init(void);
PELTIER_DIR_STATE Drv_GetPeltierDirection(void);
#if (PELTIER_PID_CONTROL)
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void Drv_SetPeltierPower(PELTIER_DIR_STATE m_NewDir , uint8_t u8SpeedPercent);
#else
void Drv_SetPeltierPower(PELTIER_STATE m_St);
void Drv_SetPeltierDirection(PELTIER_DIR_STATE m_NewDir);
void Drv_ExecutePeltierDirection(PELTIER_DIR_STATE m_Dir);
#endif
#endif /* DRIVERLAYER_DRV_PELTIER_H_ */
