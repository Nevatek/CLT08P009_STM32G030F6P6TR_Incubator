/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Datatype.h
.Summary : 
.Note :
 
Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
#ifndef __SCANTIME_H__
#define __SCANTIME_H__

typedef struct
{
	uint32_t	m_u32ScanStartTime;			/**< Start time to calculate Scan time */
	uint32_t	m_u32ScanEndTime;			/**< End time to calculate Scan time */
	uint32_t 	m_u32ScanTime;
	uint32_t	m_u32MinScanTIme;			/**< Minimum scan time */
	uint32_t 	m_u32MaxScanTime;			/**< Maximum scan time */
	uint8_t 	m_u8StartRecording;
}ScanTime_Tag;

void ScanTime_Init(ScanTime_Tag *pPtr);
void ScanTime_RecordStartRecordingScan(ScanTime_Tag *pPtr, uint8_t u8Status);
void ScanTime_CalCulateScanTime(ScanTime_Tag *pPtr);

#endif /* __SCANTIME_H__ */
