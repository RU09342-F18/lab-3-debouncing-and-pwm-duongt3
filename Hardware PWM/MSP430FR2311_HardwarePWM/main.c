
/**
 * author: Timothy Duong
 * date: 10/5/18
 *
 * This is a hardware implementation of PWM on the MSP430FR2311. Pressing the button on P1.1 will increase the PWM by 10% and reset the duty cycle after 100%.
 */

#include <msp430fr2311.h>

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;   //Disable the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //unlock GPIO
    P2DIR |= BIT0;              //Set P2.0 to OUT
    P1DIR |= BIT0;              //Set P1.0 to OUT
    P2SEL0 |= BIT0;             //Set P2.0 in peripheral mode
    P2SEL1 &= ~BIT0;            //Set P2.1 to (PWM)

    P1DIR &= ~BIT1;             //Set P1.1 as input
    P1IE |=  BIT1;              //P1.1 interrupt enabled
    P1IES |= BIT1;              //P1.1 falling edge
    P1REN |= BIT1;              //Enable Pull Up on P1.1
    P1OUT |= BIT1;              //Sets pull up resistor
    P1IFG &= ~BIT1;             //P1.1 IFG cleared

    TB1CCR0 = 1000;             //Set period
    TB1CCTL1 = OUTMOD_7;        //Set PWM to reset/set
    TB1CCR1 = 500;              //ON period
    TB1CTL = TBSSEL_2 + MC_1 + ID_2;        //SMCLK, up mode, divide by 4
    __bis_SR_register(LPM0_bits + GIE);     //Low power mode 0 and globel interrupt enable
}

#pragma vector =PORT1_VECTOR
__interrupt void PORT1(void)
{

    if(TB1CCR1 > 0) //if CCR1 is greater than 0
    {
        TB1CCR1 -=100; //reduce duty cycle by 10%
    }
    else
    {
        TB1CCR1 = 1000; //make duty cycle to 100%
    }

    P1IFG &= ~BIT1; //clear button interrupt flag
}
