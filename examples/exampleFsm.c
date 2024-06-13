#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "SWC_Fsm.h"

// ======== state ========
#define DEF_STATE_Example_Standby       ( 0U )
#define DEF_STATE_Example_A             ( 1U )
#define DEF_STATE_Example_B             ( 2U )
#define DEF_STATE_Example_C             ( 3U )
// ======== state end ========

// ======== inner variable ======== 
static volatile char    s_innerInputChar = ' ';   
// ======== inner variable end ======== 

// ======== transfer check ======== 
static bool         checkTransEnvironment( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm );

// DEF_SWC_INNER_TEMPLATE_STATE_A <-> DEF_SWC_INNER_TEMPLATE_STATE_B
static bool         checkTransStandby2A( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm );
static bool         checkTransA2B( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm );
static bool         checkTransA2C( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm );
static bool         checkTransB2C( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm );
static bool         checkTransC2Standby( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm );
// ======== transfer check end ======== 

// ======== state action ======== 
static int32_t      actionStandbyPrepare( void *item );
static int32_t      actionStandbyRoutine( void *item );
static int32_t      actionStandbyExit( void *item );

static int32_t      actionAPrepare( void *item );
static int32_t      actionARoutine( void *item );
static int32_t      actionAExit( void *item );

static int32_t      actionBPrepare( void *item );
static int32_t      actionBRoutine( void *item );
static int32_t      actionBExit( void *item );

static int32_t      actionCPrepare( void *item );
static int32_t      actionCRoutine( void *item );
static int32_t      actionCExit( void *item );
// ======== state action end ======== 

// ======== fsm ======== 
static bool         exampleFsmInnerInit( void *fsm );
static bool         exampleFsmInnerRoutine( void *fsm );
static bool         exampleFsmInnerExit( void *fsm );
static int32_t      exampleFsmTransTo( TYPE_SWC_FSM_STATE state, bool bForce );
// ======== fsm end ======== 

#define SWC_FSM_CONTEXT_ID_EXAMPLE              ( 1U )
#define SWC_FSM_ROUTINE_INTERVAL_EXAMPLE        ( 100U )    // 100ms

DECLARE_SWC_FSM_CONTEXT( Example, SWC_FSM_CONTEXT_ID_EXAMPLE,
            DECLARE_SWC_FSM_STATES(
                DECLARE_SWC_FSM_STATE( DEF_STATE_Example_Standby, actionStandbyPrepare, actionStandbyRoutine, actionStandbyExit, SWC_FSM_ROUTINE_INTERVAL_EXAMPLE ),
                DECLARE_SWC_FSM_STATE( DEF_STATE_Example_A, actionAPrepare, actionARoutine, actionAExit, SWC_FSM_ROUTINE_INTERVAL_EXAMPLE ),
                DECLARE_SWC_FSM_STATE( DEF_STATE_Example_B, actionBPrepare, actionBRoutine, actionBExit, SWC_FSM_ROUTINE_INTERVAL_EXAMPLE ),
                DECLARE_SWC_FSM_STATE( DEF_STATE_Example_C, actionCPrepare, actionCRoutine, actionCExit, SWC_FSM_ROUTINE_INTERVAL_EXAMPLE ),
            ),
            DECLARE_SWC_FSM_TRANSITIONS(
                DECLARE_SWC_FSM_TRANSITION( DEF_STATE_Example_Standby, DEF_STATE_Example_A, checkTransStandby2A ),
                DECLARE_SWC_FSM_TRANSITION( DEF_STATE_Example_A, DEF_STATE_Example_B, checkTransA2B ),
                DECLARE_SWC_FSM_TRANSITION( DEF_STATE_Example_A, DEF_STATE_Example_C, checkTransA2C ),
                DECLARE_SWC_FSM_TRANSITION( DEF_STATE_Example_B, DEF_STATE_Example_C, checkTransB2C ),
                DECLARE_SWC_FSM_TRANSITION( DEF_STATE_Example_C, DEF_STATE_Example_Standby, checkTransC2Standby ),
            ),
            checkTransEnvironment, DEF_STATE_Example_Standby, SWC_FSM_ROUTINE_INTERVAL_EXAMPLE, exampleFsmInnerInit, exampleFsmInnerRoutine, exampleFsmInnerExit )

