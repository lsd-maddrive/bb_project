#include <tests.h>
#include <lld_encoder.h>
#include <lld_control.h>

// #define WITH_MOTOR
/**
 * @brief   Test common encoder routine
 * @note    Print number of ticks, (int)revs and state of direction
 *          0   - clockwise
 *          1   - counterclockwise
 *
 *          #define WITH_MOTOR enable you to use motor
 */
void testEncoderRoutine( void )
{
    debug_stream_init( );
    lldEncoderInit( );

#ifdef WITH_MOTOR
    lldControlInit( );

    lldControlValue_t   test_duty1_prc    = 0;
    lldControlValue_t   test_duty2_prc    = 0;
    lldControlValue_t   test_duty3_prc    = 0;
    lldControlValue_t   test_delta_prc    = 10;
#endif
    // Motor 1
    encoderTicksValue_t test_ticks1  = 0;
    encoderRevsValue_t  test_revs1   = 0;
    bool                test_dir1    = 0;

    // Motor 2
    encoderTicksValue_t test_ticks2  = 0;
    encoderRevsValue_t  test_revs2   = 0;
    bool                test_dir2    = 0;

    // Motor 3
    encoderTicksValue_t test_ticks3  = 0;
    encoderRevsValue_t  test_revs3   = 0;
    bool                test_dir3    = 0;

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        test_ticks1  = lldGetEncoderTicks( A );
        test_revs1   = lldGetEncoderRevs( A );
        test_dir1    = lldGetEncoderDirection( A );

        test_ticks2  = lldGetEncoderTicks( B );
        test_revs2   = lldGetEncoderRevs( B );
        test_dir2    = lldGetEncoderDirection( B );

        test_ticks3  = lldGetEncoderTicks( C );
        test_revs3   = lldGetEncoderRevs( C );
        test_dir3    = lldGetEncoderDirection( C );

#ifdef WITH_MOTOR
        char rcv_data   = sdGetTimeout( &SD3, TIME_IMMEDIATE );

        switch( rcv_data )
        {
            case 'q':   // Motor A
                test_duty1_prc += test_delta_prc;
                break;

            case 'w':   // Motor A
                test_duty1_prc -= test_delta_prc;
                break;

            case 'a':   // Motor B
                test_duty2_prc += test_delta_prc;
                break;

            case 's':   // Motor B
                test_duty2_prc -= test_delta_prc;
                break;

            case 'z':   // Motor c
                test_duty3_prc  += test_delta_prc;
                break;

            case 'x':   // Motor C
                test_duty3_prc  -= test_delta_prc;
                break;

            case ' ':
                test_duty1_prc  = 0;
                test_duty2_prc  = 0;
                test_duty3_prc  = 0;
                break;

            default:
                break;
        }
        lldControlSetMotorPower( A, test_duty1_prc );
        lldControlSetMotorPower( B, test_duty2_prc );
        lldControlSetMotorPower( C, test_duty3_prc );

        dbgprintf( "Power: %d\n\r"
                   "T1: %d\tT2: %d\tT3: %d\n\r"
                   "R1: %d\tR2: %d\tR3: %d\n\r"
                   "D1: %d\tD2: %d\tD3: %d\n\r",
                   test_ticks1, test_ticks2, test_ticks3,
                   (int32_t)test_revs1, (int32_t)test_revs2, (int32_t)test_revs3,
                   test_dir1, test_dir2, test_dir3 );
#endif

#ifndef WITH_MOTOR
        dbgprintf( "T1: %d\tT2: %d\tT3: %d\n\r"
                   "R1: %d\tR2: %d\tR3: %d\n\r"
                   "D1: %d\tD2: %d\tD3: %d\n\r",
                   test_ticks1, test_ticks2, test_ticks3,
                   (int32_t)test_revs1, (int32_t)test_revs2, (int32_t)test_revs3,
                   test_dir1, test_dir2, test_dir3 );
#endif

        time = chThdSleepUntilWindowed( time, time + TIME_MS2I( 600 ) );
    }
}
