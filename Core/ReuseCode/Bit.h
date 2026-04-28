/* ORIGINAL LABEL START */
/****************************************************************************/
/* Conditional compilation definitions										*/
/****************************************************************************/
#ifndef BIT_H__
#define BIT_H__

/* <Other Conditional compilation definitions> */


/****************************************************************************/
/* Include files															*/
/****************************************************************************/

/****************************************************************************/
/* Other Conditional compilation definitions								*/
/****************************************************************************/

/****************************************************************************/
/* Macro definitions														*/
/****************************************************************************/
#define	UINT8_BITS			8U
#define	UINT16_BITS			16U
#define	UINT32_BITS			32U

/****************************************************************************/
/* Typedefs/Structure/Union definitions										*/
/****************************************************************************/

/****************************************************************************/
/* Enum definitions															*/
/****************************************************************************/

/****************************************************************************/
/* Extern(Exported) Variable Declarations									*/
/****************************************************************************/

/****************************************************************************/
/* Extern Function Declarations												*/
/****************************************************************************/

/****************************************************************************/
/* Global function declaration												*/
/****************************************************************************/

uint8_t Bituint8_t_Get(const uint8_t u8Value6, const uint8_t u8BitNo6);
uint8_t Bituint8_t_Set(const uint8_t u8Value, const uint8_t u8BitNo);
uint8_t Bituint8_t_Reset(const uint8_t u8Value1, const uint8_t u8BitNo1);

uint8_t Bituint16_t_Get(const uint16_t u16Value7, const uint8_t u8BitNo7);
uint16_t Bituint16_t_Set(const uint16_t u16Value2, const uint8_t u8BitNo2);
uint16_t Bituint16_t_Reset(const uint16_t u16Value3, const uint8_t u8BitNo3);

uint8_t Bituint32_t_Get(const uint32_t u32Value8, const uint8_t u8BitNo8);
uint32_t Bituint32_t_Set(const uint32_t u32Value4, const uint8_t u8BitNo4);
uint32_t Bituint32_t_Reset(const uint32_t u32Value5, const uint8_t u8BitNo5);

#endif /* BIT_H__ */

/* ORIGINAL LABEL END */
/****************************************************************************/
/*									End Of File								*/
/****************************************************************************/