bool exampleFsmInnerInit( void *fsm )       { UNUSED( fsm ); printf( "exampleFsmInnerInit\n" ); return true; }
bool exampleFsmInnerRoutine( void *fsm )    { UNUSED( fsm ); return true; }
bool exampleFsmInnerExit( void *fsm )       { UNUSED( fsm ); printf( "exampleFsmInnerExit\n" ); return true; }

int32_t exampleFsmTransTo( TYPE_SWC_FSM_STATE state, bool bForce )
{
    printf( "exampleFsmTransTo from %d to %d, %d\n", swcFsmGetCurState( DECLARE_SWC_FSM_CONTEXT_REF( Example ) ), state, bForce ? 1 : 0 );
    return swcFsmTransTo( state, bForce, DECLARE_SWC_FSM_CONTEXT_REF( Example ) );
}

bool checkTransEnvironment( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm )
{
    UNUSED( from );
    UNUSED( to );
    UNUSED( fsm );

    return true;
}

bool checkTransStandby2A( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm )
{
    UNUSED( from );
    UNUSED( to );
    UNUSED( fsm );

    if ( ( rand() % 10 ) >= 5 ) {
        printf( "mock checkTransStandby2A failed\n" );
        return false;
    } else {
        return true;
    }
}

bool checkTransA2B( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm )
{
    UNUSED( from );
    UNUSED( to );
    UNUSED( fsm );

    if ( ( rand() % 10 ) >= 5 ) {
        printf( "mock checkTransA2B failed\n" );
        return false;
    } else {
        return true;
    }
}

bool checkTransA2C( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm )
{
    UNUSED( from );
    UNUSED( to );
    UNUSED( fsm );

    if ( ( rand() % 10 ) >= 5 ) {
        printf( "mock checkTransA2C failed\n" );
        return false;
    } else {
        return true;
    }
}

bool checkTransB2C( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm )
{
    UNUSED( from );
    UNUSED( to );
    UNUSED( fsm );

    if ( ( rand() % 10 ) >= 5 ) {
        printf( "mock checkTransB2C failed\n" );
        return false;
    } else {
        return true;
    }
}

bool checkTransC2Standby( TYPE_SWC_FSM_STATE from, TYPE_SWC_FSM_STATE to, void *fsm )
{
    UNUSED( from );
    UNUSED( to );
    UNUSED( fsm );

    if ( ( rand() % 10 ) >= 5 ) {
        printf( "mock checkTransC2Standby failed\n" );
        return false;
    } else {
        return true;
    }
}

static uint32_t s_StandbyRoutineInterval = 0;
int32_t actionStandbyPrepare( void *item )  { UNUSED( item ); printf( "actionStandbyPrepare\n" ); s_StandbyRoutineInterval = 0; return 0; }
int32_t actionStandbyExit( void *item )     { UNUSED( item ); printf( "actionStandbyExit\n" ); s_StandbyRoutineInterval = 0; return 0; }
int32_t actionStandbyRoutine( void *item )
{ 
    SWCFsmStateItem *state = ( SWCFsmStateItem * )item;

    s_StandbyRoutineInterval += SWC_FSM_ROUTINE_INTERVAL_EXAMPLE;
    if ( s_StandbyRoutineInterval >= state->routineInterval ) {
        s_StandbyRoutineInterval = 0;

        if ( ( s_innerInputChar == 'A' ) || ( s_innerInputChar == 'a' ) ) {
            exampleFsmTransTo( DEF_STATE_Example_A, false );

            return 0;
        }
    }
    return 0; 
}

