#include <lcd.h>


uint8_t port_lcd    = 0;

uint8_t buf[1] = {0};

static bool isInitialized   = false;

/*
 * @brief   Initialization of LCD-display
 */
void lcdInit( void )
{
    if(isInitialized)
        return;
    chThdSleepMilliseconds( 20 );
    lcdSendHalfByte( 0x03 );
    chThdSleepMilliseconds( 4 );
    lcdSendHalfByte( 0x03 );
    chThdSleepMicroseconds( 100 );
    lcdSendHalfByte( 0x03 );
    chThdSleepMilliseconds( 1 );
    lcdSendHalfByte( 0x02 );
    chThdSleepMilliseconds( 1 );

    lcdSendByte( 0x28, 0 );         // 4 bit mode (DL = 0), 2 lines (N = 1)
    chThdSleepMilliseconds( 1 );
    lcdSendByte( 0x0C, 0 );         // show image on LCD (D = 1)
    chThdSleepMilliseconds( 1 );
    lcdSendByte( 0x01, 0 );         //  clean the screen
    chThdSleepMilliseconds(2);
    lcdSendByte(0x06, 0);           // cursor will move to the right
    chThdSleepMilliseconds(1);

    set_led();                      // turn on LED
    set_write();
}

/*
 * @brief   Show signed integer number on display
 * @note    The initial position of cursor is ( 0, 0 )
 */
void lcdSendNumber( uint8_t x, uint8_t y, int16_t num )
{
    char buff[16];                  // the width of LCD is 16 cells

    itoa( num, buff, 10 );          // 10 -> decimal radix
    lcdSendString( x, y, buff );
}


/*
 * @brief   Show string on display
 * @note    The initial position of cursor is ( 0, 0 )
 */
void lcdSendString( uint8_t x, uint8_t y, char* str )
{
    uint8_t i = 0;
    lcdSetCursorPos( x, y );
    while( str[i] != 0 )
    {
        lcdSendByte( str[i], 1 );
        i++;
    }
}


/*
 * @brief   Show 1 character on display
 * @note    The initial position of cursor is ( 0, 0 )
 */
void lcdSendChar( uint8_t x, uint8_t y, char ch )
{
    lcdSetCursorPos( x, y );
    lcdSendByte( ch, 1 );
}


/*
 * @brief   Set the position of cursor
 * @note    Work with 2 rows only
 *          The initial position is ( 0, 0 )
 */
void lcdSetCursorPos( uint8_t x, uint8_t y )
{
    uint8_t command = (y == 0) ? 0x80 : 0xc0;
    command |= x;
    lcdSendByte( command, 0);
}


/*
 * @brief   Clear LCD
 */
void lcdClear(void)
{
    lcdSendByte( 0x01, 0 );
    chThdSleepMicroseconds( 1500 );
}


/*
 * @brief   Send 8 significant bits (1 byte)
 */
void lcdSendByte( uint8_t byte, uint8_t mode )
{
    if( mode == 0 ) rs_reset();
    else rs_set();

    uint8_t hc = 0;
    hc = byte >> 4;
    lcdSendHalfByte( hc );
    lcdSendHalfByte( byte );
}


/*
 * @brief   Send 4 significant bits
 */
void lcdSendHalfByte( uint8_t byte )
{
    byte <<= 4;         // send 4 high bits (only that 4 bits are important)
    e_set();
    chThdSleepMicroseconds( 50 );
    lcdWriteByte( port_lcd|byte );
    e_reset();
    chThdSleepMicroseconds( 50 );
}


/*
 * @brief   Send information to LCD via I2C (aka send command)
 */
void lcdWriteByte( uint8_t byte )
{
    buf[0] = byte;
    // TODO: think how to remove unused variable msg here
    //       maybe change return type in i2cSimpleWrite
    //       from msg_t to void
    msg_t msg = i2cSimpleWrite(lcd_address, buf, 1, 1000);
}
