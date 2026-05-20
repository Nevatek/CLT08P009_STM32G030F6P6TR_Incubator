/*
 * Drv_ExhaustFan.h
 *
 *  Created on: Apr 29, 2026
 *      Author: aldri
 */

#ifndef DRIVERLAYER_DRV_EXHAUSTFAN_H_
#define DRIVERLAYER_DRV_EXHAUSTFAN_H_

#define MIN_EXHAUST_FAN_SPEED_PERCENT (99U)

void Drv_InitilizeExhaustFan(void);
void Drv_SetSpeedExhaustFan(uint8_t u8SpeedPercent);

#endif /* DRIVERLAYER_DRV_EXHAUSTFAN_H_ */
