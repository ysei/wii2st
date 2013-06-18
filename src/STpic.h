/* 
 * File:   PICst.h
 * Author: fjardon
 *
 * Created on 18 juin 2013, 21:28
 */

typedef unsigned char   byte;
typedef signed   char   sbyte;
typedef unsigned int    word;

#ifndef STPIC_H
#define	STPIC_H

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define ISR_TIMER0_ENABLED
#define ISR_I2C_ENABLED
#define ISR_USART_ENABLED

#define CPU_CLOCK_MHZ       (20)

#if CPU_CLOCK_MHZ == (20)
#define TIMER0_PREDIVISOR       (0b111)
#define TIMER0_JIFFIES_1MS      (10)
#define TIMER0_JIFFIES_10MS     (98)
#define TIMER0_JIFFIES_100MS    (977)
#define TIMER0_JIFFIES_1S       (9766)

/// There are 20K clocks / ms
/// There are 4 clocks / cycle
/// There are 20K/4 = 5K cycles/ms
/// There are 5K / 512 (2^9) jiffies/ms
///
/// Jiffies | ms
/// --------+------
///      10 |    1
///      98 |   10
///     977 |  100
///    9766 | 1000
///

/// I2C freq = CPU_CLOCK/(4*(I2C_PREDIVISOR+1))
/// 100KHz = 20MHz/(4*(49+1))
#define I2C_PREDIVISOR  (49)


#else
#error "Please configure TIMER0_PREDIVISOR/I2C_DIVISOR for you CPU clock"
#endif


void timer0_ISR(void);
void timer0_setup(void);

enum I2C_status
{
    I2C_OK         =  0,/// everything OK
    I2C_EAGAIN     = -1,/// please repeat (bus not idle)
    I2C_EBUSY      = -2,/// another operation is running, retry later
    I2C_ENOACK     = -4,/// received NOACK
    I2C_ECOLLISION = -8,/// collision detected
};


void i2c_ISR(void);
void i2c_bclif_ISR(void);
/// Setup the I2C subsystem
void i2c_setup(void);
/// Start a write operation. Returns I2C_status
sbyte i2c_write(byte address, byte* data, byte len);
/// Start a read operation. Returns I2C_status
sbyte i2c_read(byte address, byte* data, byte len);
/// try to flush the buffer for the current operation
/// If restart is true, send a repeated start when the
/// buffer is eventually flushed.
/// returns >0 number of bytes remaining in buffer
/// returns <0 on error: I2C_status
sbyte i2c_flush(byte restart);



#endif	/* STPIC_H */

