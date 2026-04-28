
/* ORIGINAL LABEL START */
/****************************************************************************/
/* Include files 															*/
/****************************************************************************/
#include "main.h"
#include <string.h>
#include "ErrorCode.h"
#include "StateMachine.h"

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
 *\brief This function is used to initialize state machine instance. 
 */
ErrorCode StateMachine_Init(StateMachine *pThis)
{
	ErrorCode u8Ret = ErrorCode_InvalidParameter;
	
	if(pThis != NULL)
	{
		memset(pThis, 0U, sizeof(*pThis));
		u8Ret = ErrorCode_Success;
	}
	else
	{
		;/* Do Nothing */
	}
	return u8Ret;
}

/**
 *\brief This function is used to configure state machine instance. for state machine data,
 *\brief its size and default state.
 */
ErrorCode StateMachine_Config(StateMachine *pThis, StateMachineData *pData, uint8_t u8Size, uint8_t u8DefaultState, void *pThisModule)
{
	ErrorCode u8Ret = ErrorCode_InvalidParameter;
	
	if((pThis != NULL) && (pData != NULL))
	{
		pThis->m_pData = pData;
		pThis->m_u8CurrentState = u8DefaultState;
		pThis->m_u8NextState = u8DefaultState;
		pThis->m_u8SubState = SUBSTATE_ENTRY;
		pThis->m_u8Size = u8Size;
		pThis->m_pThisModule = pThisModule;
		
		u8Ret = ErrorCode_Success;
	}
	else
	{
		;/* Do Nothing */
	}
	return u8Ret;
}

/**
 *\brief This function is used to Run state machine. functions assigned using
 *\brief state machine data structure are being called as per state machine sub-state.
 */
ErrorCode StateMachine_Run(StateMachine *pThis, void *pParam)
{
	StateMachineData *pData;
	ErrorCode u8Ret = ErrorCode_InvalidParameter;
	
	if(NULL != pThis)
	{
		uint8_t u8Continue;
		do{
			u8Continue = 0U;
			if(NULL != pThis->m_pData)
			{
				pData = &(pThis->m_pData[pThis->m_u8CurrentState]);
			}
			else
			{
				pData = NULL;
			}

			
			if(SUBSTATE_ENTRY == pThis->m_u8SubState)
			{
				if(NULL != pData)
				{
					if(NULL != pData->m_StateEntry)
					{
						pData->m_StateEntry(pThis, pThis->m_u8CurrentState, pThis->m_pThisModule, pParam); /* Call to Entry funtion of that State */
					}
				}
				pThis->m_u8SubState = SUBSTATE_RUN;
			}
				
			if(SUBSTATE_RUN == pThis->m_u8SubState)
			{
				if(NULL != pData)
				{
					if(NULL != pData->m_StateRun)
					{
						pData->m_StateRun(pThis, pThis->m_u8CurrentState, pThis->m_pThisModule, pParam);	/* Call to Run funtion of that State */
					}
				}
			}
			
			if(pThis->m_u8NextState != pThis->m_u8CurrentState)
			{
				if(NULL != pThis->m_pData)
				{
					if(NULL != pData->m_StateExit)
					{
						pData->m_StateExit(pThis, pThis->m_u8CurrentState, pThis->m_u8NextState, pThis->m_pThisModule, pParam);	/* Call to Exit funtion of that State */
					}
					if(1U == pData->m_u8Flag)
					{
						u8Continue = 1U;
					}
				}
				pThis->m_u8SubState = SUBSTATE_ENTRY;
				pThis->m_u8PreviousState = pThis->m_u8CurrentState;
				pThis->m_u8CurrentState = pThis->m_u8NextState;
			}
			
		}while(1U == u8Continue);
		
		u8Ret = ErrorCode_Success;
	}
	else
	{
		;/* Do Nothing */
	}
	return u8Ret;
}

/**
 *\brief This function is used to switch state machine state to next state. 
 */
ErrorCode StateMachine_SetNextState(StateMachine *pThis, uint8_t u8NextState)
{
	ErrorCode u8Ret = ErrorCode_InvalidParameter;
	
	if(NULL != pThis)
	{
		if(u8NextState < pThis->m_u8Size)
		{
			pThis->m_u8NextState = u8NextState;
		}
		u8Ret = ErrorCode_Success;
	}
	else
	{
		;/* Do Nothing */
	}
	return u8Ret;
}

/**
 *\brief This function is used to Get current state value.
 */
uint8_t StateMachine_GetCurrentState(StateMachine *pThis)
{
	uint8_t u8State;
	if(pThis->m_u8CurrentState == pThis->m_u8NextState)
	{
		u8State = pThis->m_u8CurrentState;
	}
	else
	{
		u8State = pThis->m_u8NextState;
	}
	return u8State;
}

/**
 *\brief This function is used to Get current state value.
 */
uint8_t StateMachine_GetPreviousState(StateMachine *pThis)
{
	return pThis->m_u8PreviousState;
}

/**
 *\brief This function is used to switch state machine substate to next substate. 
 */
ErrorCode StateMachine_SetSubState(StateMachine *pThis, uint8_t u8SubState)
{
	ErrorCode u8Ret = ErrorCode_InvalidParameter;
	
	if(NULL != pThis)
	{
		pThis->m_u8SubState = u8SubState;
	}
	else
	{
		;/* Do Nothing */
	}
	return u8Ret;
}

/**
 *\brief This function is used to exit state machine and clear state machine instance. 
 */
ErrorCode StateMachine_Exit(StateMachine *pThis)
{
	ErrorCode u8Ret = ErrorCode_InvalidParameter;
	
	if(NULL != pThis)
	{
		memset(pThis, 0U, sizeof(*pThis));
	}
	else
	{
		;/* Do Nothing */
	}
	return u8Ret;
}

/* ORIGINAL LABEL END */
/****************************************************************************/
/*									End Of File 							*/
/****************************************************************************/
