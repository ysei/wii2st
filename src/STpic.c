/*
 * File:   STpic.c
 * Author: fjardon
 *
 * Created on 18 juin 2013, 21:15
 */


#include <xc.h>
#include "STpic.h"

void timer0_setup(void)
{
    /// Setup Timer 0
    OPTION_REGbits_t options = OPTION_REGbits;
    INTCONbits.TMR0IF = 0;
    options.PSA       = 0;
    options.PS        = TIMER0_PREDIVISOR;
    options.T0SE      = 0;
    options.T0CS      = 0;
    TMR0              = 0;
    OPTION_REGbits    = options;

#ifdef ISR_TIMER0_ENABLED
    /// Enable individual interrupts
    INTCONbits.TMR0IE = 1;
#endif
}

int main(void) {
    timer0_setup();
    i2c_setup();

    /// Configure all portB as output
    TRISB = 0;
    return 0;
}
