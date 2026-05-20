/******************************************************************************
 * @file    Drv_ET6226.c
 * @brief   driver implementation for Drv_ET6226 led controller
 *
 * @details Handles initialization, transmission of LED Display control .
 *
 * @author
 * @date    2026-04-28
 *
 * @note    MISRA C:2012 compliant
 ******************************************************************************/

/****************************************************************************/
/* Include files 															*/
/****************************************************************************/
#include "main.h"
#include "ErrorCode.h"
#include <stdbool.h>

#include "Drv_ET6226.h"

/****************************************************************************/
/* Macro definitions														*/
/****************************************************************************/

#include <stdint.h>

#define ET6226M_SEG_SG1   (1U << 0)  /* Segment A */
#define ET6226M_SEG_SG2   (1U << 1)  /* Segment B */
#define ET6226M_SEG_SG3   (1U << 2)  /* Segment C */
#define ET6226M_SEG_SG4   (1U << 3)  /* Segment D */
#define ET6226M_SEG_SG5   (1U << 4)  /* Segment E */
#define ET6226M_SEG_SG6   (1U << 5)  /* Segment F */
#define ET6226M_SEG_SG7   (1U << 6)  /* Segment G */
#define ET6226M_SEG_DP    (1U << 7)  /* Decimal Point */


/* Bit positions */
#define ET6226M_DISPLAY_POS      (0U)
#define ET6226M_SLEEP_POS        (1U)
#define ET6226M_MODE_POS         (2U)
#define ET6226M_BRIGHT_POS       (4U)

/* Masks */
#define ET6226M_DISPLAY_MASK     (0x01U << ET6226M_DISPLAY_POS)
#define ET6226M_SLEEP_MASK       (0x01U << ET6226M_SLEEP_POS)
#define ET6226M_MODE_MASK        (0x01U << ET6226M_MODE_POS)
#define ET6226M_BRIGHT_MASK      (0x07U << ET6226M_BRIGHT_POS) /* 3 bits */

// Command Bytes (High 8-bit commands)
#define ET6226M_CMD_CONTROL     0x48  // Status control command
#define ET6226M_CMD_DISPLAY_GR1 0x68  // Display data for GR1 (Grid 1)
#define ET6226M_CMD_DISPLAY_GR2 0x6A  // Display data for GR2 (Grid 2)
#define ET6226M_CMD_DISPLAY_GR3 0x6C  // Display data for GR3 (Grid 3)
//#define ET6226M_CMD_DISPLAY_GR4 0x6E  // Display data for GR4 (Grid 4)
#define ET6226M_CMD_KEY_READ    0x4F  // Key scan read command (not used)

#define I2C_DATA_SIZE 			(1U)	//2 bytes

#define ET6226M_TOTAL_DIGITS	(10U) 	// Total no of digits possible

#define ET6226M_FLAG_GR1    (1U << 0)
#define ET6226M_FLAG_GR2   	(1U << 1)
#define ET6226M_FLAG_GR3    (1U << 2)
//#define ET6226M_FLAG_GR4    (1U << 3)
#define ET6226M_FLAG_CTRL   (1U << 4)

#define ET6226M_MAX_DISPLAY (999U)
#define ET6226M_DIGIT_COUNT (3U)


/****************************************************************************/
/* Typedefs/Structure/Union definitions & declarations						*/
/****************************************************************************/
/* Field identifiers */
typedef enum
{
	ET_FIELD_DISPLAY = 0,
	ET_FIELD_SLEEP,
	ET_FIELD_MODE,
	ET_FIELD_BRIGHTNESS,
	ET_FIELD_MAX
} EtFieldId_t;

typedef enum
{
    ET6226M_PKT_DISPLAY_GR1 = 0U,
    ET6226M_PKT_DISPLAY_GR2,
    ET6226M_PKT_DISPLAY_GR3,
    ET6226M_PKT_DISPLAY_GR4,
    ET6226M_PKT_CONTROL,
    ET6226M_PKT_MAX
} ET6226M_PacketId_t;

typedef struct
{
	uint8_t pos;
	uint8_t mask;
} EtField_t;

