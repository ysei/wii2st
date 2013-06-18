#include <xc.h>

#include "STpic.h"

/// I2C doesn't need to be handled by interrupts as clock stretching allow both
/// master and slave to slow down the clock to match their needs.
///
/// We just need to ensure we don't lock the bus forever.
///
/// I2C pins are SCL (RC3) and SDA (RC4)


/// Master write
/// ------------
/// 1) Generate a start condition
///    SSPCON2bits.SEN = 1
/// 2) Wait for SSPCON2bits.SEN to be cleared (10us ~ 50 Tcy)
///
/// 3) Send the 7 bits address and R/nW bit (0)
///    SSPBUF = (<SLAVEAddr> << 1) | 0
/// 4) Wait for SSPSTATbits.R_W to be cleared (9*10us ~ 360 Tcy)
///
/// 5) Check ACK
///    SSPCON2bits.ACKSTAT (1 == error)
///
/// 6) Write a byte
///    SSPBUF = <DATAbyte>
/// 7) Wait for SSPSTATbits.R_W to be cleared (9*10us ~ 360 Tcy)
///
/// 8) Check ACK
///    SSPCON2bits.ACKSTAT (1 == error)
///
///  =====
///  At this point we may either:
///  - send another byte
///  - generate a STOP condition to terminate
///  - generate a REPEATED START condition to continue the session with another
///    R/nW bit
/// ====
///
/// 9) Send REPEATED START condition
///    SSPCON2bits.RSEN = 1
/// 10) Wait for SSPCON2bits.RSEN to be cleared (10us ~ 50Tcy)
///
/// 9) Send STOP condition
///    SSPCON2bits.PEN = 1
/// 10) Wait for SSPCON2bits.PEN to be cleared (10us ~50Tcy)
///
/// Master Read
/// -----------
/// 1/2) Send START/REPEATED START condition
///
/// 3/4) Send address
///
/// 5) Check ACK from slave
///
/// 6) Read <DATAbyte>
///    SSPCON2bits.RCEN = 1
/// 7) Wait for SSPCON2bits.RCEN to be cleared
///
/// 8) Send ACK/NOACK (ack => slave will send another byte)
///    SSPCON2bits.ACKDT = <ACK/nACK>
///    SSPCON2bits.ACKEN = 1
/// 9) Wait for SSPCON2bits.ACKEN to be cleared
///
/// 10) Receive another byte/Send REPEATED START/STOP condition

/// 10 Bits Addresses
/// -----------------
/// When using 10 bits adresses, the session always looks like:
/// 1) Write to 1 1 1 1 0 a9 a8 0 a data byte containing the low 8 bits of the
///    address
/// 2) Send a repeated start condition, adressing 1 1 1 1 0 a9 a8 R/nW
///    using a R/nW bits as desired for a read or write. You do  not send the
///    low 8 bits of the address
/// 3) Continue the session as usual

/// Multi-Master Mode
/// -----------------
/// In this mode you must check before taking the bus.
/// Check that:
///    SSPSTATbits.P is set
/// OR
///    SSPSTATbits.P is cleared AND SSPSTATbits.S is cleared


/// Bus Collision
/// -------------
/// If at any time PIR2bits.BCLIF is set. You lost control. Go to idle state.


enum I2C_state
{
    S_I2C_IDLE,       /// Waiting for a read()/write() call
    S_I2C_START,      /// START condition sent, waiting for bus mastering
    S_I2C_RESTART,    /// Repeated START sent, waiting for bus mastering
    
    S_I2C_W_ADDR,     /// 7bits write address sent, waiting for ACK
    S_I2C_WRITING,    /// Byte written, waiting for R_W to clear, waiting for ACK
                    /// (ack) -> goto I2C_WRITING if buffer > 0
                    ///       -> goto I2C_STOP/RESTART depending on i2c_poll()
                    ///          parameter

