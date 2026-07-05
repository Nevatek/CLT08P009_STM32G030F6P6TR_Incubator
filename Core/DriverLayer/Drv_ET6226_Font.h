/*
 * Drv_ET6226_Font.h
 *
 *  Created on: 05-Jul-2026
 *      Author: User
 */

#ifndef DRIVERLAYER_DRV_ET6226_FONT_H_
#define DRIVERLAYER_DRV_ET6226_FONT_H_

#define ET6226M_TOTAL_DIGITS	(12U) 	// Total no of digits possible
#define ET6226M_TOTAL_CHARS		(128U) 	// Total no of digits possible


#define ET6226M_SEG_SG1   (1U << 0)  /* Segment A */
#define ET6226M_SEG_SG2   (1U << 1)  /* Segment B */
#define ET6226M_SEG_SG3   (1U << 2)  /* Segment C */
#define ET6226M_SEG_SG4   (1U << 3)  /* Segment D */
#define ET6226M_SEG_SG5   (1U << 4)  /* Segment E */
#define ET6226M_SEG_SG6   (1U << 5)  /* Segment F */
#define ET6226M_SEG_SG7   (1U << 6)  /* Segment G */
#define ET6226M_SEG_DP    (1U << 7)  /* Decimal Point */

extern const uint8_t ET6226M_DigitTable[ET6226M_TOTAL_DIGITS];
extern const uint8_t ET6226M_CharTable[ET6226M_TOTAL_CHARS];

#endif /* DRIVERLAYER_DRV_ET6226_FONT_H_ */
