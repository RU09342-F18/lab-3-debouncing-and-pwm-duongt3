#include "msp.h"

/**
 * author: Timothy Duong
 * date: 10/5/18
 *
 * This is a software implementationg of pwm on the MSP432P401R. Pressing Button on P1.1 will increase the PWM by 10% and reset after 100% duty cycle.
 */

extern void LEDSetup();
extern void ButtonSetup();
extern void TimerSetup();
extern void InterruptSetup();

int main(void)
{
    NVIC->ISER[0] = 1 << ((TA0_N_IRQn) & 31);   //Enable timer interrupt
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);   //Enable port interrupt
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    LEDSetup();         //Setup LED
    ButtonSetup();      //Setup Button
    TimerSetup();       //Setup timer
    InterruptSetup();   //Setup interrupt

    while(1)
    {

    }
}

void InterruptSetup()
{
    P1->IE |= BIT1;     //Enable interrupt on P1.1
    P1->IES |= BIT1;    //Set interrupt to trigger on falling-edge
    P1->IFG &= ~BIT1;   //Clear interrupt flag

    __enable_irq();//Enables Global Interrupt
}

void TimerSetup(void)
{
    TIMER_A0->CTL = TIMER_A_CTL_TASSEL_1 |    // ACLK
                    TIMER_A_CTL_MC_1 |            // Continuous mode
                    TIMER_A_CTL_CLR |             // Clear TAR
                    TIMER_A_CTL_IE;               // Enable timer overflow interrupt
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE;   //Enable CCIE for CCR1
    TA0CCR0 = 29;
    TA0CCR1 = 15;

}

void LEDSetup(void)
{
    P1->SEL0 &= ~BIT0;      //Set P1 as GPIO
    P1DIR |= BIT0;          //Set P1.0 as output
    P1OUT &= ~BIT0;         //Initialize P1.0 as 0
}

void ButtonSetup(void)
{
    P1DIR &= ~BIT1; //Set P1.1 as input
    P1REN |= BIT1;  //Set P1.1 for pull-up resistor
    P1IN &= ~BIT1;  //Initialize input as 0
}


void TA0_N_IRQHandler(void)
{
    switch(TA0IV)       //TimerA interrupt vector values
    {
     case 2:
        P1->OUT ^= BIT0;     //Toggle LED
        break;
     case 4:
     case 6:
     case 8:
     case 10:
     case 12:
         break;
     case 14:               //Counter reaches CCR0
         if(TA0CCR1 == 0)
         {
             P1->OUT |= BIT0;
         }
         else{
            P1->OUT &= ~BIT0;    //Reset LED
         }
         break;
    }
}

void PORT1_IRQHandler(void)
{
    if(TA0CCR1 > 0)
    {
        TA0CCR1 -= 3;                 //Increase duty cycle by 10%
    }
    else
    {
        TA0CCR1 = 30;
    }
    P1IFG &= ~BIT1;                     //Clear interrupt flag
}
