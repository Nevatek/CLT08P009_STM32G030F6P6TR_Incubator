/*
 * Drv_Peltier.h
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */

#ifndef DRIVERLAYER_DRV_PELTIER_H_
#define DRIVERLAYER_DRV_PELTIER_H_

#define PELTIER_DIR_SWITCH_HYSTERISIS_TIMEOUT_MS (20U)

typedef enum
{
	PELTIER_DIR_OFF = 0U,
	PELTIER_DIR_HEAT = 1U,
	PELTIER_DIR_COOL = 2U,
}PELTIER_DIR_STATE;

void Drv_Peltier_Init(void);
void Drv_SetPeltierPower(PELTIER_DIR_STATE m_NewDir , uint8_t u8SpeedPercent);
PELTIER_DIR_STATE Drv_GetPeltierDirection(void);
#endif /* DRIVERLAYER_DRV_PELTIER_H_ */
