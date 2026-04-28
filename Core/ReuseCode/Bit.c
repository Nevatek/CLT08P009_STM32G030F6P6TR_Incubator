/* ORIGINAL LABEL START */
/****************************************************************************/
/* Include files 															*/
/****************************************************************************/
#include "main.h"
#include <string.h>
#include "Datatype.h"
#include "Bit.h"

/* <Other Conditional compilation definitions> */

/****************************************************************************/
/* Macro definitions														*/
/****************************************************************************/

/****************************************************************************/
/* Enum definitions & declarations											*/
/****************************************************************************/

/****************************************************************************/
/* Typedefs/Structure/Union definitions & declarations						*/
/****************************************************************************/

/****************************************************************************/
/* Extern(Exported) Variable Declarations									*/
/****************************************************************************/

/****************************************************************************/
/* Global Variable Declarations												*/
/****************************************************************************/

/****************************************************************************/
/* Static Variable Declarations												*/
/****************************************************************************/

/****************************************************************************/
/* Extern Function Declarations												*/
/****************************************************************************/

/****************************************************************************/
/* Internal Function Declarations											*/
/****************************************************************************/

/****************************************************************************/
/* Function Definitions														*/
/****************************************************************************/

/**
 *\brief	This function will return reseptive bit.
 *
 *\return	Reseptive bit.
 */
uint8_t BitUINT8_Get(const uint8_t u8Value6,			/**< Value in-which bit is to be set */
					const uint8_t u8BitNo6			/**< Bit no is to be set */
					) {

	uint8_t u8ReturnValue6;	/* Variable for bit manupulation. */
	uint8_t u8Val6;			/* Variable for bit manupulation. */
	uint8_t u8BitValue6;		/* Variable for bit manupulation. */

	u8ReturnValue6 = 0U;
	/* Check if bit number is valid or not. */
	if(UINT8_BITS > u8BitNo6) {
		/* Bit number is valid. */
		/* Set receptive bit. */
		u8BitValue6 = (uint8_t)0x01U << (uint8_t)u8BitNo6;
		/* Set receptive bit in Final value. */
		u8Val6 = u8Value6 & u8BitValue6;
		if(0U != u8Val6) {
			/* Respetive Bit is set */
			u8ReturnValue6 = 0x01U;
		}
	}
	return(u8ReturnValue6);
}

/**
 *\brief	This function will set bit in received value.
 *
 *\return	Value with reseptive bit set.
 */
uint8_t BitUINT8_Set(const uint8_t u8Value,			/**< Value in-which bit is to be set */
					const uint8_t u8BitNo			/**< Bit no is to be set */
					) {

	uint8_t u8ReturnValue;
	uint8_t u8BitValue;

	u8ReturnValue = u8Value;
	/* Check if bit number is valid or not. */
	if(UINT8_BITS > u8BitNo) {
		/* Bit number is valid. */
		/* Set receptive bit. */
		u8BitValue = (uint8_t)0x01U << (uint8_t)u8BitNo;
		/* Set receptive bit in Final value. */
		u8ReturnValue |= u8BitValue;
	}
	return(u8ReturnValue);
}

/**
 *\brief	This function will reset bit in received value.
 *
 *\return	Value with reseptive bit reset.
 */
uint8_t BitUINT8_Reset(const uint8_t u8Value1,			/**< Value in-which bit is to be reset */
						const uint8_t u8BitNo1		/**< Bit no is to be reset */
						) {

	uint8_t u8ReturnValue1;
	uint8_t u8BitValue1;

	u8ReturnValue1 = u8Value1;
	/* Check if bit number is valid or not. */
	if(UINT8_BITS > u8BitNo1) {
		/* Bit number is valid. */
		/* Set receptive bit. */
		u8BitValue1 = (uint8_t)0x01U << (uint8_t)u8BitNo1;
		/* Set receptive bit in Final value. */
		u8ReturnValue1 &= ~u8BitValue1;
	}
	return(u8ReturnValue1);
}

/**
 *\brief	This function will return reseptive bit.
 *
 *\return	Reseptive bit.
 */
uint8_t BitUINT16_Get(const uint16_t u16Value7,			/**< Value in-which bit is to be set */
					const uint8_t u8BitNo7			/**< Bit no is to be set */
					) {

	uint8_t u8ReturnValue7;	/* Variable for bit manupulation. */
	uint16_t u16Val7;			/* Variable for bit manupulation. */
	uint16_t u16BitValue7;		/* Variable for bit manupulation. */

	u8ReturnValue7 = 0U;
	/* Check if bit number is valid or not. */
	if(UINT16_BITS > u8BitNo7) {
		/* Bit number is valid. */
		/* Set receptive bit. */
		u16BitValue7 = (uint16_t)0x01U << (uint16_t)u8BitNo7;
		/* Set receptive bit in Final value. */
		u16Val7 = u16Value7 & u16BitValue7;
		if(0U != u16Val7) {
			/* Respetive Bit is set */
			u8ReturnValue7 = 0x01U;
		}
	}
	return(u8ReturnValue7);
}