typedef struct
{
    uint8_t cmd;
    uint8_t data;
} ET6226M_Packet_t;

/****************************************************************************/
/* Extern(Exported) Variable Declarations									*/
/****************************************************************************/

/****************************************************************************/
/* Global Variable Declarations												*/
/****************************************************************************/

/* Field table with designated initializers */
static ET6226M_Packet_t g_txPacket[ET6226M_PKT_MAX] =
{
    [ET6226M_PKT_DISPLAY_GR1] = { ET6226M_CMD_DISPLAY_GR1, 0U },
    [ET6226M_PKT_DISPLAY_GR2] = { ET6226M_CMD_DISPLAY_GR2, 0U },
    [ET6226M_PKT_DISPLAY_GR3] = { ET6226M_CMD_DISPLAY_GR3, 0U },
    [ET6226M_PKT_CONTROL]     = { ET6226M_CMD_CONTROL,     0U }
};

/* Field table with designated initializers */
static const EtField_t g_EtFields[ET_FIELD_MAX] =
{
		[ET_FIELD_DISPLAY]    = { ET6226M_DISPLAY_POS, ET6226M_DISPLAY_MASK },
		[ET_FIELD_SLEEP]      = { ET6226M_SLEEP_POS,   ET6226M_SLEEP_MASK   },
		[ET_FIELD_MODE]       = { ET6226M_MODE_POS,    ET6226M_MODE_MASK    },
		[ET_FIELD_BRIGHTNESS] = { ET6226M_BRIGHT_POS,  ET6226M_BRIGHT_MASK  }
};

// Seven segment encoding (bit mapping: DP-SG7-SG6-SG5-SG4-SG3-SG2-SG1)
static const uint8_t ET6226M_DigitTable[ET6226M_TOTAL_DIGITS] =
{
		/* 0 */ (ET6226M_SEG_SG1 | ET6226M_SEG_SG2 | ET6226M_SEG_SG3 |
				ET6226M_SEG_SG4 | ET6226M_SEG_SG5 | ET6226M_SEG_SG6),

		/* 1 */ (ET6226M_SEG_SG2 | ET6226M_SEG_SG3),

		/* 2 */ (ET6226M_SEG_SG1 | ET6226M_SEG_SG2 | ET6226M_SEG_SG4 |
				ET6226M_SEG_SG5 | ET6226M_SEG_SG7),

		/* 3 */ (ET6226M_SEG_SG1 | ET6226M_SEG_SG2 | ET6226M_SEG_SG3 |
								ET6226M_SEG_SG4 | ET6226M_SEG_SG7),

		/* 4 */ (ET6226M_SEG_SG2 | ET6226M_SEG_SG3 |
				ET6226M_SEG_SG6 | ET6226M_SEG_SG7),

		/* 5 */ (ET6226M_SEG_SG1 | ET6226M_SEG_SG3 | ET6226M_SEG_SG4 |
				ET6226M_SEG_SG6 | ET6226M_SEG_SG7),

		/* 6 */ (ET6226M_SEG_SG1 | ET6226M_SEG_SG3 | ET6226M_SEG_SG4 |
				ET6226M_SEG_SG5 | ET6226M_SEG_SG6 | ET6226M_SEG_SG7),

		/* 7 */ (ET6226M_SEG_SG1 | ET6226M_SEG_SG2 | ET6226M_SEG_SG3),

		/* 8 */ (ET6226M_SEG_SG1 | ET6226M_SEG_SG2 | ET6226M_SEG_SG3 |
				ET6226M_SEG_SG4 | ET6226M_SEG_SG5 | ET6226M_SEG_SG6 |
				ET6226M_SEG_SG7),

		/* 9 */ (ET6226M_SEG_SG1 | ET6226M_SEG_SG2 | ET6226M_SEG_SG3 |
				ET6226M_SEG_SG4 | ET6226M_SEG_SG6 | ET6226M_SEG_SG7)
};

static I2C_HandleTypeDef* g_i2cHandler = NULL;
static volatile ET6226M_I2C_State_t g_i2cState = ET6226M_I2C_IDLE;
static volatile uint8_t g_TransferFlags = 0;
static uint8_t currentTransferFlag = 0;
static uint8_t tempData = 0;

