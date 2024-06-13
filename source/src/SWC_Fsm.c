
#include <string.h>
#include "SWC_Fsm.h"

#ifdef BUILD_FSM_STATE_FAST_FIND
#define swcFsmGetStateItem( state, context )        ( context ? ( & ( context->fsmStateLis[ state ] ) : NULL )
#define swcFsmGetCurStateItem( context )            ( context ? swcFsmGetStateItem( context->curState, context ) : NULL )
#else
SWCFsmStateItem* swcFsmGetStateItem( TYPE_SWC_FSM_STATE state, SWCFsmContext* context )
{
    uint8_t index = 0;

    UNUSED( index );

    if ( !context )     return NULL;

    for ( index = 0; index < context->fsmStateSize; ++index ) {
        if ( context->fsmStateList[ index ].state == state ) {
            return &( context->fsmStateList[index] );
        }
    }

    return NULL;
}

SWCFsmStateItem* swcFsmGetCurStateItem( SWCFsmContext* context )
{
    if ( !context )     return NULL;

    return swcFsmGetStateItem( context->curState, context );
}
#endif

bool swcFsmInit( SWCFsmContext* context )
{
    if ( context ) {
        if ( ( context->fsmInit ) && !( ( *( context->fsmInit ) )( context ) ) ) return false;

        // transfer to init state
        return ( 0 == swcFsmTransTo( context->initState, true, context ) );
    }

    return true;
}

void swcFsmRoutine( SWCFsmContext* context )
{
    SWCFsmStateItem* state = NULL;

    UNUSED( state );

    if ( !context ) return;

    if ( context->fsmRoutine )  ( *( context->fsmRoutine ) )( context );

    state = swcFsmGetCurStateItem( context );

    if ( state && state->routine )  ( *( state->routine ) )( state );
}

void swcFsmExit( SWCFsmContext* context )
{
    SWCFsmStateItem* state = NULL;

    UNUSED( state );

    if ( !context ) return;

    state = swcFsmGetCurStateItem( context );

    if ( state && state->exit )  ( *( state->exit ) )( state );

    if ( context->fsmExit )  ( *( context->fsmExit ) )( context );
}

int32_t swcFsmTransTo( TYPE_SWC_FSM_STATE state, bool bForce, SWCFsmContext* context )
{
    uint8_t index = 0;
    int32_t ret = -1;   // not match
    bool bTransition = false;
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

    if ( ( !bForce ) && ( context->curState == state ) ) return 0;

    for ( index = 0; index < context->fsmTransitionSize; ++index ) {
        if ( context->fsmTransTable[index].nextState == state ) {
            if ( bForce ) {
                bTransition = true;
                break;
            } else {
                curfsmTransItem = &( context->fsmTransTable[index] );
                if ( ( context->fsmTransTable[index].curState == context->curState ) && \
                        ( !( context->fsmCommonCheck ) || ( ( *( context->fsmCommonCheck  ) ) ( context->curState, state, context ) ) ) && \
                        ( !( curfsmTransItem->transCheck ) || ( ( *( curfsmTransItem->transCheck ) ) ( context->curState, state, context ) ) ) ) {
                    bTransition = true;
                    break;
                }
            }
        }
    }

    if ( bTransition ) {
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

int32_t swcFsmGoBack( bool bForce, SWCFsmContext* context )
{
    if ( !context )     return -1;

    return swcFsmTransTo( context->preState, bForce, context );
}

TYPE_SWC_FSM_STATE swcFsmGetCurState( SWCFsmContext* context )
{
    if ( !context )     return DEF_SWC_FSM_STATE_INVALID;

    return context->curState;
}

TYPE_SWC_FSM_STATE swcFsmGetPreState( SWCFsmContext* context )
{
    if ( !context )     return DEF_SWC_FSM_STATE_INVALID;

    return context->preState;
}
