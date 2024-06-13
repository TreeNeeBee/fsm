
/**
 * @file        SWC_FsmMacroDefine.h
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
#ifndef SWC_FSMMACRODEFINE_H_
#define SWC_FSMMACRODEFINE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNUSED
#define UNUSED(X)                               ( void )( X )
#endif

typedef uint16_t                                TYPE_SWC_FSM_CONTEXT_ID;
typedef uint16_t                                TYPE_SWC_FSM_STATE;

#define DEF_SWC_FSM_CONTEXT_ID_RESERVE          ( ( TYPE_SWC_FSM_CONTEXT_ID )0U )
#define DEF_SWC_FSM_CONTEXT_ID_INVALID          ( ( TYPE_SWC_FSM_CONTEXT_ID )0xFFFFU )

//#define DEF_SWC_FSM_STATE_RESERVE               ( ( TYPE_SWC_FSM_STATE )0U )
#define DEF_SWC_FSM_STATE_INVALID               ( ( TYPE_SWC_FSM_STATE )0xFFFFU )

#define DECLARE_SWC_FSM_CONTEXT( _name, _fsmID, _fsmStateList, _fsmTransTable, _fsmCommonCheck, _fsmInitState, _fsmRoutineInterval, _fsmInit, _fsmRoutine, _fsmExit ) \
    static SWCFsmStateItem s_fsmState##_name[] = {_fsmStateList}; \
    static SWCFsmTransItem s_fsmTransTable##_name[] = {_fsmTransTable}; \
    static SWCFsmContext s_fsmContext##_name = { \
        .initState              = _fsmInitState, \
        .curState               = DEF_SWC_FSM_STATE_INVALID, \
        .preState               = DEF_SWC_FSM_STATE_INVALID, \
        .fsmStateList           = ( s_fsmState##_name ), \
        .fsmTransTable          = ( s_fsmTransTable##_name ), \
        .fsmCommonCheck         = _fsmCommonCheck, \
        .fsmStateSize           = sizeof( s_fsmState##_name ) / sizeof( SWCFsmStateItem ), \
        .fsmTransitionSize      = sizeof( s_fsmTransTable##_name ) / sizeof( SWCFsmTransItem ), \
        .fsmRoutineInterval     = _fsmRoutineInterval, \
        .fsmContextID           = _fsmID, \
        .fsmInit                = _fsmInit, \
        .fsmRoutine             = _fsmRoutine, \
        .fsmExit                = _fsmExit \
    };

#define DECLARE_SWC_FSM_CONTEXT_REF( _name ) ( &( s_fsmContext##_name ) )

#define DECLARE_SWC_FSM_STATE( _state, _entry, _routine, _exit, _routineInterval ) \
    { \
        .state                  = _state, \
        .entry                  = _entry, \
        .routine                = _routine, \
        .exit                   = _exit, \
        .routineInterval        = _routineInterval \
    }

#define DECLARE_SWC_FSM_TRANSITION( _curState, _nextState, _transCheck ) \
    { \
        .curState               = _curState, \
        .nextState              = _nextState, \
        .transCheck             = _transCheck \
    }

#define DECLARE_SWC_FSM_STATES(...)        __VA_ARGS__
#define DECLARE_SWC_FSM_TRANSITIONS(...)   __VA_ARGS__

#ifdef __cplusplus
}
#endif
#endif

