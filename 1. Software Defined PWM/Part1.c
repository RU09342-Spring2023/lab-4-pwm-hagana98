/*
 * Part1.c
 *
 *  Created on: Feb 22, 2023
 *      Author: Allison Hagan
 *
 */

#include <msp430.h>

unsigned short DutyCycle = 1000;

int main(void)
    {
        WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
        // Initialize our LEDS
        P1DIR |= BIT0;  // Initialize Red LED (P1.0)

        P6DIR |= BIT6;  // Initialize Green LED (P6.6)

        // Initialize our button
        P2DIR &= ~BIT3; // Initialize P2.3 Button

        P4DIR &= ~BIT1; // Initialize P4.1 Button

        // Configure Timer_A
        TB0CTL = TBSSEL_2 | MC_1 | TBCLR | TBIE;      // SMCLK, continuous mode, clear TBR, enable interrupt

        TB0CCTL1 |= CCIE;                             // Enable TB0 CCR1 Interrupt

        TB0CCR1 = DutyCycle;                          // Set CCR1 to the value to set the duty cycle

        __bis_SR_register(LPM3_bits | GIE);           // Enter LPM3, enable interrupts
        __no_operation();                             // For debugger

        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;

    }

// Timer0_B3 Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
        case TB0IV_NONE:
            break;                               // No interrupt
        case TB0IV_TBCCR1:
            P1OUT &= ~BIT0;
            break;                               // CCR1 Set the pin to a 0
        case TB0IV_TBCCR2:
            break;                               // CCR2 not used
        case TB0IV_TBIFG:
            P1OUT |= BIT0;                       // overflow Set the pin to a 1
            break;
        default:
            break;
    }
}
