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
#include "string.h"
#include "Timer.h"

/**
 *\brief This function will be used to create a blockimng delay of time given as parameter.
 *\param[in] pTimeOut Pointer to timeout structure.
 *\param[in] TimerTimeOut Timeout type.
 */
void TimeOut_DelayBlocking(TimerTimeOut *pTimeOut, TimeOutType TimeOutValue)
{
	TimeOut_Init(pTimeOut);
	TimeOut_Start(pTimeOut , TimeOutValue);
	for(;;)
	{
		if(TRUE == TimeOut_IsTimeout(pTimeOut))
		{
			break;
		}
	}
}
/**
 *\brief This function will be used to initialize the time out.
 *\param[in] pTimer Pointer to timeout structure.
 */
void TimeOut_Init(TimerTimeOut *pTimer)
{
	(void)memset(pTimer, 0U, sizeof(*pTimer));
}

/**
 *\brief This function will be used to start the timeout.
 *\param[in] pTimeOut Pointer to timeout structure.
 *\param[in] TimerTimeOut Timeout type.
 */
void TimeOut_Start(TimerTimeOut *pTimeOut, TimeOutType TimeOutValue)
{
	TimeOutType CurrentTime = HAL_GetTick();

	(void)memset(pTimeOut, 0, sizeof(*pTimeOut));
	pTimeOut->m_u32StartTime = CurrentTime;
	pTimeOut->m_u32Timeout = TimeOutValue;
	
	/* Indicate timer is running */
	pTimeOut->m_u8TimerRunning = TRUE;
}

/**
 *\brief This function will restart timer. Old Timeout value will be timeout
 *\param[in] pTimeOut Pointer to timeout structure.
 */
void TimeOut_Restart(TimerTimeOut *pTimeOut)
{
	/* Read timeout value. */ 
	TimeOutType TimeOutValue = pTimeOut->m_u32Timeout;
	/* Clear up timer. */
	TimeOut_Init(pTimeOut);
	/* Start Timer. */
	TimeOut_Start(pTimeOut, TimeOutValue);
}

/**
 *\brief This function will stop running timer.
 *\param[in] pTimeOut Pointer to timeout structure.
 */
void TimeOut_Stop(TimerTimeOut *pTimeOut)
{
	/* Clear up timer. */
	TimeOut_Init(pTimeOut);
	/* Indicate timer is not running (i.e. stopped.) */
	pTimeOut->m_u8TimerRunning = FALSE;
}

/**
 *\brief This function will return timer is running or not?
 *\param[in] pTimeOut Pointer to timeout structure.
 *\retval TRUE Timer is running.
 *\retval FALSE Timer is not running.
 */
uint8_t TimeOut_IsTimerRunning(TimerTimeOut *pTimeOut)
{
	return(pTimeOut->m_u8TimerRunning);
}

/**
 *\brief This function will be used to identify whether the timeout is over.
 *\param[in] pTimeOut Pointer to timeout structure.
 *\retval TRUE Timeout over.
 *\retval FALSE Timeout not over.
 */
uint8_t TimeOut_IsTimeout(TimerTimeOut *pTimeOut)
{
	TimeOutType TimeValue;
	if (pTimeOut->m_u8Over == 0U)
	{
		TimeValue = HAL_GetTick() - pTimeOut->m_u32StartTime;
		if (TimeValue >= pTimeOut->m_u32Timeout)
		{
			pTimeOut->m_u8Over = 1U;
			/* Indicate timer is over i.e. not running (i.e. stopped.) */
			pTimeOut->m_u8TimerRunning = FALSE;
		}
	}
	return(pTimeOut->m_u8Over);
}
/**
 *\brief This function will be used to identify whether the timeout is over.
 *\param[in] pTimeOut Pointer to timeout structure.
 *\retval TRUE Timeout over.
 *\retval FALSE Timeout not over.
 */
uint32_t TimeOut_getTime_ms(TimerTimeOut *pTimeOut)
{
	TimeOutType TimeValue = 0;
	if (pTimeOut->m_u8Over == 0U)
	{
		TimeValue = HAL_GetTick() - pTimeOut->m_u32StartTime;
	}
	return(TimeValue);
}
/**
 *\brief This function will be used to identify whether the timeout is over. This funciton also restart the timer.
 *\retval TRUE Timeout over.
 *\retval FALSE Timeout not over.
 */
uint8_t TimeOut_IsTimeoutRestart(TimerTimeOut *pTimeOut	/**< param[in] pTimeOut Pointer to timeout structure. */
								) {

	uint8_t u8RetValue;
	
	/* Get timeout over status. */
	u8RetValue = TimeOut_IsTimeout(pTimeOut);
	
	/* Check if timeour is over or not. */
	if (TRUE == u8RetValue)	{
		/* Timeout is over so restart the timer. */
		TimeOut_Restart(pTimeOut);
	}
	
	/* Return Status for timeout. */
	return(u8RetValue);
}

/**
 *\brief This function will be used to cleanup the time out.
 *\param[in] pTimer Pointer to timeout structure.
 */
void TimeOut_Exit(TimerTimeOut *pTimer)
{
	(void)memset(pTimer, 0U, sizeof(*pTimer));
}

