cmake_minimum_required ( VERSION "3.13.2" )

set ( GLOBAL_SYSTEM_TARGET "swc" )
include ( BuildTemplate/Config.cmake.in )

project ( swctool )

set ( MODULE_NAME "fsm" )
set ( MODULE_VERNO 1.0.0 )

set ( MODULE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set ( MODULE_SOURCE_C_DIR ${MODULE_ROOT_DIR}/source )
set ( MODULE_EXTRA_COMPILE_OPTIONS "-DBUILD_FSM_STATE_FAST_FIND" )

set ( ENABLE_BUILD_SHARED_LIBRARY ON CACHE BOOL "Build fsm tool" FORCE )
include ( BuildTemplate/SharedLibrary.cmake.in )

set ( MODULE_DAEMON_DIR ${MODULE_ROOT_DIR}/examples )
set ( ENABLE_BUILD_DAEMON ON CACHE BOOL "Build FSM daemon" FORCE )
set ( MODULE_EXTERNAL_DAEMON_LIB fsm pthread )
include ( BuildTemplate/Daemon.cmake.in )
