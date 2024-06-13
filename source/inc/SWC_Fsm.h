
/**
 * @file        SWC_Fsm.h
 * @author      ddkv587 ( ddkv587@gmail.com )
 * @brief 
 * @date        2024-03-04
 * @details     
 * @copyright   Copyright (c) 2024
 * @note
 * sdk:
 * platform:
 * project:     swctool
 * @version
 * <table>
 * <tr><th>Date        <th>Version  <th>Author          <th>Description
 * <tr><td>2024/03/04  <td>1.0      <td>                <td>init version
 * </table>
 */
#ifndef SWC_FSM_H_
#define SWC_FSM_H_

#include <stdbool.h>
#include "SWC_FsmMacroDefine.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef bool        ( *ptrSWCFunFSMAction )( void *fsm );
typedef bool        ( *ptrSWCFunTransferCheck )( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm );
typedef int32_t     ( *ptrSWCFunTransferAction )( void *fsm );

typedef struct
{
    TYPE_SWC_FSM_STATE              state;
    ptrSWCFunTransferAction         entry;
    ptrSWCFunTransferAction         routine;
    ptrSWCFunTransferAction         exit;
    uint32_t                        routineInterval;
} SWCFsmStateItem;

typedef struct
{
    TYPE_SWC_FSM_STATE              curState;
    TYPE_SWC_FSM_STATE              nextState;
    ptrSWCFunTransferCheck          transCheck;
} SWCFsmTransItem;

typedef struct 
{
    TYPE_SWC_FSM_STATE              initState;
    TYPE_SWC_FSM_STATE              curState;
    TYPE_SWC_FSM_STATE              preState;
    SWCFsmStateItem*                fsmStateList;
    SWCFsmTransItem*                fsmTransTable;
    ptrSWCFunTransferCheck          fsmCommonCheck;
    uint16_t                        fsmStateSize;
    uint16_t                        fsmTransitionSize;
    uint32_t                        fsmRoutineInterval;
    TYPE_SWC_FSM_CONTEXT_ID         fsmContextID;
    ptrSWCFunFSMAction              fsmInit;
    ptrSWCFunFSMAction              fsmRoutine;
    ptrSWCFunFSMAction              fsmExit;
} SWCFsmContext;

extern bool                         swcFsmInit( SWCFsmContext* fsm );
extern void                         swcFsmRoutine( SWCFsmContext* fsm );
extern void                         swcFsmExit( SWCFsmContext* fsm );
extern int32_t                      swcFsmTransTo( TYPE_SWC_FSM_STATE state, bool bForce, SWCFsmContext* fsm );
extern int32_t                      swcFsmGoBack( bool bForce, SWCFsmContext* fsm );

extern TYPE_SWC_FSM_STATE           swcFsmGetCurState( SWCFsmContext* fsm );
extern TYPE_SWC_FSM_STATE           swcFsmGetPreState( SWCFsmContext* fsm );

#ifdef __cplusplus
}
#endif
#endif

