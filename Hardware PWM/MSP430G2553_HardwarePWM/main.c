#include <msp430.h> 


/**
 * main.c
 */
extern void LEDSetup();
extern void ButtonSetup();
extern void TimerSetup();
extern void InterruptSetup();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    LEDSetup();
    ButtonSetup();
    TimerSetup();
    InterruptSetup();

}

void InterruptSetup()
{
    P1IE |= BIT3;
    P1IES &= ~BIT3;
    P1IFG &= ~BIT3;

    _BIS_SR(GIE);       //Enable global interrupt

}

void TimerSetup(void)
{
    TA0CTL = TASSEL_1 + MC_1;    //Set timer to ACLK, UPMODE, Interrupt enable
    TA0CCR0 = 29;
    TA0CCR1 = 15;
    TA0CCTL1 = OUTMOD_2;
}

void LEDSetup(void)
{
    P1SEL |= BIT6;      //Set P1 as GPIO
    P1DIR |= BIT6;  //Set P1.0 as output
    P1OUT &= ~BIT6; //Initialize P1.0 as 0
}

void ButtonSetup(void)
{
    P1DIR &= ~BIT3; //Set P1.3 as input
    P1REN |= BIT3;  //Set P1.3 for pull-up resistor
    P1IN &= ~BIT3;  //Initialize input as 0
}


#pragma vector=PORT1_VECTOR
__interrupt void Port1(void)
{
    if(TA0CCR1 > 0)
    {
        TA0CCR1 -= 3;                 //Increase duty cycle by 10%
    }
    else
    {
        TA0CCR1 = 30;
    }
    P1IFG &= ~BIT3;                     //Clear interrupt flag
}

