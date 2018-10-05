#include <msp430.h> 
/**
 * author: Timothy Duong
 * date: 10/5/18
 *
 * This file solves debouncing on the MSP430F5529
 */

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       //Stop watchdog timer

    P1DIR |= BIT0;                  //Set P1.0 as output

    P2DIR &= ~BIT1;                 //Set P2.1 as input
    P2REN |= BIT1;                  //Enable Pull Up on SW1 (P2.1)
    P2OUT |= BIT1;                  //Sets pull up resistor

    P2IE |=  BIT1;                  //P2.1 interrupt enabled
    P2IES |= BIT1;                  //P2.1 falling edge
    P2IFG &= ~BIT1;                 //P2.1 IFG cleared

    TB0CTL = TBSSEL_1 + ID_3 + MC_0;//Set timer to ACLK, divide by 8, not count
    TB0CCTL0 = CCIE;                //Enable capture compare interrupt
    TB0CCR0 = 1000;                 //Delay for debouncing

    _BIS_SR(LPM0_bits + GIE);       //enable global interrupt
}


#pragma vector =PORT2_VECTOR
__interrupt void Button_down(void)
{
    TB0CTL = TBSSEL_1 + ID_3 + MC_1;    //Enable Timer
    P2IE &= ~BIT1;                      //Disable button interrupt
    P2IFG &= ~BIT1;                     //clear button interrupt flag
}

#pragma vector =TIMER0_B0_VECTOR
__interrupt void TIMER0_B1(void)
{
    P1OUT ^= BIT0;                      //Toggle P1.0 LED
    TB0CTL = TBSSEL_1 + ID_3 + MC_0;    //Stop timer
    TB0CTL |= TBCLR;                    //Clear timer
    P2IFG &= ~BIT1;                     //Clear button interrupt flag
    P2IE |=  BIT1;                      //Enable button interrupt
}
