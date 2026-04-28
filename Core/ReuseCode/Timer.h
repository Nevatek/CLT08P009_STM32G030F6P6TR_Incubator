/*********************************.FILE_HEADER.*******************************
<Copyright Notice>
.File : Datatype.h
.Summary : 
.Note :
 
Author Date Description
------------------------------------------------------------------------------
<Author name> <DDMMMYYYY> <changes made>*/
/*Aldrin.Rebellow 24/04/2024 Initial Release*/
#ifndef __TIMER_H__
#define __TIMER_H__


typedef uint32_t TimeOutType;			/**< Time-out type is defined */

typedef struct
{
	TimeOutType m_u32StartTime;		/**< Start Time */
	TimeOutType m_u32Timeout;		/**< Time out value */
	uint8_t  m_u8Over;				/**< Time out over flag */
	uint8_t  m_u8TimerRunning;		/**< Flag for timer running */
}TimerTimeOut;

void TimeOut_Init(TimerTimeOut *pTimer);
void TimeOut_Start(TimerTimeOut *pTimeOut, TimeOutType TimeOutValue);
void TimeOut_Restart(TimerTimeOut *pTimeOut);
void TimeOut_Stop(TimerTimeOut *pTimeOut);
uint32_t TimeOut_getTime_ms(TimerTimeOut *pTimeOut);
uint8_t TimeOut_IsTimerRunning(TimerTimeOut *pTimeOut);
uint8_t TimeOut_IsTimeout(TimerTimeOut *pTimeOut);
uint8_t TimeOut_IsTimeoutRestart(TimerTimeOut *pTimeOut);
void TimeOut_Exit(TimerTimeOut *pTimer);
void TimeOut_DelayBlocking(TimerTimeOut *pTimeOut, TimeOutType TimeOutValue);
#endif /* __TIMER_H__ */
