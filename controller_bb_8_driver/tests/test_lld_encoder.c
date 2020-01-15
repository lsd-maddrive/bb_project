#include <tests.h>
#include <lld_encoder.h>

/**
 * @brief   Test common encoder routine 
 * @note    Print number of ticks, (int)revs and state of direction
 *          0   - clockwise 
 *          1   - counterclockwise  
 */
void testEncoderRoutine( void )
{
    debug_stream_init( );
    lldEncoderInit( );

    encoderTicksValue_t test_ticks  = 0; 
    encoderRevsValue_t  test_revs   = 0; 
    bool                test_dir    = 0; 

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        test_ticks  = lldGetEncoderTicks( );
        test_revs   = lldGetEncoderRevs( ); 
        test_dir    = lldGetEncoderDirection( ); 

        dbgprintf( "Ticks: (%d)\tRevs: (%d)\tDir: (%d)\n\r",
                    test_ticks, test_revs, test_dir ); 

        time = chThdSleepUntilWindowed( time, time + MS2ST( 300 ) );
    }
}