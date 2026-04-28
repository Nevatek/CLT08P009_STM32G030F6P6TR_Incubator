
/* ORIGINAL LABEL START */
/****************************************************************************/
/* Conditional compilation definitions										*/
/****************************************************************************/
#ifndef STATEMACHINE_H__
#define STATEMACHINE_H__

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
#define SUBSTATE_ENTRY			(0U)
#define SUBSTATE_RUN			(1U)
#define SUBSTATE_EXIT			(2U)

#define STATE_STATE				(0U)
#define FOLLOW_THROUGH_STATE	(1U)

/****************************************************************************/
/* Typedefs/Structure/Union definitions										*/
/****************************************************************************/
struct StateMachineTag;

typedef void (*pState_Entry)(struct StateMachineTag *pThis, uint8_t u8CurrentState, void *pThisModule, void *pParam);
typedef void (*pState_Run)(struct StateMachineTag *pThis, uint8_t u8CurrentState, void *pThisModule, void *pParam);
typedef void (*pState_Exit)(struct StateMachineTag *pThis, uint8_t u8CurrentState, uint8_t u8ExitState, void *pThisModule, void *pParam);

typedef struct{
	pState_Run		m_StateRun;
	uint8_t			m_u8State;
	uint8_t			m_u8Flag;
	pState_Entry	m_StateEntry;
	pState_Exit		m_StateExit;
}StateMachineData;

typedef struct StateMachineTag{
	StateMachineData *m_pData;
	void			*m_pThisModule;
	uint8_t			m_u8CurrentState;
	uint8_t			m_u8NextState;
	uint8_t			m_u8PreviousState;
	uint8_t			m_u8SubState;
	uint8_t			m_u8Size;
}StateMachine;

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

ErrorCode StateMachine_Init(StateMachine *pThis);
ErrorCode StateMachine_Config(StateMachine *pThis, StateMachineData *pData, uint8_t u8Size, uint8_t u8DefaultState, void *pThisModule);
ErrorCode StateMachine_Run(StateMachine *pThis, void *pParam);
ErrorCode StateMachine_SetNextState(StateMachine *pThis, uint8_t u8NextState);
uint8_t StateMachine_GetCurrentState(StateMachine *pThis);
uint8_t StateMachine_GetPreviousState(StateMachine *pThis);
ErrorCode StateMachine_SetSubState(StateMachine *pThis, uint8_t u8SubState);
ErrorCode StateMachine_Exit(StateMachine *pThis);

#endif /* STATEMACHINE_H__ */

/* ORIGINAL LABEL END */
/****************************************************************************/
/*									End Of File								*/
/****************************************************************************/
