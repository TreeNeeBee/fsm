
/**
 * @file        SWC_Fsm.h
 * @author      ddkv587 ( ddkv587@gmail.com )
 * @brief 
 * @date        2025-09-18
 * @details     
 * @copyright   Copyright (c) 2025
 * @note
 * sdk:
 * platform:
 * project:     swctool
 * @version
 * <table>
 * <tr><th>Date        <th>Version  <th>Author          <th>Description
 * <tr><td>2024/03/04  <td>1.0      <td>                <td>init version
 * <tr><td>2025/09/18  <td>1.0      <td>                <td>make all in one header file
 * </table>
 */
#ifndef SWC_FSM_H_
#define SWC_FSM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FSM_VERSION     1

// Inline control for debugging and optimization
#ifdef FSM_DEBUG
#define FSM_INLINE static
#define FSM_FUNC static
#else
#define FSM_INLINE static inline
#define FSM_FUNC static
#endif

typedef uint8_t                                 fsm_bool_t;
#define DEF_FSM_TRUE                            1
#define DEF_FSM_FALSE                           0

#ifndef NULL
#define NULL                                    ( ( void* )0 )
#endif

#ifndef UNUSED
#define UNUSED(X)                               ( void )( X )
#endif

typedef uint16_t                                fsm_context_id_t;
typedef uint16_t                                fsm_state_t;

#define DEF_SWC_FSM_CONTEXT_ID_RESERVE          ( ( fsm_context_id_t )0U )
#define DEF_SWC_FSM_CONTEXT_ID_INVALID          ( ( fsm_context_id_t )0xFFFFU )

//#define DEF_SWC_FSM_STATE_RESERVE               ( ( fsm_state_t )0U )
#define DEF_SWC_FSM_STATE_INVALID               ( ( fsm_state_t )0xFFFFU )

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

typedef fsm_bool_t          ( *ptrSWCFunFSMAction )( void *fsm );
typedef fsm_bool_t          ( *ptrSWCFunTransferCheck )( fsm_state_t from, fsm_state_t to, void *fsm );
typedef int32_t             ( *ptrSWCFunTransferAction )( void *fsm );

typedef struct
{
    fsm_state_t                     state;
    ptrSWCFunTransferAction         entry;
    ptrSWCFunTransferAction         routine;
    ptrSWCFunTransferAction         exit;
    uint32_t                        routineInterval;
} SWCFsmStateItem;

typedef struct
{
    fsm_state_t                     curState;
    fsm_state_t                     nextState;
    ptrSWCFunTransferCheck          transCheck;
} SWCFsmTransItem;

typedef struct 
{
    fsm_state_t                     initState;
    fsm_state_t                     curState;
    fsm_state_t                     preState;
    SWCFsmStateItem*                fsmStateList;
    SWCFsmTransItem*                fsmTransTable;
    ptrSWCFunTransferCheck          fsmCommonCheck;
    uint16_t                        fsmStateSize;
    uint16_t                        fsmTransitionSize;
    uint32_t                        fsmRoutineInterval;
    fsm_context_id_t                fsmContextID;
    ptrSWCFunFSMAction              fsmInit;
    ptrSWCFunFSMAction              fsmRoutine;
    ptrSWCFunFSMAction              fsmExit;
} SWCFsmContext;

#ifdef FSM_STATE_FF
#define swcFsmGetStateItem( state, context )        ( ( context && ( ( state ) != DEF_SWC_FSM_STATE_INVALID ) ) ? ( & ( context->fsmStateList[ state ] ) ) : NULL )
#define swcFsmGetCurStateItem( context )            ( context ? swcFsmGetStateItem( context->curState, context ) : NULL )
#else
FSM_FUNC
SWCFsmStateItem* swcFsmGetStateItem( fsm_state_t state, SWCFsmContext* context )
{
    uint8_t index = 0;

    UNUSED( index );

    if ( !context || state == DEF_SWC_FSM_STATE_INVALID )     return NULL;

    for ( index = 0; index < context->fsmStateSize; ++index ) {
        if ( context->fsmStateList[ index ].state == state ) {
            return &( context->fsmStateList[index] );
        }
    }

    return NULL;
}

FSM_FUNC
SWCFsmStateItem* swcFsmGetCurStateItem( SWCFsmContext* context )
{
    if ( !context )     return NULL;

    return swcFsmGetStateItem( context->curState, context );
}
#endif

#ifdef FSM_IMPLEMENTATION
#define FSM_NO_IMPL
#endif

#ifndef FSM_NO_IMPL
FSM_INLINE
fsm_state_t swcFsmGetCurState( SWCFsmContext* context )
{
    if ( !context )     return DEF_SWC_FSM_STATE_INVALID;

    return context->curState;
}