/**
 *\brief	This function will set bit in received value.
 *
 *\return	Value with reseptive bit set.
 */
uint16_t BitUINT16_Set(const uint16_t u16Value2,		/**< Value in-which bit is to be set */
					const uint8_t u8BitNo2			/**< Bit no is to be set */
					) {

	uint16_t u16ReturnValue2;
	uint16_t u16BitValue2;

	u16ReturnValue2 = u16Value2;
	/* Check if bit number is valid or not. */
	if(UINT16_BITS > u8BitNo2) {
		/* Bit number is valid. */
		/* Set receptive bit. */
		u16BitValue2 = (uint16_t)0x01U << (uint16_t)u8BitNo2;
		/* Set receptive bit in Final value. */
		u16ReturnValue2 |= u16BitValue2;
	}
	return(u16ReturnValue2);
}

/**
 *\brief	This function will reset bit in received value.
 *
 *\return	Value with reseptive bit reset.
 */
uint16_t BitUINT16_Reset(const uint16_t u16Value3,			/**< Value in-which bit is to be reset */
						const uint8_t u8BitNo3			/**< Bit no is to be reset */
						) {

	uint16_t u16ReturnValue3;
	uint16_t u16BitValue3;

	u16ReturnValue3 = u16Value3;
	/* Check if bit number is valid or not. */
	if(UINT16_BITS > u8BitNo3) {
		/* Bit number is valid. */
		/* Set receptive bit. */
		u16BitValue3 = (uint16_t)0x01U << (uint16_t)u8BitNo3;
		/* Set receptive bit in Final value. */
		u16ReturnValue3 &= ~u16BitValue3;
	}
	return(u16ReturnValue3);
}

/**
 *\brief	This function will return reseptive bit.
 *
 *\return	Reseptive bit.
 */
uint8_t BitUINT32_Get(const uint32_t u32Value8,			/**< Value in-which bit is to be set */
					const uint8_t u8BitNo8			/**< Bit no is to be set */
					) {

	uint8_t u8ReturnValue8;	/* Variable for bit manupulation. */
	uint32_t u32Val8;			/* Variable for bit manupulation. */
	uint32_t u32BitValue8;	/* Variable for bit manupulation. */

	u8ReturnValue8 = 0U;
	/* Check if bit number is valid or not. */
	if(UINT32_BITS > u8BitNo8) {
		/* Bit number is valid. */
		/* Set receptive bit. */
		u32BitValue8 = (uint32_t)0x01U << (uint32_t)u8BitNo8;
		/* Set receptive bit in Final value. */
		u32Val8 = u32Value8 & u32BitValue8;
		if((uint32_t)(0U) != u32Val8) {
			/* Respetive Bit is set */
			u8ReturnValue8 = 0x01U;
		}
	}
	return(u8ReturnValue8);
}

/**
 *\brief	This function will set bit in received value.
 *
 *\return	Value with reseptive bit set.
 */
uint32_t BitUINT32_Set(const uint32_t u32Value4,		/**< Value in-which bit is to be set */
					const uint8_t u8BitNo4			/**< Bit no is to be set */
					) {

	uint32_t u32ReturnValue4;
	uint32_t u32BitValue4;

	u32ReturnValue4 = u32Value4;
	/* Check if bit number is valid or not. */
	if(UINT32_BITS > u8BitNo4) {
		/* Bit number is valid. */
		/* Set receptive bit. */
		u32BitValue4 = (uint32_t)0x01U << (uint32_t)u8BitNo4;
		/* Set receptive bit in Final value. */
		u32ReturnValue4 |= u32BitValue4;
	}
	return(u32ReturnValue4);
}

/**
 *\brief	This function will reset bit in received value.
 *
 *\return	Value with reseptive bit reset.
 */
uint32_t BitUINT32_Reset(const uint32_t u32Value5,			/**< Value in-which bit is to be reset */
						const uint8_t u8BitNo5			/**< Bit no is to be reset */
						) {

	uint32_t u32ReturnValue5;
	uint32_t u32BitValue5;

	u32ReturnValue5 = u32Value5;
	/* Check if bit number is valid or not. */
	if(UINT32_BITS > u8BitNo5) {
		/* Bit number is valid. */
		/* Set receptive bit. */
		u32BitValue5 = (uint32_t)0x01U << (uint32_t)u8BitNo5;
		/* Set receptive bit in Final value. */
		u32ReturnValue5 &= ~u32BitValue5;
	}
	return(u32ReturnValue5);
}

/* ORIGINAL LABEL END */
/****************************************************************************/
/*									End Of File 							*/
/****************************************************************************/
