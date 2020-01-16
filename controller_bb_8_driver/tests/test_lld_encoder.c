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
    lldControlValue_t   test_duty_prc   = 0; 
    lldControlValue_t   test_delta_prc  = 10; 
#endif 

    encoderTicksValue_t test_ticks  = 0; 
    encoderRevsValue_t  test_revs   = 0; 
    bool                test_dir    = 0; 

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        test_ticks  = lldGetEncoderTicks( );
        test_revs   = lldGetEncoderRevs( ); 
        test_dir    = lldGetEncoderDirection( ); 

#ifdef WITH_MOTOR
        char rcv_data   = sdGetTimeout( &SD3, TIME_IMMEDIATE ); 

        switch( rcv_data )
        {
            case 'a':
                test_duty_prc += test_delta_prc;
                break; 
            
            case 's':
                test_duty_prc -= test_delta_prc;
                break;

            case ' ':
                test_duty_prc = 0;
                break; 
            
            default:
                break; 
        }
        test_duty_prc = CLIP_VALUE( test_duty_prc, LLD_MOTOR_MIN_PRC, LLD_MOTOR_MAX_PRC ); 
        lldControlSetMotorPower( 1, test_duty_prc );

        dbgprintf( "Power: (%d)\tTicks: (%d)\tRevs: (%d)\tDir: (%d)\n\r",
                    test_duty_prc, test_ticks, (int32_t)test_revs, test_dir ); 
#endif

#ifndef WITH_MOTOR
        dbgprintf( "Ticks: (%d)\tRevs: (%d)\tDir: (%d)\n\r",
                    test_ticks, (int32_t)test_revs, test_dir ); 
#endif 

        time = chThdSleepUntilWindowed( time, time + MS2ST( 300 ) );
    }
}

