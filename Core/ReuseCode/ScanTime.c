/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Datatype.h
.Summary : 
.Note :
 
Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
/*Aldrin.Rebellow 24/04/2024 Initial Release*/
#include "main.h"
#include "Datatype.h"
#include <string.h>
#include "Timer.h"
#include "ScanTime.h"

/* -------------------------- Global Functions ------------------------------ */
/* -------------------------------------------------------------------------- */
/* ------------------------ Global Variables -------------------------------- */
/* -------------------------------------------------------------------------- */

/**
 *  \brief This function used to Initialize scan time object
 *  \param [in] pPtr Scan time object pointer
 */
void ScanTime_Init(ScanTime_Tag *pPtr)
{
	(void)memset(pPtr, 0U, sizeof(*pPtr));
	pPtr->m_u32MinScanTIme = 0xFFFFFFFFU;
}

/**
 *  \brief This function used to record start scan
 *  \param [in] pPtr Scan time object pointer
 */
void ScanTime_RecordStartRecordingScan(ScanTime_Tag *pPtr, uint8_t u8Status)
{/* Update scan start */
	pPtr->m_u8StartRecording = u8Status;
	pPtr->m_u32ScanStartTime = HAL_GetTick();
	pPtr->m_u32ScanEndTime = 0U;
}

/**
 *  \brief This function used to calculate scan time of system
 *  \param [in] pPtr Scan time object pointer
 */
void ScanTime_CalCulateScanTime(ScanTime_Tag *pPtr)
{
	if(TRUE == pPtr->m_u8StartRecording)
	{
		uint32_t u32ScanTime = 0U;

		pPtr->m_u32ScanEndTime = HAL_GetTick();
	/* Get difference between Start of scan end end of scan */
		u32ScanTime = pPtr->m_u32ScanEndTime - pPtr->m_u32ScanStartTime;
		pPtr->m_u32ScanTime = u32ScanTime;/* Update current scan time */
		if(u32ScanTime < pPtr->m_u32MinScanTIme)
		{/* If current scan time is less than Last min scan time */
			pPtr->m_u32MinScanTIme = u32ScanTime;
		}

		if(u32ScanTime > pPtr->m_u32MaxScanTime)
		{/* If current scan time is less than Last max scan time */
			pPtr->m_u32MaxScanTime = u32ScanTime;
		}
		pPtr->m_u32ScanStartTime = pPtr->m_u32ScanEndTime;
	}
}

/**
 * \brief This function used to Get current scan time of system
 * \param [in] pPtr Scan time object pointer
 * \return Current Scan Time
 */
uint32_t ScanTime_GetCurrentScanTime(ScanTime_Tag *pPtr)
{
	return(pPtr->m_u32ScanTime);
}

/**
 *  \brief This function used to Get Maximum scan time of system
 *  \param [in] pPtr Scan time object pointer
 *  \return Maximum scan Time
 */
uint32_t ScanTime_GetMaxScanTime(ScanTime_Tag *pPtr)
{
	return(pPtr->m_u32MaxScanTime);
}

/**
 *  \brief This function used to Get minimum scan time of system
 *  \param [in] pPtr Scan time object pointer
 *  \return Minimum Scan Time
 */
uint32_t ScanTime_GetMinScanTime(ScanTime_Tag *pPtr)
{
	return(pPtr->m_u32MinScanTIme);
}
