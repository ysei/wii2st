#include <xc.h>

#include "STpic.h"


/******************************************************************************\
 * GLOBAL INTERRUPT HANDLER                                                   *
 * ------------------------                                                   *
 *                                                                            *
\******************************************************************************/
static void interrupt main_ISR()
{
    if(0) {}
#ifdef ISR_TIMER0_ENABLED
    /// Check the Timer 0 interrupt
    else if(INTCONbits.T0IE && INTCONbits.T0IF) {
        /// Clear Timer 0 triggered flag
        INTCONbits.T0IF = 0;

        /// Update the current time
        timer0_ISR();
    }
#endif

#ifdef ISR_INTB_ENABLED
    /// Check RB0/INT interrupt
    else if(INTCONbits.INTE && INTCONbits.INTF) {
        /// Clear RB0/INT triggered flag
        INTCONbits.INTF = 0;
        /// TODO: Handle INTB interruption...
    }
#endif

#ifdef ISR_RB47_ENABLED
    /// Check RB4/RB7 interrupt
    else if(INTCONbits.RBIE && INTCONbits.RBIF) {
        /// Clear RB4/RB7 triggered flag
        INTCONbits.RBIF = 0;
        /// TODO: Handle RB47 interruption...
    }
#endif
    
    /// From now on, it has to be a Peripheral interupt
    /// -----------------------------------------------

#ifdef ISR_ADCONV_ENABLED
    /// Check the A/D converter interrupt
    else if(PIE1bits.ADIE && PIR1bits.ADIF) {
        /// Clear A/D converter triggered flag
        PIR1bits.ADIF = 0;
        /// TODO: Handle ADCONV interruption...
    }
#endif

#ifdef ISR_USART_ENABLED
    /// Check the USART reception interrupt
    else if(PIE1bits.RCIE && PIR1bits.RCIF) {
        /// No need to clear the RCIF flag
        /// TODO: Handle USART RX interruption...
    }

    /// Check the USART transmission interrupt
    else if(PIE1bits.TXIE && PIR1bits.TXIF) {
        /// No need to clear the TXIF flag
        /// TODO: Handle USART TX interruption...
    }
#endif
        
#if defined(ISR_SSP_ENABLED) || defined(ISR_I2C_ENABLED)
    /// Check the SSP interrupt
    else if(PIE1bits.SSPIE && PIR1bits.SSPIF) {
        /// Clear the SSP triggered flag
        PIR1bits.SSPIF = 0;

        i2c_ISR();
    }
#endif

#ifdef ISR_CCP1_ENABLED
    /// Check the CCP1 interrupt
    else if(PIE1bits.CCP1IE && PIR1bits.CCP1IF) {
        /// Clear the CCP1 triggered flag
        PIR1bits.CCP1IF = 0;
        /// TODO: Handle CCP1 interruption...
    }
#endif

#ifdef ISR_TIMER2_ENABLED
    /// Check the Timer 2 interrupt
    else if(PIE1bits.TMR2IE && PIR1bits.TMR2IF) {
        /// Clear the Timer 2 triggered flag
        PIR1bits.TMR2IF = 0;
        /// TODO: Handle TIMER2 interruption...
    }
#endif

#ifdef ISR_TIMER1_ENABLED
    /// Check the Timer 1 interrupt
    else if(PIE1bits.TMR1IE && PIR1bits.TMR1IF) {
        /// Clear the Timer 2 triggered flag
        PIR1bits.TMR1IF = 0;
        /// TODO: Handle TIMER1 interruption...
    }
#endif

#ifdef ISR_EEPROM_ENABLED
    /// Check the EEPROM interrupt
    else if(PIE2bits.EEIE && PIR2bits.EEIF) {
        /// Clear the Timer 2 triggered flag
        PIR2bits.EEIF = 0;
        /// TODO: Handle EEPROM interruption...
    }
#endif

#ifdef ISR_I2C_ENABLED
    /// Check the collision interrupt
    else if(PIE2bits.BCLIE && PIR2bits.BCLIF) {
        /// Clear the Timer 2 triggered flag
        PIR2bits.BCLIF = 0;

        i2c_clif_ISR();
    }
#endif

#ifdef ISR_CCP2_ENABLED
    /// Check the CCP2 interrupt
    else if(PIE2bits.CCP2IE && PIR2bits.CCP2IF) {
        /// Clear the Timer 2 triggered flag
        PIR2bits.CCP2IF = 0;
        /// TODO: Handle CCP2 interruption...
    }
#endif
}

