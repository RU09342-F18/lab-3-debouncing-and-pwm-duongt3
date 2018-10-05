#include <msp430.h> 


/**
 * main.c
 */

int timerDelay;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1SEL = 0;          //Set P1 as GPIO

	P1DIR &= ~BIT3;     //Set P1.3 as input
	P1IN  &= ~BIT3;     //Initialize P1.3 input as 0
	P1REN |= BIT3;      //Set P1.3 as pullup resistor

	P1DIR |= BIT0;      //Set P1.0 as output
	P1OUT &= ~BIT0;     //Initialize P1.0 output as 0

	P1IE |= BIT3;       //Enable interrupt for P1.3
	P1IES |= BIT3;       //Set interrupt for falling edge
	P1IFG &= ~BIT3;     //Clear interrupt flag on P1.3

	_BIS_SR(GIE);       //Enable global interrupt

	TA0CTL = TASSEL1 + MC_0 + TAIE; //Set TimerA0 to ACLK, No counting, overflow interrupt
	TA0CCR0 = 10000;

	timerDelay = 1;
	while(1){

	}
}


#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
    switch(TA0IV)       //TimerA interrupt vector values
    {
     case 2:
        break;
     case 4:
         break;
     case 10:                       //Counter reaches CCR0
         timerDelay = 1;            //Timer has delayed for desired length of time so set to true
         TA0CTL = TACLR;            //Clear counter
         TA0CTL = TASSEL_1 + MC_0;  //Stop Counting
         break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port1(void)
{
    if(timerDelay)                          //Only occurs if the timer has delayed
    {
        P1OUT ^= BIT0;                      //Flip state of LED
        TA0CTL = TACLR;                     //Reset Counter
        TA0CTL = TASSEL_1 + MC_1 + TAIE;    //Set TimerA to ACLK, UPMode, Interrupt enabled
        timerDelay = 0;                     //Set timerDelay to 0
    }
    P1IFG &= ~BIT3;                         //Clear interrupt flag
}

