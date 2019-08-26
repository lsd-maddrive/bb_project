#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <ch.h>
#include <hal.h>

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define     PROGRAM_ROUTINE_MASTER                      0
/***    JUST FOR EXAMPLE (TEMPORARY)           ***/
#define     PROGRAM_ROUTINE_TEST                        1


#define     MAIN_PROGRAM_ROUTINE                        PROGRAM_ROUTINE_TEST


/**************/
/*** MACROS ***/
/**************/

#define CLIP_VALUE(x, min, max) ((x) < (min) ? (min) :      \
                                 (x) > (max) ? (max) : (x))

/*************/
/*** DEBUG ***/
/*************/

void debug_stream_init( void );
void dbgprintf( const char* format, ... );


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_COMMON_H_ */