/****************************************************************************/
/* Function Declarations												*/
/****************************************************************************/

static void ET6226M_SetField(EtFieldId_t type, uint8_t value);
static void ET6226M_SetFlag(uint8_t flag);
static void ET6226M_ClearFlag(uint8_t flag);

static void i2c_write(ET6226M_PacketId_t PacketId);
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
/****************************************************************************/
/* Function Definitions														*/
/****************************************************************************/

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ET6226M_Init()
{
	g_i2cHandler = GetInstance_I2C1();

	if(g_i2cHandler == NULL)
	{
		g_i2cState = ET6226M_I2C_ERROR;
		return;
	}
	ET6226M_SetBrightness(ET6226M_BRIGHT_LEVEL_6);
	ET6226M_SetMode(ET6226M_7_SEGMENT);
	ET6226M_SetOperation(ET6226M_WAKE);
	ET6226M_SetDisplayState(ET6226M_ON);
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
static void ET6226M_SetField(EtFieldId_t type, uint8_t value)
{
	/* clear bits */
    g_txPacket[ET6226M_PKT_CONTROL].data &= (uint8_t)(~(g_EtFields[type].mask));

	/* set new value */
    g_txPacket[ET6226M_PKT_CONTROL].data |= (uint8_t)((value << (g_EtFields[type].pos)) &
													(g_EtFields[type].mask));
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
ErrorCode ET6226M_SetBrightness(ET6226M_BrightLevel_t level)
{
	ErrorCode status = ErrorCode_InvalidParameter;

	if (level <= ET6226M_BRIGHT_LEVEL_7)
	{
		ET6226M_SetField(ET_FIELD_BRIGHTNESS, (uint8_t)level);
        ET6226M_SetFlag(ET6226M_FLAG_CTRL);
		status = ErrorCode_Success;
	}

	return status;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
ErrorCode ET6226M_SetMode(ET6226M_Mode_t mode)
{
	ErrorCode status = ErrorCode_InvalidParameter;

	if (mode <= ET6226M_8_SEGMENT)
	{
		ET6226M_SetField(ET_FIELD_MODE, (uint8_t)mode);
        ET6226M_SetFlag(ET6226M_FLAG_CTRL);
		status = ErrorCode_Success;
	}

	return status;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
ErrorCode ET6226M_SetOperation(ET6226M_Operation_t operation)
{
	ErrorCode status = ErrorCode_InvalidParameter;

	if (operation <= ET6226M_WAKE)
	{
		ET6226M_SetField(ET_FIELD_SLEEP, (uint8_t)operation);
        ET6226M_SetFlag(ET6226M_FLAG_CTRL);

		status = ErrorCode_Success;
	}

	return status;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
ErrorCode ET6226M_SetDisplayState(ET6226M_Display_t state)
{
	ErrorCode status = ErrorCode_InvalidParameter;

	if (state <= ET6226M_ON)
	{
		ET6226M_SetField(ET_FIELD_DISPLAY, (uint8_t)state);
        ET6226M_SetFlag(ET6226M_FLAG_CTRL);

		status = ErrorCode_Success;
	}

	return status;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
static void ET6226M_SetFlag(uint8_t flag)
{
	g_TransferFlags |= flag;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
static void ET6226M_ClearFlag(uint8_t flag)
{
	g_TransferFlags &= (uint8_t)~flag;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
static inline bool ET6226M_IsFlagSet(uint8_t flag)
{
    return ((g_TransferFlags & flag) != 0U);
}
/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ET6226M_DisplayNumber(uint16_t value)
{
    uint16_t tempData = value;
    uint8_t digits[ET6226M_DIGIT_COUNT];

    /* limit to 999 */
    if (tempData > ET6226M_MAX_DISPLAY)
    {
    	tempData = ET6226M_MAX_DISPLAY;
    }

    digits[ET6226M_PKT_DISPLAY_GR3] = (uint8_t)(tempData % 10U);
    tempData /= 10U;

    digits[ET6226M_PKT_DISPLAY_GR2] = (uint8_t)(tempData % 10U);
    tempData /= 10U;

    digits[ET6226M_PKT_DISPLAY_GR1] = (uint8_t)(tempData % 10U);

    /* update display packets */
    tempData = ET6226M_DigitTable[digits[ET6226M_PKT_DISPLAY_GR1]];
    if(tempData != g_txPacket[ET6226M_PKT_DISPLAY_GR1].data)
    {
        g_txPacket[ET6226M_PKT_DISPLAY_GR1].data = tempData;
        ET6226M_SetFlag(ET6226M_FLAG_GR1);
    }

    /* update display packets */
    tempData = ET6226M_DigitTable[digits[ET6226M_PKT_DISPLAY_GR2]];
    if(tempData != g_txPacket[ET6226M_PKT_DISPLAY_GR2].data)
    {
        g_txPacket[ET6226M_PKT_DISPLAY_GR2].data = tempData | ET6226M_SEG_DP;/*Enable dp*/
        ET6226M_SetFlag(ET6226M_FLAG_GR2);
    }

    /* update display packets */
    tempData = ET6226M_DigitTable[digits[ET6226M_PKT_DISPLAY_GR3]];
    if(tempData != g_txPacket[ET6226M_PKT_DISPLAY_GR3].data)
    {
        g_txPacket[ET6226M_PKT_DISPLAY_GR3].data = tempData;
        ET6226M_SetFlag(ET6226M_FLAG_GR3);
    }
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ET6226M_RefreshDisplay(void)
{
    if (ET6226M_I2C_GetState() == ET6226M_I2C_IDLE)
    {
        /* GR1 */
        if (ET6226M_IsFlagSet(ET6226M_FLAG_GR1) == true)
        {
        	i2c_write(ET6226M_PKT_DISPLAY_GR1);
        	currentTransferFlag = ET6226M_FLAG_GR1;
        }
        /* GR2 */
        else if (ET6226M_IsFlagSet(ET6226M_FLAG_GR2) == true)
        {
        	i2c_write(ET6226M_PKT_DISPLAY_GR2);
        	currentTransferFlag = ET6226M_FLAG_GR2;
        }
        /* GR3 */
        else if (ET6226M_IsFlagSet(ET6226M_FLAG_GR3) == true)
        {
        	i2c_write(ET6226M_PKT_DISPLAY_GR3);
        	currentTransferFlag = ET6226M_FLAG_GR3;
        }
        /* GR4 */
        else if (ET6226M_IsFlagSet(ET6226M_FLAG_CTRL) == true)
        {
        	i2c_write(ET6226M_PKT_CONTROL);
        	currentTransferFlag = ET6226M_FLAG_CTRL;
        }
    }
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
ET6226M_I2C_State_t ET6226M_I2C_GetState(void)
{
    return g_i2cState;
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
******************************************************************************/
void ET6226M_TransferCompleteCallback(I2C_HandleTypeDef *hi2c)
{
	if(hi2c != NULL)
	{
		g_i2cState = ET6226M_I2C_IDLE;
        ET6226M_ClearFlag(currentTransferFlag);
	}
}

/******************************.FUNCTION_HEADER.******************************
.Purpose : This function serve as one time call function of application layer
.Returns :
.Note : use this function for all major initilization
 ******************************************************************************/
static void i2c_write(ET6226M_PacketId_t PacketId)
{
	if (PacketId >= ET6226M_PKT_MAX )
	{
        ET6226M_ClearFlag(currentTransferFlag);
        return;
	}

    if (ET6226M_I2C_GetState() == ET6226M_I2C_IDLE)
    {
		tempData = g_txPacket[PacketId].data;

		if(HAL_OK != HAL_I2C_Master_Transmit_IT(g_i2cHandler,
				g_txPacket[PacketId].cmd,
				&tempData,
				I2C_DATA_SIZE))
		{
			g_i2cState = ET6226M_I2C_ERROR;
		}
		else
		{
			g_i2cState = ET6226M_I2C_BUSY;
		}
	}
}