FSM_INLINE
fsm_state_t swcFsmGetPreState( SWCFsmContext* context )
{
    if ( !context )     return DEF_SWC_FSM_STATE_INVALID;

    return context->preState;
}

FSM_FUNC
int32_t swcFsmTransTo( fsm_state_t state, fsm_bool_t bForce, SWCFsmContext* context )
{
    uint8_t index = 0;
    int32_t ret = -1;   // not match
    fsm_bool_t bTransition = DEF_FSM_FALSE;
    SWCFsmTransItem *curfsmTransItem = NULL;
    SWCFsmStateItem *curStateItem = NULL;
    SWCFsmStateItem *nextStateItem = NULL;

    UNUSED( bForce );
    UNUSED( index );
    UNUSED( ret );
    UNUSED( bTransition );
    UNUSED( curfsmTransItem );
    UNUSED( curStateItem );
    UNUSED( nextStateItem );

    if ( ( !context ) || ( state == DEF_SWC_FSM_STATE_INVALID ) ) return -1;

    if ( ( bForce == DEF_FSM_FALSE ) && ( context->curState == state ) ) return 0;

    for ( index = 0; index < context->fsmTransitionSize; ++index ) {
        if ( context->fsmTransTable[index].nextState == state ) {
            if ( bForce == DEF_FSM_TRUE ) {
                bTransition = DEF_FSM_TRUE;
                break;
            } else {
                curfsmTransItem = &( context->fsmTransTable[index] );
                if ( ( context->fsmTransTable[index].curState == context->curState ) && \
                        ( !( context->fsmCommonCheck ) || ( ( *( context->fsmCommonCheck  ) ) ( context->curState, state, context ) ) == DEF_FSM_TRUE ) && \
                        ( !( curfsmTransItem->transCheck ) || ( ( *( curfsmTransItem->transCheck ) ) ( context->curState, state, context ) ) ) == DEF_FSM_TRUE ) {
                    bTransition = DEF_FSM_TRUE;
                    break;
                }
            }
        }
    }

    if ( bTransition == DEF_FSM_TRUE ) {
        curStateItem    = swcFsmGetStateItem( context->curState, context );
        nextStateItem   = swcFsmGetStateItem( state, context );

        if ( curStateItem && curStateItem->exit ) {
            // call exit action for cur state
            ( *( curStateItem->exit ) ) ( curStateItem );
        }

        if ( nextStateItem && nextStateItem->entry ) {
            // call entry action for next state
            ret = ( *( nextStateItem->entry ) ) ( nextStateItem );
        }

        // update state
        if ( context->curState != state ) {
            context->preState = context->curState;
        }
        context->curState = state;
    }

    return ret;
}

FSM_FUNC
fsm_bool_t swcFsmInit( SWCFsmContext* context )
{
    if ( context ) {
        if ( ( context->fsmInit ) && !( ( *( context->fsmInit ) )( context ) ) ) return DEF_FSM_FALSE;

        // transfer to init state
        return ( 0 == swcFsmTransTo( context->initState, DEF_FSM_TRUE, context ) );
    }

    return DEF_FSM_TRUE;
}

FSM_FUNC
void swcFsmRoutine( SWCFsmContext* context )
{
    SWCFsmStateItem* state = NULL;

    UNUSED( state );

    if ( !context ) return;

    if ( context->fsmRoutine )  ( *( context->fsmRoutine ) )( context );

    state = swcFsmGetCurStateItem( context );

    if ( state && state->routine )  ( *( state->routine ) )( state );
}

FSM_FUNC
void swcFsmExit( SWCFsmContext* context )
{
    SWCFsmStateItem* state = NULL;

    UNUSED( state );

    if ( !context ) return;

    state = swcFsmGetCurStateItem( context );

    if ( state && state->exit )  ( *( state->exit ) )( state );

    if ( context->fsmExit )  ( *( context->fsmExit ) )( context );
}

FSM_FUNC
int32_t swcFsmGoBack( fsm_bool_t bForce, SWCFsmContext* context )
{
    if ( !context )     return -1;

    return swcFsmTransTo( context->preState, bForce, context );
}

#else
// implement these function in .c if defined FSM_IMPLEMENTATION 
extern fsm_bool_t                   swcFsmInit( SWCFsmContext* context );
extern void                         swcFsmRoutine( SWCFsmContext* fsm );
extern void                         swcFsmExit( SWCFsmContext* fsm );
extern int32_t                      swcFsmTransTo( fsm_state_t state, fsm_bool_t bForce, SWCFsmContext* fsm );
extern int32_t                      swcFsmGoBack( fsm_bool_t bForce, SWCFsmContext* fsm );

extern fsm_state_t                  swcFsmGetCurState( SWCFsmContext* fsm );
extern fsm_state_t                  swcFsmGetPreState( SWCFsmContext* fsm );
#endif

#ifdef __cplusplus
}
#endif
#endif