static uint32_t s_ARoutineInterval = 0;
int32_t actionAPrepare( void *item )        { UNUSED( item ); printf( "actionAPrepare\n" ); s_ARoutineInterval = 0; return 0; }
int32_t actionAExit( void *item )           { UNUSED( item ); printf( "actionAExit\n" ); s_ARoutineInterval = 0; return 0; }
int32_t actionARoutine( void *item )
{ 
    SWCFsmStateItem *state = ( SWCFsmStateItem * )item;

    s_ARoutineInterval += SWC_FSM_ROUTINE_INTERVAL_EXAMPLE;
    if ( s_ARoutineInterval >= state->routineInterval ) {
        s_ARoutineInterval = 0;

        if ( ( s_innerInputChar == 'B' ) || ( s_innerInputChar == 'b' ) ) {
            exampleFsmTransTo( DEF_STATE_Example_B, false );

            return 0;
        }

        if ( ( s_innerInputChar == 'C' ) || ( s_innerInputChar == 'c' ) ) {
            exampleFsmTransTo( DEF_STATE_Example_C, false );

            return 0;
        }
    }
    return 0; 
}

static uint32_t s_BRoutineInterval = 0;
int32_t actionBPrepare( void *item )        { UNUSED( item ); printf( "actionBPrepare\n" ); s_BRoutineInterval = 0; return 0; }
int32_t actionBExit( void *item )           { UNUSED( item ); printf( "actionBExit\n" ); s_BRoutineInterval = 0; return 0; }
int32_t actionBRoutine( void *item )
{ 
    SWCFsmStateItem *state = ( SWCFsmStateItem * )item;

    s_BRoutineInterval += SWC_FSM_ROUTINE_INTERVAL_EXAMPLE;
    if ( s_BRoutineInterval >= state->routineInterval ) {
        s_BRoutineInterval = 0;
    
        if ( ( s_innerInputChar == 'C' ) || ( s_innerInputChar == 'c' ) ) {
            exampleFsmTransTo( DEF_STATE_Example_C, false );

            return 0;
        }
    }
    return 0; 
}

static uint32_t s_CRoutineInterval = 0;
int32_t actionCPrepare( void *item )        { UNUSED( item ); printf( "actionCPrepare\n" ); s_CRoutineInterval = 0; return 0; }
int32_t actionCExit( void *item )           { UNUSED( item ); printf( "actionCExit\n" ); s_CRoutineInterval = 0; return 0; }
int32_t actionCRoutine( void *item )
{
    SWCFsmStateItem *state = ( SWCFsmStateItem * )item;

    s_CRoutineInterval += SWC_FSM_ROUTINE_INTERVAL_EXAMPLE;
    if ( s_CRoutineInterval >= state->routineInterval ) {
        s_CRoutineInterval = 0;

        if ( ( s_innerInputChar == 'S' ) || ( s_innerInputChar == 's' ) ) {
            exampleFsmTransTo( DEF_STATE_Example_Standby, false );

            return 0;
        }
    }
    return 0;
}

// ============== main loop ==============
static bool b_innerLoopEnable = true;
static void* task_100ms( void *arg )
{
    UNUSED( arg );

    printf( "start task_100ms\n" );
    while( b_innerLoopEnable ) {
        swcFsmRoutine( DECLARE_SWC_FSM_CONTEXT_REF( Example ) );
        usleep( SWC_FSM_ROUTINE_INTERVAL_EXAMPLE * 1000 );
    }

    return NULL;
}

static void* updateInput( void *arg )
{
    char ch = ' ';
    UNUSED( arg );

    printf( "start updateInput\n" );
    while( b_innerLoopEnable ) {
        ch = getchar();
        if ( ch != '\n' ) {
            s_innerInputChar = ch;
            printf( "updateInput: %c\n", s_innerInputChar );
        }
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t tidTask, tidUpdate;

    UNUSED( argc );
    UNUSED( argv );

    swcFsmInit( DECLARE_SWC_FSM_CONTEXT_REF( Example ) );

    pthread_create( &tidTask, NULL ,&task_100ms, NULL );
    pthread_create( &tidUpdate, NULL ,&updateInput, NULL );

    while( ( s_innerInputChar != 'E' ) && ( s_innerInputChar != 'e' ) );

    b_innerLoopEnable = false;
    pthread_join( tidUpdate, NULL );
    pthread_join( tidTask, NULL );

    swcFsmExit( DECLARE_SWC_FSM_CONTEXT_REF( Example ) );

    return 0;
}