    S_I2C_R_ADDR,     /// 7bits read address sent, waiting for ACK
    S_I2C_READING,    /// Reading byte, waiting for complete byte
                    /// (complete) -> send ACK/NOACK according to buffer status
                    ///            -> goto I2C_STOP/I2C_RESTART depending on
                    ///               i2c_poll() parameter
    
    S_I2C_STOP,       /// STOP condition sent, waiting for I2C bus

    S_I2C_COLLISION,  /// Bus collision
};

static byte  i2c_state;

static word  i2c_addr;
static byte* i2c_buf;
static byte  i2c_len;
static byte  i2c_pos;
static byte  i2c_nWr;

void i2c_setup(void)
{
    i2c_state = I2C_IDLE;
    i2c_len   = 0;
    i2c_nWr   = 0;
    
    /// First set RC3/4 as input (open drain)
    /// Other pins are set as output to avoid destructive CMOS leakage
    TRISC = 0b00011000;

    /// Enables I2C
    SSPCON = 0b00101000;

    /// Set I2C standard levels
    SSPSTAT = 0b10000000;

    /// Set I2C clock freq
    SSPADD = I2C_PREDIVISOR;
}



static void i2c_start(void)
{
    SSPCON2bits.SEN = 1;
    i2c_state = I2C_START;
}

static void i2c_stop(void)
{
    SSPCON2bits.PEN = 1;
    i2c_state = I2C_STOP;
}

static void i2c_restart(void)
{
    SSPCON2bits.RSEN = 1;
    i2c_state = I2C_RESTART;
}

static void i2c_send_w_addr(void)
{
    SSPBUF = (i2c_addr<<1);
}

static void i2c_send_r_addr(void)
{
    SSPBUF = (i2c_addr<<1) | 1;
}

static void i2c_write_byte(void)
{
    SSPBUF = i2c_buf[i2c_pos];
    ++ i2c_pos;
    -- i2c_len;
    i2c_state = I2C_WRITING;
}

void i2c_ISR(void)
{
}

void i2c_bclif_ISR(void)
{
}

/// Start a write operation. Returns I2C_status
sbyte i2c_write(byte address, byte* data, byte len)
{
    if(i2c_len != 0)
        return I2C_EBUSY;
    
    if(I2C_IDLE    != i2c_state
    && I2C_START   != i2c_state
    && I2C_RESTART != i2c_state)
        return I2C_EAGAIN;

    if(0 == len) {
        i2c_stop();
        return I2C_OK;
    }

    i2c_addr = address;
    i2c_buf  = data;
    i2c_len  = len;
    i2c_pos  = 0;
    i2c_nWr  = 0;

    if(I2C_IDLE == i2c_state)
        i2c_start();

    return I2C_OK;
}

/// Start a read operation. Returns I2C_status
sbyte i2c_read(byte address, byte* data, byte len)
{
    return I2C_EBUSY;
}

/// try to flush the buffer for the current operation
/// If restart is true, send a repeated start when the
/// buffer is eventually flushed.
/// returns >0 number of bytes remaining in buffer
/// returns <0 on error: I2C_status
sbyte i2c_flush(byte restart)
{
    switch(i2c_state) {
        case I2C_START:
        case I2C_RESTART:
            /// Check RSEN | SEN
            if(SSPCON2bits.SEN || SSPCON2bits.RSEN)
                return I2C_EAGAIN;
            
            if(i2c_nWr) {
                i2c_send_r_addr();
                i2c_state = I2C_R_ADDR;
            } else {
                i2c_send_w_addr();
                i2c_state = I2C_W_ADDR;
            }
            break;

        case I2C_W_ADDR:
            /// Check ACK
            if(SSPCON2bits.ACKSTAT) {
                i2c_state = I2C_IDLE;
                return I2C_ENOACK;
            }
            i2c_write_byte();
            break;

        case I2C_WRITING:
            if(SSPSTATbits.R_W)
                return I2C_EAGAIN;
            if(SSPCON2bits.ACKSTAT) {
                if(restart) {
                    
                }
                i2c_state = I2C_IDLE;
                return I2C_ENOACK;
            }
            
        default:
            break;
    }
    return i2c_len;
}
